#!/bin/bash


cat <<EOF
# ----------------------- #
#   compile-consumer.sh   #
# ----------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=$PROJECT_ROOT/src/cpp
LIBS=$CPP_ROOT/libs

if [[ -d ${CPP_ROOT}/build/ ]]; then
    rm -rf ${CPP_ROOT}/build
fi

mkdir ${CPP_ROOT}/build
cd ${CPP_ROOT}/build
cmake ..
make -j4
cd ${PROJECT_ROOT}
