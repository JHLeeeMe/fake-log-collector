#!/bin/bash


cat <<EOF
# ------------------------- #
#  compile-save_to_hdfs.sh  #
# ------------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
SAVE_TO_HDFS_PATH=${CPP_ROOT}/save-to-hdfs
LIB=${SAVE_TO_HDFS_PATH}/lib

if [[ -d ${SAVE_TO_HDFS_PATH}/build/ ]]; then
    rm -rf ${SAVE_TO_HDFS_PATH}/build
fi

mkdir ${SAVE_TO_HDFS_PATH}/build
cd ${SAVE_TO_HDFS_PATH}/build
cmake ..
make -j4
cd ${PROJECT_ROOT}
