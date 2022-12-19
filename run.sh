#!/bin/bash


if [[ -d ./tmpfs ]]; then
    rm -rf ./tmpfs
fi

CMD=$1
if [[ $CMD == "-d" ]]; then
    docker-compose up -d
else
    docker-compose up
fi
