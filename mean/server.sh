# node server.js 2>&1 | tee server.out
set -x
nodemon server.js 2>&1 | tee server.out
set +x
