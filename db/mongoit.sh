#!/bin/bash
set -x
# mongo diana/forms_mean < $1 2>&1 | tee $1.out
mongo localhost/forms_mean < $1 2>&1 | tee $1.out
set +x
