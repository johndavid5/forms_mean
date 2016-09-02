REM nmake /e /f sec_forms_daemon.mak 2>&1 | tee c.out
REM nmake /e /f sec_forms_daemon.mak 2>&1 | tee c.out
REM nmake /e /f mongoc.mak 2>&1 | tee c.out
REM nmake build/mongoc.exe /e /f sec_forms_daemon.mak 2>&1 | tee c.out
REM NOTE: name /a -- is like "rebuild all"
REM nmake /a /e /f sec_forms_daemon.mak 2>&1 | tee c.out
REM nmake build/ftpget.exe /e /f sec_forms_daemon.mak 2>&1 | tee c.out
REM nmake build/mongoc.exe /e /f sec_forms_daemon.mak 2>&1 | tee c.out
nmake /e /f sec_forms_daemon.mak 2>&1 | tee c.out
