#!/bin/bash


PROJECT_ROOT=..

if [[ -d ${PROJECT_ROOT}/tmpfs ]]; then
    echo "exec: sudo rm -rf ${PROJECT_ROOT}/tmpfs"
    sudo rm -rf ${PROJECT_ROOT}/tmpfs
fi

if [[ ! -e ${PROJECT_ROOT}/.env ]]; then
    echo "DOCKER_SOCK_GROUP=$(stat -c '%g' /var/run/docker.sock)" > ${PROJECT_ROOT}/.env
fi

CMD=$1
if [[ $CMD == "-d" ]]; then
    docker-compose -f ${PROJECT_ROOT}/docker-compose.yml up -d
else
    docker-compose -f ${PROJECT_ROOT}/docker-compose.yml up
fi
