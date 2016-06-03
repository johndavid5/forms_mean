use forms_mean;
//db.forms.ensureIndex({"cik":1}, {"unique": true });
//db.forms.dropIndex({"cik":1}, {"unique": true });
db.forms.ensureIndex({"accession_number":1}, {"unique": true });
