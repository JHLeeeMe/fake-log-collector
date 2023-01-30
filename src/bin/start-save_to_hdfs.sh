#!/bin/bash


cat <<EOF
# ----------------------- #
#  start-save_to_hdfs.sh  #
# ----------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
SAVE_TO_HDFS_PATH=${CPP_ROOT}/save-to-hdfs
LIB=${SAVE_TO_HDFS_PATH}/lib

echo "exec save-to-hdfs"
${SAVE_TO_HDFS_PATH}/build/save-to-hdfs &
