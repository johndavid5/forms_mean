set -x
git status . 2>&1 | tee status.txt
vim status.txt
set +x
