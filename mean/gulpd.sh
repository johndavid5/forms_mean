# gulp js
# gulp watch:js
set -x
gulp dev 2>&1 | perl ./scripts/strip-colors.pl -tee gulpd.out | tee
set +x
