use forms_mean; 

var joe = { name: "Joe" };
//print( "joe:" );
//printjson( joe );
//print("getIndexes()...");
//db.forms.getIndexes();

//print( "=== first one, Moe..." );
//db.forms.find().limit(1);
// format: { field: { $in: [<value1>, <value2>, ... <valueN> ] } }
// example: db.inventory.find( { qty: { $in: [ 5, 15 ] } } )
//db.forms.find( { accession_number : { $in: ["0001242648-14-000081","0001127602-14-030663"] } } );
//db.forms.find( { "accession_number" : "0001242648-14-000081" } );

//var query = { accession_number : "" };
//for( var i = 0 ; i < accession_numbers.length; i++ ){
//	query.accession_number = accession_numbers[i];
//	print( "=== " + (i+1) + "/" + accession_numbers.length + ": Runnin' find(" );
//	printjson( query )
//	print("), Moe..." );
//	db.forms.find( query );
//}

//db.forms.update( { accession_number :  "0001127602-14-030663" }, { "index_loaded_date": { $date: 2000 } } );
//db.forms.update( { "_id": ObjectId("57634111b3d97718975568b4") }, { $set: { accession_number :  "0001127602-14-030663", "index_loaded_date": ISODate("2016-07-07T00:00:01Z") } } );
//db.forms.update( { accession_number :  "0001127602-14-030663" }, { $set: { "index_loaded_date": ISODate("2016-07-07T00:00:01Z") } } );
//db.forms.update( { accession_number :  "0001127602-14-030663" }, { $set: { "index_loaded_date": new ISODate() } } );
//db.forms.update( { accession_number :  "0001127602-14-030663" }, { $set: { "index_loaded_date": { $date : 2000 } } } );
db.forms.insert( { accession_number :  "0001047469-15-001027" } );
db.forms.insert( { accession_number :  "0001193125-15-399601" } );

var accession_numbers = ["0001242648-14-000081","0001127602-14-030663","0001047469-15-001027", "0001193125-15-399601" ];
var query = { accession_number : { $in: accession_numbers } };
print( "=== " + "Runnin' find(..." );
printjson( query )
print("...), Moe..." );
db.forms.find( query ).sort( { form_processing_attempts: -1 } ).pretty();
