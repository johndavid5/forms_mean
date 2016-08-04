REM nmake /d /e /f tests.mak 2>&1 | tee c.out
REM cl /EHsc joe.cpp 2>&1 | tee c.out
REM cl /EHsc union.cpp 2>&1 | tee c.out
REM cl /EHsc find.cpp 2>&1 | tee c.out
nmake /e /f tests.mak 2>&1 | tee c.out
