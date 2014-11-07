set -x
node curl.js ftp://ftp.sec.gov/edgar/data/1012037/0001012037-14-000001.txt 2>&1 | tee curl.out
#node curl.js http://www.cnn.com/ 2>&1 | tee curl.out
#curl -v ftp://ftp.sec.gov/edgar/data/1012037/0001012037-14-000001.txt 2>&1 | tee curl.out
set +x
