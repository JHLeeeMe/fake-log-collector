#!/bin/bash


cat <<EOF
# -------------------- #
#   start-consumer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=${PROJECT_ROOT}/src/cpp
CONSUMER_CPP_ROOT=${CPP_ROOT}/consumer-cpp
LIB=${CPP_ROOT}/lib

echo "exec consumer-cpp"
${CONSUMER_CPP_ROOT}/build/consumer-cpp
