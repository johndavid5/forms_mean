use forms_mean; 


print("before...")
db.forms.runCommand(
{
 "find": "forms",
 "filter": { "accession_number": { "$in" : ["0001193125-16-524271", "9999999997-16-022382"] } },
 //"projection": { "date_filed": 1, "accession_number": 1 },
 "sort": { "accession_number" : 1 },
 //"batchSize": 5,
 "limit": 5 
});


print("update()...")
db.forms.update({ "accession_number": { "$in" : ["0001193125-16-524271", "9999999997-16-022382"] } }, { "$set": { "dn_denormalized": false } }, { "multi": true } );


print("after...")
db.forms.runCommand(
{
 "find": "forms",
 "filter": { "accession_number": { "$in" : ["0001193125-16-524271", "9999999997-16-022382"] } },
 //"projection": { "date_filed": 1, "accession_number": 1 },
 "sort": { "accession_number" : 1 },
 //"batchSize": 5,
 "limit": 5 
});
