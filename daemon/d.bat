REM .\build\sec_forms_daemon.exe --manual-index-process-url "ftp://ftp.sec.gov/edgar/daily-index/master.20160101.idx" --no-daemon 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-index-process-url "file:///../edgar/daily-index/master.20141031.sample.idx" --no-daemon 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-form-process-url "file:///../edgar/data/1000180/0001242648-14-000081.txt" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-form-process-url "ftp://ftp.sec.gov/edgar/data/1000180/0001242648-14-000081.txt" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-form-process-url "file:///../edgar/ftp.sec.gov--edgar--data--1066107--POSASR--has-a-gazillion-filers--0001047469-15-001027.txt" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-form-process-url "file:///../edgar/ftp.sec.gov--edgar--data--1000180--SANDISK--issuer--reporting-owner--0001242648-14-000081.txt" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-form-process-url "file:///../edgar/ftp.sec.gov--edgar--data--99359--breeze-eastern-corp--subject-company--filed-by--0001193125-15-399601.txt" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-load-next-edgar-form 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe --manual-index-process-url "file:///../edgar/ftp.sec.gov--edgar--full-index--2016--QTR1--master.idx" 2>&1 | tee d.out
REM .\build\sec_forms_daemon.exe 2>&1 | tee d.out
REM "WESTERN INTERNATIONAL SECURITIES, INC."
.\build\sec_forms_daemon.exe --manual-denormalize-form-accession-number "9999999997-16-022382" 2>&1 | tee d.out
