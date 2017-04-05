REM .\node_modules\.bin\mocha test/server/controllers/api/forms.spec.js 2>&1 | tee run-mocha.out
REM List reporters...
REM .\node_modules\.bin\mocha --reporters 
REM .\node_modules\.bin\mocha 2>&1 | tee run-mocha.out
REM Run mocha with Nyan Cat reporter...
REM .\node_modules\.bin\mocha -R nyan 2>&1 | tee run-mocha.out
.\node_modules\.bin\mocha -R nyan 2>&1 | perl ./scripts/strip-colors.pl -tee run-mocha.out | tee
