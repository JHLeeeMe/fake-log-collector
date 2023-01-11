#!/bin/bash


cat <<EOF
# -------------------------- #
#   compile-transformer.sh   #
# -------------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=${PROJECT_ROOT}/src/java
LIB=${JAVA_ROOT}/lib

if [[ -d ${JAVA_ROOT}/target/ ]]; then
    rm -rf ${JAVA_ROOT}/target
fi

echo "compile Transformer.java & TransformerTopology.java"
javac \
    -cp ${LIB}/kafka-clients-3.3.1.jar:${LIB}/kafka-streams-3.3.1.jar \
    -d ${JAVA_ROOT}/target/ \
    ${JAVA_ROOT}/*.java
