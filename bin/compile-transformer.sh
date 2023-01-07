#!/bin/bash


cat <<EOF
# -------------------------- #
#   compile-transformer.sh   #
# -------------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=$PROJECT_ROOT/src/java
JARS=$JAVA_ROOT/jars

if [[ -d $JAVA_ROOT/target/ ]]; then
    rm -rf $JAVA_ROOT/target
fi

echo "compile Transformer.java & TransformerTopology.java"
javac \
    -cp $JARS/kafka-clients-3.3.1.jar:$JARS/kafka-streams-3.3.1.jar \
    -d $JAVA_ROOT/target/ \
    $JAVA_ROOT/*.java
