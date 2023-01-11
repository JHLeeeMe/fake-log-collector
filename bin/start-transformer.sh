#!/bin/bash


cat <<EOF
# ----------------------- #
#   start-transformer.sh  #
# ----------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=${PROJECT_ROOT}/src/java
LIB=${JAVA_ROOT}/lib

echo "exec Transformer"
java \
    -cp ${JAVA_ROOT}/target/:${LIB}/kafka-clients-3.3.1.jar:${LIB}/slf4j-api-1.7.36.jar:${LIB}/kafka-streams-3.3.1.jar \
    Transformer
