REM .\build\forms_mean_daemon.exe --manual-index-process-url "ftp://ftp.sec.gov/edgar/daily-index/master.20160101.idx" --no-daemon 2>&1 | tee d.out
REM .\build\forms_mean_daemon.exe --manual-index-process-url "file:///../edgar/daily-index/master.20141031.sample.idx" --no-daemon 2>&1 | tee d.out
REM .\build\mongoc.exe find -uri "mongodb://127.0.0.1/" -db "forms_mean" -collection "forms" -query "{ \"accession_number\" : \"0001242648-14-000082\" }" 2>&1 | tee d.out
.\build\mongoc.exe find -uri "mongodb://127.0.0.1/" -db "forms_mean" -collection "forms" -query "{ }" 2>&1 | tee d.out
