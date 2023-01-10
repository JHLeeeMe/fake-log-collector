#!/bin/bash


cat <<EOF
# -------------------- #
#   start-producer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
PYTHON_ROOT=${PROJECT_ROOT}/src/python
LOG_GENERATORS_ROOT=${PYTHON_ROOT}/log-generators

echo "pip installing..."
pip install -r ${LOG_GENERATORS_ROOT}/requirements.txt

echo "exec log_generator.py"
python ${LOG_GENERATORS_ROOT}/log_generator.py
