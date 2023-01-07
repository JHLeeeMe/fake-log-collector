#!/bin/bash


cat <<EOF
# -------------------- #
#   start-consumer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
CPP_ROOT=$PROJECT_ROOT/src/cpp
LIBS=$CPP_ROOT/libs

echo "exec consumer-cpp"
${CPP_ROOT}/build/consumer-cpp
