#!/bin/bash

RETENTION_POLICY="WITH DURATION 2h REPLICATION 1 SHARD DURATION 1h NAME d_2h_sgd_1h_r_1"
DATABASES=("fake_log","container_metrics")

IFS=','
for db in ${DATABASES[@]}; do
    influx -host localhost \
           -port 8086 \
           -execute "CREATE DATABASE ${db} ${RETENTION_POLICY}"
done
