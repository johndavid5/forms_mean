# sec_forms
Download, load into MongoDB, and display financial forms info from SEC's EDGAR repository.

First the C++ sec_forms_daemon downloads form header information from ftp://ftp.sec.gov/edgar and
loads it into MongoDB via the curllib and mongoc and libbson libraries.

Next we create a searchable user interface in C++ using the Qt cross-platform toolkit.
