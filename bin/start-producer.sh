#!/bin/bash


cat <<EOF
# -------------------- #
#   start-producer.sh  #
# -------------------- #
EOF

PROJECT_ROOT=/workspace
PYTHON_ROOT=$PROJECT_ROOT/src/python

echo "pip installing..."
pip install -r $PYTHON_ROOT/log-generators/requirements.txt

echo "exec log_generator.py"
python $PYTHON_ROOT/log-generators/log_generator.py
