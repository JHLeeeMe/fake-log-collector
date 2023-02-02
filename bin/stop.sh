#!/bin/bash


PROJECT_ROOT=..

if [[ -d ${PROJECT_ROOT}/tmpfs ]]; then
    echo "exec: sudo rm -rf ${PROJECT_ROOT}/tmpfs"
    sudo rm -rf ${PROJECT_ROOT}/tmpfs
fi

if [[ -e ${PROJECT_ROOT}/.env ]]; then
    rm -rf ${PROJECT_ROOT}/.env
fi

docker-compose -f ${PROJECT_ROOT}/docker-compose.yml down
