#!/bin/bash
set -x
mongo jayne/forms_mean < $1 2>&1 | tee $1.out
set +x
