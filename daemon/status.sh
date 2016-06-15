#!/bin/bash
set -x
git status 2>&1 | tee status.txt
gvim status.txt &
set +x
