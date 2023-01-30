#!/bin/bash


cat <<EOF
# ---------------------------- #
#  start-write_to_influxdb.sh  #
# ---------------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
WRITE_TO_INFLUXDB_ROOT=${CPP_ROOT}/write-to-influxdb
LIB=${WRITE_TO_INFLUXDB_ROOT}/lib

echo "exec write-to-influxdb"
${WRITE_TO_INFLUXDB_ROOT}/build/write-to-influxdb &
