#!/bin/bash


cat <<EOF
# -------------------------- #
#   compile-transformer.sh   #
# -------------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=${PROJECT_ROOT}/src/java
TRANSFORMER_PATH=${JAVA_ROOT}/transformer
LIB=${TRANSFORMER_PATH}/lib

if [[ -d ${TRANSFORMER_PATH}/target/ ]]; then
    rm -rf ${TRANSFORMER_PATH}/target
fi

echo "compile Transformer"
javac \
    -cp ${LIB}/kafka-clients-3.3.1.jar:${LIB}/kafka-streams-3.3.1.jar \
    -d ${TRANSFORMER_PATH}/target/ \
    ${TRANSFORMER_PATH}/*.java
