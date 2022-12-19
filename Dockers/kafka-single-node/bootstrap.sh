#!/usr/bin/env bash


echo "Starting zookeeper"
zookeeper-server-start.sh -daemon /opt/kafka/config/zookeeper.properties

echo "Starting kafka"
#kafka-server-start.sh /opt/kafka/config/server.properties
kafka-server-start.sh -daemon /opt/kafka/config/server.properties

echo "Health checking..."
for i in $(seq 1 10); do
    if [[ $(jps |wc -l) == 3 ]]; then  # Jps, Kafka, QuorumPeerMain
        jps |grep -v Jps >> /tmpfs/health_check.txt
        break
    elif [[ $i == 10 ]]; then
        echo "start server error..."
        exit 1
    fi
    sleep 2
done

echo "$(hostname -f): exec /bin/bash"
/bin/bash
