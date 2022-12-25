#!/bin/bash


cat <<EOF
# -------------------- #
#   start-consumer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=$PROJECT_ROOT/src/java
JARS=$JAVA_ROOT/jars

if [[ -d $PROJECT_ROOT/src/java/target/ ]]; then
    rm -rf $JAVA_ROOT/target
fi

echo "compile Transformer.java & TransformerTopology.java"
javac \
    -cp $JARS/kafka-clients-3.3.1.jar:$JARS/kafka-streams-3.3.1.jar \
    -d $JAVA_ROOT/target/ \
    $JAVA_ROOT/*.java

echo "exec Transformer"
java \
    -cp $JAVA_ROOT/target/:$JARS/kafka-clients-3.3.1.jar:$JARS/slf4j-api-1.7.36.jar:$JARS/kafka-streams-3.3.1.jar \
    Transformer
