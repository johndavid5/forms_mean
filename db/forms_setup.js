use forms_mean;
// NOTE: ensureIndex() is deprecated.
//db.forms.ensureIndex({"cik":1}, {"unique": true });
//db.forms.dropIndex({"cik":1}, {"unique": true });
// ensureIndex() is deprecated since MongoDb version 3.0.0
//db.forms.ensureIndex({"accession_number":1}, {"unique": true });
db.forms.createIndex({"accession_number":1}, {"unique": true });
db.forms.createIndex({"date_filed":1}, {"unique": false });
db.forms.createIndex({"form_type":1}, {"unique": false });
db.forms.createIndex({"form_processing_attempts.success":1}, {"unique":false});
