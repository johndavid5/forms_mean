use forms_mean;
//db.forms.ensureIndex({"cik":1}, {"unique": true });
//db.forms.dropIndex({"cik":1}, {"unique": true });
// ensureIndex() is deprecated since MongoDb version 3.0.0
//db.forms.ensureIndex({"accession_number":1}, {"unique": true });
db.forms.createIndex({"accession_number":1}, {"unique": true });
