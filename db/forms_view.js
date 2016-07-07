use forms_mean; 

var joe = { name: "Joe" };
//print( "joe:" );
//printjson( joe );
//print("getIndexes()...");
//db.forms.getIndexes();

print( "=== first one, Moe..." );
db.forms.find().limit(1);
print( "=== accession_number = 0001242648-14-000081, Moe...");
db.forms.find( { "accession_number" : "0001242648-14-000081" } );
