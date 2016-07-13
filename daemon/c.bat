REM nmake /e /f forms_mean_daemon.mak 2>&1 | tee c.out
REM nmake /e /f forms_mean_daemon.mak 2>&1 | tee c.out
REM nmake /e /f mongoc.mak 2>&1 | tee c.out
REM nmake build/mongoc.exe /e /f forms_mean_daemon.mak 2>&1 | tee c.out
REM NOTE: name /a -- is like "rebuild all"
nmake /a /e /f forms_mean_daemon.mak 2>&1 | tee c.out
REM nmake build/ftpget.exe /e /f forms_mean_daemon.mak 2>&1 | tee c.out
