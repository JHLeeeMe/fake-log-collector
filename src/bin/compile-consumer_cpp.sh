#!/bin/bash


cat <<EOF
# ------------------------- #
#  compile-consumer_cpp.sh  #
# ------------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
CONSUMER_CPP_ROOT=${CPP_ROOT}/consumer-cpp
LIB=${CONSUMER_CPP_ROOT}/lib

if [[ -d ${CONSUMER_CPP_ROOT}/build/ ]]; then
    rm -rf ${CONSUMER_CPP_ROOT}/build
fi

mkdir ${CONSUMER_CPP_ROOT}/build
cd ${CONSUMER_CPP_ROOT}/build
cmake ..
make -j4
cd ${PROJECT_ROOT}
