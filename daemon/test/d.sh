#!/bin/bash
set -x
./build/example-client -uri "mongodb://jayne/" -db "test" -collection "grades" -json "{ \"student_id\" : 2 }" 2>&1 | tee d.out
set +x
