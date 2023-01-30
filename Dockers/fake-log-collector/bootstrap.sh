#!/usr/bin/env bash


echo "alias ll='ls -alF'" >> /root/.bashrc

if [[ ! -e .tmpfs/health_check.txt ]]; then
    touch .tmpfs/health_check.txt
fi

echo "Health checking..."
for i in $(seq 1 10); do
    # master: ResourceManager, NameNode
    # worker-1: NodeManager, DataNode, SecondaryNameNode
    # worker-2: NodeManager, DataNode
    # worker-3: NodeManager, DataNode
    # kafka-single-node: Kafka, QuorumPeerMain
    if [[ $(cat .tmpfs/health_check.txt |wc -l) -ge 11 ]]; then
        rm -rf .tmpfs/
        break
    elif [[ $i == 10 ]]; then
        echo "hadoop or kafka server start error..."
        exit 1
    fi

    sleep 3
done

echo "Compile..."
/workspace/src/bin/compile-transformer.sh
/workspace/src/bin/compile-consumer_cpp.sh
/workspace/src/bin/compile-save_to_hdfs.sh
/workspace/src/bin/compile-write_to_influxdb.sh

# Exec
/workspace/src/bin/start-producer.sh
/workspace/src/bin/start-write_to_influxdb.sh
/workspace/src/bin/start-save_to_hdfs.sh
/workspace/src/bin/start-consumer_cpp.sh
/workspace/src/bin/start-transformer.sh

/bin/bash
