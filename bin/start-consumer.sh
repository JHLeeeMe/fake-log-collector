#!/bin/bash


cat <<EOF
# -------------------- #
#   start-consumer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=$PROJECT_ROOT/src/java
JARS=$JAVA_ROOT/jars

if [[ ! -d ./src/java/target/ ]]; then
    echo "compile TestConsumer.java"
    javac \
        -cp $JAVA_ROOT/jars/kafka-clients-3.3.1.jar \
        -d $JAVA_ROOT/target/ \
        $JAVA_ROOT/TestConsumer.java
fi

echo "exec TestConsumer"
java \
    -cp $JAVA_ROOT/target/:$JARS/kafka-clients-3.3.1.jar:$JARS/slf4j-api-1.7.36.jar \
    TestConsumer
