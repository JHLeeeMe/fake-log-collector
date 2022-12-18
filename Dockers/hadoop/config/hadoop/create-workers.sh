#!/usr/bin/env bash


if [ -z $WORKER_NUM ]; then
    echo "# $(hostname -f): WORKER_NUM is not found."
    exit 0
fi

# Create workers
if [[ $(cat $HADOOP_CONF_DIR/workers) == "localhost" ]]; then
    # remove line 1 (localhost) in workers
    sed -i 1d $HADOOP_CONF_DIR/workers

    for i in $(seq 1 $WORKER_NUM); do
        echo "worker-$i" >> $HADOOP_CONF_DIR/workers
    done
fi
