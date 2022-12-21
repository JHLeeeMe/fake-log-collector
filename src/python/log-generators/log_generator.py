"""log_generator.py

Fake Log Generator
  (Nginx, Apache, Flask, ...)

Attributes:
    fake: Faker
    log_elem: LogElements

Classes:
    LogElements
        A class to store fake data created using Faker().

Functions:
    gen_nginx_log() -> str
    gen_apache_log() -> str
    gen_flask_log() -> str

"""
import os
import time
import random
import threading
from datetime import datetime

from pytz import timezone
from faker import Faker
from kafka import KafkaProducer

import utils
import configs

logger = utils.get_logger()
fake = Faker()


class LogElements:
    def __init__(self):
        self.remote_addr = fake.ipv4()
        self.remote_user = random.choice(seq=['-', fake.last_name()])
        self.time_local = datetime.now(tz=timezone('Asia/Seoul')).strftime('%d/%b/%Y:%H:%M:%S %z')
        self.method = random.choice(seq=['GET', 'POST'])
        if self.method == 'GET':
            self.uri_path = random.choice(seq=['', fake.uri_path(), fake.uri_path() + '?' + self._get_params()])
        else:
            self.uri_path = random.choice(seq=['', fake.uri_path()])
        self.request = f"{self.method} {'/' + self.uri_path} HTTP/1.1"
        self.status = random.choice(seq=[200, 404, 500])
        self.body_bytes_sent = random.randint(1, 8192)
        self.http_referer = random.choice(seq=[
                                '-',
                                fake.url() + random.choice(seq=['', fake.uri_path()])
                            ])
        self.http_user_agent = fake.user_agent()

    def refresh(self):
        self.__init__()

    def _get_params(self) -> str:
        params = ''
        for _ in range(random.randint(1, 4)):
            params += f"{fake.first_name()}={fake.last_name()}&"
        return params[:-1]


log_elem = LogElements()


def gen_nginx_log() -> str:
    """Nginx Log Generator
    https://nginx.org/en/docs/http/ngx_http_log_module.html#access_log

    Nginx access log generator

    combined format:
        '$remote_addr - $remote_user [$time_local] '
        '"$request" '$status $body_bytes_sent '
        '"$http_referer" "$http_user_agent"'
      e.g.
        192.168.0.250 - - [29/Apr/2017:02:17:41 +0900]
        "GET / HTTP/1.1" 200 396
        "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36"

    """
    log = f'{log_elem.remote_addr} - {log_elem.remote_user} [{log_elem.time_local}] ' \
          f'"{log_elem.request}" {log_elem.status} {log_elem.body_bytes_sent} ' \
          f'"{log_elem.http_referer}" "{log_elem.http_user_agent}"'

    return log


def gen_apache_log(format: str = 'combined') -> str:
    """Apache Log Generator
    https://httpd.apache.org/docs/2.4/logs.html

    Apache access log generator

    common format:
        "%h %l %u %t \"%r\" %>s %b"
      e.g.
        127.0.0.1 - - [10/Oct/2000:13:55:36 -0700] "GET /apache_pb.gif HTTP/1.0" 200 2326

    combined format:
        "%h %l %u %t \"%r\" %>s %b \"%{Referer}i\" \"%{User-agent}i\""
      e.g.
        127.0.0.1 - frank [10/Oct/2000:13:55:36 -0700] "GET /apache_pb.gif HTTP/1.0" 200 2326 "http://www.example.com/start.html" "Mozilla/4.08 [en] (Win98; I ;Nav)"

    """
    log = f'{log_elem.remote_addr} - {log_elem.remote_user} [{log_elem.time_local}] ' \
          f'"{log_elem.request}" {log_elem.status} {log_elem.body_bytes_sent}'

    if format == 'combined':
        log += f' "{log_elem.http_referer}" "{log_elem.http_user_agent}"'

    return log


def gen_flask_log() -> str:
    """Flask Log Generator

    Flask access log generator

    default format (version >= 1.0):
        [%(asctime)s] %(levelname)s in %(module)s: %(message)s [in %(pathname)s:%(lineno)d]
      e.g.
        [2022-12-14 12:34:56] ERROR in app: An error occurred while processing a request [in /path/to/app.py:123]

    """
    asctime = datetime.now(tz=timezone('Asia/Seoul')).strftime('%Y-%m-%d %H:%M:%S')
    module = random.choice(seq=['app1', 'app2', 'app3'])
    levelname = random.choice(seq=['INFO', 'WARN', 'ERROR', 'DEBUG'])
    if levelname == 'ERROR':
        message = f'An error occurred while processing a request '
    else:
        message = f'{fake.street_address()} '

    log = f'[{asctime}] {levelname} in {module}: {message} ' \
          f'[in /workspace/./src/python/log-generators/{module}.py:{random.randint(1, 500)}]'

    return log


def send_to_topic(producer: KafkaProducer, key: str, topic: str = 'raw'):
    assert (key in ['nginx', 'apache', 'flask'])
    assert (topic in ['raw'])

    while True:
        if key == 'nginx':
            fake_log = gen_nginx_log()
            log_elem.refresh()
        elif key == 'apache':
            fake_log = gen_apache_log(format='combined')
            log_elem.refresh()
        else:
            fake_log = gen_flask_log()

        producer.send(topic,
                      key=bytes(key, 'utf8'), value=bytes(fake_log, 'utf8'))

        print(f'{os.getpid()}(pid): log send to "{topic}" topic.')
        time.sleep(random.uniform(0.1, 3))


if __name__ == '__main__':
    # Create 'raw' topic
    #   retention.ms: 1 hour,
    #   delete.retention.ms = 0.5 hour
    #   retention.bytes: 5GB
    configs.create_topic(topic='raw', num_partitions=3, replica=1)

    producer: KafkaProducer | None = None
    for i in range(5):
        try:
            producer = KafkaProducer(bootstrap_servers=['kafka-single-node:9092'])
        except Exception as e:
            if i == 0 or i == 4:
                logger.error(f'retry {i+1}: {e}')
            time.sleep(3)
            continue

    if producer is None:
        logger.error('Kafka Producer is None.')
        exit()

    thread_pool = []
    for e in ['nginx', 'apache', 'flask']:
        thread_pool.append(threading.Thread(target=send_to_topic, args=(producer, e, 'raw')))

    try:
        for t in thread_pool:
            t.start()
    except KeyboardInterrupt:
        producer.flush()
        producer.close()
    except Exception as e:
        logger.error(e)
        exit()

