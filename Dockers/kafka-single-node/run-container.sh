#!/usr/bin/env bash


# Create network if not exists
line=$(docker network ls |grep my-net |wc -l)
if [ "$line" -eq 0 ]; then
    docker network create \
        --subnet 10.0.0.0/24 \
        my-net
fi

docker run \
    -it \
    --name kafka-single-node \
    --ip 10.0.0.30 \
    --network my-net \
    jhleeeme/kafka-single-node:2.13-3.3.1-alpine
