#!/usr/bin/env bash


############################################################
# 일단 보류
#   이유: 일단 필요가 없음, 구현한 시스템에선 자동 생성됨.
#         docker 관련해서 없는게 괜찮다고 판단.
############################################################

if [ -z "$CREATE_TOPICS" ]; then
    exit 0
fi

# e.g. "topic1 topic2 topic3" -> "topic1,topic2,topic3"
IFS=","; topics="$CREATE_TOPICS"

for t in $topics; do
    kafka-topics.sh --create \
        --zookeeper localhost:2181 \
        --replication-factor 1 \
        --partitions 1 \
        --topic "$t"

    echo "created topic: ${topic}"
done
