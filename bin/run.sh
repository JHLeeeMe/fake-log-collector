#!/bin/bash


PROJECT_ROOT=..

if [[ -d ${PROJECT_ROOT}/tmpfs ]]; then
    echo "exec: sudo rm -rf ${PROJECT_ROOT}/tmpfs"
    sudo rm -rf ${PROJECT_ROOT}/tmpfs
fi

CMD=$1
if [[ $CMD == "-d" ]]; then
    docker-compose -f ${PROJECT_ROOT}/docker-compose.yml up -d
else
    docker-compose -f ${PROJECT_ROOT}/docker-compose.yml up
fi
