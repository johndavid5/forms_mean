use forms;
// db.forms_indexes.ensureIndex({"file_name": 1}, {"unique": true, "dropDups": true } ); 
//db.forms_indexes.ensureIndex({"file_name": 1}, {"unique": true, "dropDups": true } );
// NOTE: dropDups is deprecated ( and dangerous! )...
db.forms_indexes.ensureIndex({"file_name": 1}, {"unique": true } );
