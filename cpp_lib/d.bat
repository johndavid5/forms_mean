REM .\build\ftpget.exe http://cnn.com 2>&1 | tee d.out
REM .\build\mongoc.exe find -uri "mongodb://127.0.0.1/" -db "forms_mean" -collection "forms" -query "{ \"accession_number\" : \"0001242648-14-000082\" }" 2>&1 | tee d.out
REM .\build\mongoc.exe find -uri "mongodb://127.0.0.1/" -db "forms_mean" -collection "forms" -query "{ }" 2>&1 | tee d.out
REM .\build\mongoc.exe insert -uri "mongodb://127.0.0.1/" -db "forms_mean" -collection "forms" -query "{ \"accession_number\" : \"0001242648-14-000085\" }" 2>&1 | tee d.out
REM .\build\mongoc.exe find -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -query "{ \"accession_number\" : \"0001242648-14-000081\" }" 2>&1 | tee d.out
REM c:\cygwin\bin\echo "{ \"find\": \"forms\", \"filter\": { \"accession_number\": \"001242648-14-000082\" } }" 2>&1 | tee d.out
REM c:\cygwin\bin\echo "{ \"find\": \"forms\", \"filter\": { \"accession_number\": \"0001193125-14-390153\"} }" | .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command - 2>&1 | tee d.out
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command "{ \"find\": \"forms\", \"filter\": { \"accession_number\": \"0001193125-14-390153\"} }" 2>&1 | tee d.out
REM > db.forms.runCommand({ find: "forms", filter: {}, limit: 3 });
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command "{ \"find\": \"forms\", \"limit\": 3 }" 2>&1 | tee d.out
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command "{ \"insert\": \"forms\", \"documents\": [ { \"name\": \"Larry\", \"accession_number\": 2, \"faux\": true } ] }" 2>&1 | tee d.out
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command "{ \"find\": \"forms\", \"filter\": { \"name\": \"Moe\" } }" 2>&1 | tee d.out
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command - < view-1.js  2>&1 | tee d.out
REM .\build\mongoc.exe command -uri "mongodb://prnvf-jayne/" -db "forms_mean" -collection "forms" -command - < view-2.js  2>&1 | tee d.out
