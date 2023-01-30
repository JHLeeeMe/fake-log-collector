#!/bin/bash


cat <<EOF
# -------------------- #
#   start-producer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
PYTHON_ROOT=${PROJECT_ROOT}/src/python
LOG_GENERATOR_PATH=${PYTHON_ROOT}/log-generator

echo "pip installing..."
pip install -r ${LOG_GENERATOR_PATH}/requirements.txt

echo "exec main.py"
python ${LOG_GENERATOR_PATH}/main.py &
