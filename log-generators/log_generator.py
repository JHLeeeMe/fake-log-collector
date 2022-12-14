"""log_generator.py

fake log generator

Functions:
    gen_nginx_log() -> None

    _get_params() -> str

"""
import time
import random
from pytz import timezone
from datetime import datetime

from faker import Faker


fake = Faker()


def gen_nginx_log() -> str:
    """Nginx Log Generator

        default format:
            '$remote_addr - $remote_user [$time_local] "$request" '
            '$status $body_bytes_sent "$http_referer" '
            '"$http_user_agent" "$http_x_forwarded_for"';
        e.g.
            192.168.0.250 - - [29/Apr/2017:02:17:41 +0900] "GET / HTTP/1.1"
            200 396 "-"
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36"

    """
    remote_addr = fake.ipv4()
    remote_user = '-'
    time_local = datetime.now(tz=timezone('Asia/Seoul')).strftime('%d/%b/%Y:%H:%M:%S %z')
    method = random.choice(seq=['GET', 'POST'])
    if method == 'GET':
        uri_path = random.choice(seq=['', fake.uri_path(), fake.uri_path() + '?' + _get_params()])
    else:
        uri_path = random.choice(seq=['', fake.uri_path()])
    request = f"{method} {'/' + uri_path} HTTP/1.1"

    status = random.choice(seq=[200, 404, 500])
    body_bytes_sent = random.randint(1, 8192)
    http_referer = random.choice(seq=[
                       '-',
                       fake.url() + random.choice(seq=['', fake.uri_path()])
                   ])
    http_user_agent = fake.user_agent()

    log = f'{remote_addr} - {remote_user} [{time_local}] {request} ' \
          f'{status} {body_bytes_sent} "{http_referer}" ' \
          f'"{http_user_agent}"'

    return log


def _get_params() -> str:
    params = ''
    for _ in range(random.randint(1, 4)):
        params += f"{fake.name().split()[0]}={fake.name().split()[0]}&"
    return params[:-1]


if __name__ == '__main__':
    while True:
        fake_nginx_log = gen_nginx_log()
        print(fake_nginx_log)
        time.sleep(1)

