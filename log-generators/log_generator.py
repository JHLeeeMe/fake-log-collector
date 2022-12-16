"""log_generator.py

Fake Log Generator
  (Nginx, Apache, Flask, ...)

Attributes:
    fake: Faker
    log_elem: LogElements

Classes:
    LogElements

Functions:
    gen_nginx_log() -> str
    gen_apache_log() -> str
    gen_flask_log() -> str:

"""
import os
import time
import random
from datetime import datetime

from pytz import timezone
from faker import Faker
from kafka import KafkaProducer

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
        '"$http_referer" "$http_user_agent"';
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
    app_name = random.choice(seq=['app1', 'app2', 'app3'])
    log_level = random.choice(seq=['INFO', 'WARN', 'ERROR', 'DEBUG'])
    log = f'[{log_elem.time_local}] {log_level} in {app_name}: '
    if log_level == 'ERROR':
        log += f'An error occurred while processing a request '
    else:
        log += f'{fake.street_address()} '
    log +=  f'[in {os.path.dirname(__file__)}/{app_name}.py:{random.randint(1, 500)}]'

    return log


if __name__ == '__main__':
    producer = KafkaProducer(bootstrap_servers=['kafka-single-node:9092'])

    while True:
        print('----------- Nginx -----------')
        fake_nginx_log = gen_nginx_log()
        producer.send('raw', bytes(fake_nginx_log, encoding='utf8'))
        print('send data to "raw" topic')
        log_elem.refresh()

        print('----------- Apache -----------')
        fake_apache_log = gen_apache_log(format='combined')
        producer.send('raw', bytes(fake_apache_log, encoding='utf8'))
        print('send data to "raw" topic')
        log_elem.refresh()

        print('----------- Flask -----------')
        fake_flask_log = gen_flask_log()
        producer.send('raw', bytes(fake_flask_log, encoding='utf8'))
        print('send data to "raw" topic')
        log_elem.refresh()

        print()

        time.sleep(1)

