#!/bin/bash

: ${HADOOP_PREFIX:=/opt/hadoop}

# installing libraries if any - (resource urls added comma separated to the ACP system variable)
cd $HADOOP_PREFIX/share/hadoop/common ; for cp in ${ACP//,/ }; do  echo == $cp; curl -LO $cp ; done; cd -

# Start ssh
echo "$(hostname -f): ssh start"
service ssh start

# Modify workers & start server in master
if [[ $(uname -n) == "master" ]]; then
    echo "$(hostname -f): exec create-workers.sh"
    /etc/create-workers.sh $WORKER_NUM
    unset WORKER_NUM
    start-dfs.sh && start-yarn.sh
fi

# health check and write to tmpfs
echo "Health checking..."
if [[ $(uname -n) == "worker-1" ]]; then
    jps_wc_l=4  # (Jps, NodeManager, DataNode, SecondaryNameNode)
else
    jps_wc_l=3  # (Jps, ResourceManager, NameNode) || (Jps, NodeManager, DataNode)
fi

for i in $(seq 1 10); do
    if [[ $(jps |wc -l) == $jps_wc_l ]]; then
        jps |grep -v Jps >> /tmpfs/health_check.txt
        break
    elif [[ $i == 10 ]]; then
        echo "$(hostname -f): start server error..."
    fi
    sleep 2
done

echo "$(hostname -f): exec /bin/bash"
CMD="$1"
if [[ "$CMD" == "-d" ]]; then
    service sshd stop
    /usr/sbin/sshd -D -d
else
    /bin/bash
fi
