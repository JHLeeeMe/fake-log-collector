"""utils

Attributes:
    logger: Optional[logging.Logger] = None
        Logger를 할당받을 글로벌 변수

Functions:
    retry(func, *args, retry_cnt: int = 5, **kwargs) -> Optional[Any]
        주어진 함수 에러시 재시도
    get_logger() -> logging.Logger
        글로벌 변수 logger를 리턴

    _init_logger()
        글로벌 변수 logger 초기화

"""
import os
import time
import logging
from logging import handlers
from typing import Optional, Any


logger: Optional[logging.Logger] = None


def retry(func, *args, retry_cnt: int = 5, **kwargs) -> Optional[Any]:
    my_logger = get_logger()

    if retry_cnt < 0:
        retry_cnt = 5

    for i in range(retry_cnt):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            if i == 0:
                my_logger.error(e)
            my_logger.error(f"{func.__name__}() failed... Trying Again.")
        time.sleep(3)
    return None


def get_logger() -> logging.Logger:
    global logger
    if logger is None:
        _init_logger()

    return logger


def _init_logger():
    global logger

    output_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'logs')
    print(output_path)
    os.makedirs(output_path, exist_ok=True)
    logger = logging.getLogger('error_detector')
    logger.setLevel(logging.INFO)

    log_handler = handlers.TimedRotatingFileHandler(
        filename=f'{output_path}/error.log', when='midnight', encoding='utf-8'
    )
    log_formatter = logging.Formatter(
        '[%(asctime)s - %(levelname)s] %(message)s'
    )
    log_handler.setFormatter(log_formatter)
    log_handler.suffix = '-%Y%m%d'

    logger.addHandler(log_handler)

