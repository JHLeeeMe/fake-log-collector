#!/bin/bash


cat <<EOF
# ----------------------- #
#   start-transformer.sh  #
# ----------------------- #
EOF

PROJECT_ROOT=/workspace
JAVA_ROOT=$PROJECT_ROOT/src/java
JARS=$JAVA_ROOT/jars

echo "exec Transformer"
java \
    -cp $JAVA_ROOT/target/:$JARS/kafka-clients-3.3.1.jar:$JARS/slf4j-api-1.7.36.jar:$JARS/kafka-streams-3.3.1.jar \
    Transformer
