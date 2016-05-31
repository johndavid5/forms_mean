REM .\build\forms_mean_daemon.exe --manual-index-process-url "ftp://ftp.sec.gov/edgar/daily-index/master.20160101.idx" --no-daemon 2>&1 | tee d.out
REM .\build\forms_mean_daemon.exe --manual-index-process-url "file:///../edgar/daily-index/master.20141031.sample.idx" --no-daemon 2>&1 | tee d.out
.\build\mongoc.exe find -uri "mongodb://127.0.0.1/" -db "test" -collection "grades" -json "{ \"student_id\" : 2 }" 2>&1 | tee d.out
