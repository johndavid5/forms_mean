# sec_forms
Download, load into MongoDB, and display financial forms info from SEC's EDGAR repository.

* ./daemon folder: The C++ sec_forms_daemon -- see the ./daemon folder -- downloads form header information
  from ftp://ftp.sec.gov/edgar and loads it into MongoDB via the curllib and mongoc and libbson libraries.

* ./mean folder: The MEAN stack (MongoDB, Express.js, Angular.js, Node.js) web interface 
  to the sec_forms...search filters, sorting, paginating, CSV-download, all that good
  stuff, plus a forms-details page for starters...

* ./gui folder: If time permits, a desktop GUI interface built in C++ using the Qt cross-platform toolkit. 
