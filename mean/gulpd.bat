REM gulp js
REM gulp watch:js
gulp dev 2>&1 | perl ./scripts/strip-colors.pl -tee gulpd.out | tee
