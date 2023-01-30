#!/bin/bash


cat <<EOF
# ------------------------------ #
#  compile-write_to_influxdb.sh  #
# ------------------------------ #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
WRITE_TO_INFLUXDB_PATH=${CPP_ROOT}/write-to-influxdb
LIB=${WRITE_TO_INFLUXDB_PATH}/lib

if [[ -d ${WRITE_TO_INFLUXDB_PATH}/build/ ]]; then
    rm -rf ${WRITE_TO_INFLUXDB_PATH}/build
fi

mkdir ${WRITE_TO_INFLUXDB_PATH}/build
cd ${WRITE_TO_INFLUXDB_PATH}/build
cmake ..
make -j4
cd ${PROJECT_ROOT}
