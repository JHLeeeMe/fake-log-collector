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

echo "pip installing..."
pip install -r ./src/python/log-generators/requirements.txt

echo "exec log_generator.py"
python ./src/python/log-generators/log_generator.py
