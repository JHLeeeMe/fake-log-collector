#!/usr/bin/env bash


if [[ ! -e .tmpfs/health_check.txt ]]; then
    touch .tmpfs/health_check.txt
fi

echo "Hadoop health checking..."
for i in $(seq 1 10); do
    # master: ResourceManager, NameNode
    # worker-1: NodeManager, SecondaryNameNode, DataNode
    # worker-2: NodeManager, DataNode
    # worker-3: NodeManager, DataNode
    if [[ $(cat .tmpfs/health_check.txt |wc -l) -ge 9 ]]; then
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

# exec /bin/bash
#/bin/bash
