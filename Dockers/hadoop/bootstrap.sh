#!/bin/bash

: ${HADOOP_PREFIX:=/opt/hadoop}

# installing libraries if any - (resource urls added comma separated to the ACP system variable)
cd $HADOOP_PREFIX/share/hadoop/common ; for cp in ${ACP//,/ }; do  echo == $cp; curl -LO $cp ; done; cd -

# Start ssh
echo "# $(hostname -f): ssh start"
service ssh start

# Modify workers
echo "# $(hostname -f): exec create-workers.sh"
/etc/create-workers.sh $WORKER_NUM
unset WORKER_NUM

# in master server
if [[ $(uname -n) == "master" ]]; then
    start-dfs.sh && start-yarn.sh
fi

# health check and write to tmpfs
echo "Health checking..."
case $(uname -n) in
    "master")
        for i in $(seq 1 10); do
            if [[ $(jps |wc -l) == 3 ]]; then  # Jps, ResourceManager, NameNode
                jps |grep -v Jps >> /tmpfs/health_check.txt
                break
            elif [[ $i == 10 ]]; then
                echo "$(hostname -f): start server error..."
                exit 1
            fi
            sleep 2
        done
        ;;
    "worker-1")
        for i in $(seq 1 10); do
            if [[ $(jps |wc -l) == 4 ]]; then  # Jps, NodeManager, DataNode, SecondaryNameNode
                jps |grep -v Jps >> /tmpfs/health_check.txt
                break
            elif [[ $i == 10 ]]; then
                echo "$(hostname -f): start server error..."
                exit 1
            fi
            sleep 2
        done
        ;;
    *)
        for i in $(seq 1 10); do
            if [[ $(jps |wc -l) == 3 ]]; then  # Jps, NodeManager, DataNode
                jps |grep -v Jps >> /tmpfs/health_check.txt
                break
            elif [[ $i == 10 ]]; then
                echo "$(hostname -f): start server error..."
                exit 1
            fi
            sleep 2
        done
        ;;
esac

echo "# $(hostname -f): exec /bin/bash"
#CMD=${1:-"exit 0"}
CMD="$1"
if [[ "$CMD" == "-d" ]]; then
    service sshd stop
    /usr/sbin/sshd -D -d
else
    #/bin/bash -c "$*"
    /bin/bash
fi
