var deepcopy = require('deepcopy');

var logger = require('../../logger');
var sharedUtils = require('../../ng/shared-utils.svc.js');

var Form = require('../../models/form');
var ourForm = new Form();

var CsvInator = require('../../helpers/CsvInator');
var FrontMatterMapper = require('../../helpers/FrontMatterMapper');

var router = require('express').Router();

router.get('/:accessionNumber?', function(req, res, next){

	var sWho = "forms: router.get";

	console.log(sWho + '(): GET /api/forms received...');

	//console.log(sWho + "():\n" +
    //"<req> ********* here goes nuttin'...\n" +
	//"req = ", req, "........\n" +
	//"</req> ********* there went nuttin'...\n"  );

	console.log(sWho + "(): req.params.accessionNumber = ", req.params.accessionNumber, "..."); 

	console.log(sWho + "(): BEFORE: req.query = ", req.query );

	if( typeof req.params.accessionNumber != "undefined" ){
		logger.info(sWho + "(): Setting req.query.accessionNumber equal to req.params.accessionNumber...");
		req.query.accessionNumber = req.params.accessionNumber;
	}

	console.log(sWho + "(): AFTER: req.query = ", req.query );

	if( req.query.format == "csv" ){

		console.log(sWho + "(): Looks like a CSV request, Sir...");

		var filename = "forms-report.csv";
		res.setHeader("Content-Type", "text/csv");
		res.setHeader('Content-disposition', 'attachment; filename=' + filename);

		req.query.getter = function(options,callback){ ourForm.getForms(options,callback); };
		req.query.csv = {};
		//req.query.csv.fieldsPretty = ["Accession Number", "Form Type", "Date Filed", "Subject Company", "Subject Company CIK", "Filing Agent", "Filing Agent CIK"];
		req.query.csv.fieldsPretty = ["Accession Number", "Form Type", "Date Filed", "Filer Name", "Filer CIK"];
		//req.query.csv.fieldsActual = ["accession_number", "form_type", "date_filed", "subject_company_entity_name_varchar", "subject_company_cik_bigint", "filing_agent_entity_name_varchar", "filing_agent_cik_bigint"];
		req.query.csv.fieldsActual = ["accession_number", "form_type", "date_filed", "dn_company_conformed_name", "dn_company_central_index_key"];
		req.query.csv.fieldTransforms = [
			//{"field": "date_filed", "transform": function(row){ return sharedUtils.formatDateObjectAsString(row.date_filed) } },
			// TypeError: Object 2016-03-31 has no method 'getYear'
    		// at Object.SharedUtils.formatDateObjectAsString (c:\inetpub\wwwroot\sec_forms\mean\ng\shared-utils.svc.js:186:48)
			{"field": "dn_company_conformed_name", "transform": function(row){ return (row.dn_company_conformed_name ? row.dn_company_conformed_name : ""); } }, // Display "" instead of "null"
			{"field": "dn_company_central_index_key", "transform": function(row){ return (row.dn_company_central_index_key ? row.dn_company_central_index_key : ""); } }, // Display "" instead of "null"
		];

		req.query.frontMatter = [];
		req.query.frontMatter.push("Forms Report");

		//var sHost = req.headers.host;
		////var sUrl = req.url;
		//var sUrl = req.originalUrl;
		//var sLink = "http://" + req.headers.host + sUrl;
		//console.log(sWho + "(): How to Form a Web Link Without Really Trying, Moe...\n" +
		//"\t" + "sHost = req.headers.host = '" + sHost + "',\n" +
		//"\t" + "sUrl = req.originalUrl = '" + sUrl + "',\n" +
		//"\t" + "sLink = '" + sLink + "'...\n"
		//);
		//
		//req.query.frontMatter.push("Web Link: " + sLink );

		FrontMatterMapper.mapItUp( req.query );

//		var frontMatterMap = [ 
//			{"field": "entities_id_filter", "field_pretty": "Entities ID" },
//			{"field": "accession_number_filter", "field_pretty": "Accession Number" },
//			{"field": "form_type_filter", "field_pretty": "Form Type" },
//			{"field": "date_filed_from_filter", "field_pretty": "From Date" },
//			{"field": "date_filed_to_filter", "field_pretty": "To Date" },
//			{"field": "filing_agent_entity_name_varchar_filter", "field_pretty": "Filing Agent" },
//			{"field": "filing_agent_cik_bigint_filter", "field_pretty": "Filing Agent CIK" },
//			{"field": "entity_cik_filter", "field_pretty": "Entity CIK" },
//			{"field": "orderBy", "field_pretty": "Order By",
//				"field_transform": function(field, query){
//					field += " " + query.ascDesc;
//					field = field.replace("_", " ");
//					field = sharedUtils.ucFirstAllWords(field);
//					return field;
//				}
//			},
//		];
//
//		for( var i = 0; i < frontMatterMap.length; i++ ){
//			var field_key = frontMatterMap[i].field;
//			var field_pretty = frontMatterMap[i].field_pretty;
//			var field_transform = frontMatterMap[i].field_transform;
//			var field_value = req.query[ field_key ];
//			if( field_value ){
//				if( field_transform ){ 
//					req.query.frontMatter.push( field_pretty + ": " + field_transform( field_value, req.query ) );
//				}
//				else {
//					req.query.frontMatter.push( field_pretty + ": " + field_value );
//				}
//			}
//		}

		// var filestream = fs.createReadStream(file);
		// filestream.pipe(res);

		//logger.setLevel('DEBUG');
		var readStream = new CsvInator(req.query); 
		console.log(sWho + "(): readStream = ", readStream, "...");
		console.log(sWho + "(): Calling readStream.pipe(res)...");

		readStream.pipe(res);
		//logger.setLevel('INFO');
	}
	else {

		ourForm.getForms( req.query, function(jsonOutput, rowsAffected, err ){

			if(jsonOutput instanceof Array){ 		
				console.log( sWho + "(): jsonOutput.length = " + jsonOutput.length );
			}
			console.log(sWho + "(): rowsAffected = " + rowsAffected );
			console.log(sWho + "(): err = ", err, "..." );
	
			if(err){
				console.log(sWho + "(): Calling return next(err)...");
				return next(err);
			}
	
			//console.log("Sending to client: jsonOutput = ", jsonOutput, "..." );

			console.log(sWho + "(): Sending jsonOutput to client...");
	
			res.header({
				"Access-Control-Allow-Origin": "*"
			});

			//res.json([
			//	{ accession_number: "0001214659-15-003094", form_type: "10-Z", date_filed: "2015-04-16"  }
			//]);

			res.json( jsonOutput );
	
			console.log("Done!");
		});
	}


}); /* get() */





router.post('/', function(req, res, next){

	var sWho = "forms.js::router.post";
	var sOuterWho = "forms.js::router.post";

	console.log(sWho + '(): POST /api/forms received!');

	console.log(sWho + "(): req.body=", req.body);

	// req.auth automagically filled in
	// via auth.js middleware...
	console.log(sWho + "(): req.auth=", req.auth );

	var bAuthRequired = false;

	// If authorization is required and they're not logged in, throw them out on their keisters...
	if( bAuthRequired && ! req.auth ){
		var message = "Login required to post a filing.";
		console.log("Doesn't appear to be logged in, so send message \"" + message + "\" along with response code 401 (unauthorized)...");
		return res.status(401).send("Login required to post a filing.");
	}

	var options = deepcopy( req.body );

	if( req.auth && req.auth.username ){
		options.source_modified_varchar = req.auth.username.trim();
	}

	//console.log("req =");
	//console.log( req );

	// var newFiling = new Filing({...});
	// newFiling.username = req.auth.username; 
	// newFiling.save(function(err, post){
	//   if(err){ return next(err); }
    //   res.status(201).json(newFiling)
	// });

	if( req.body.action == "associatePeopleWithFiling" ){

		ourFiling.associatePeopleWithFiling( req.body, function(jsonOutput, rowsAffected, err ){

			var sWho = sOuterWho + "::associatePeopleWithFiling";

			if( jsonOutput instanceof Array ){
				console.log( sWho + "(): jsonOutput.length = " + jsonOutput.length );
			}

			console.log(sWho + "(): jsonOutput =", jsonOutput );
	
			console.log(sWho + "(): rowsAffected = " + rowsAffected );
	
			if(err){
				logger.error(sWho + "(): err = ", err, "...calling return next(err)...");
				return next(err);
			}
	
			console.log("Sending res.json(jsonOutput)...");
	
			res.header({
				"Access-Control-Allow-Origin": "*"
			});
			res.json( jsonOutput );
	
			console.log(sWho + "(): Done!");

		});/* ourFiling.associatePeopleWithFiling() */

	}
	else if( req.body.action == "disAssociatePeopleWithFiling" ){

		ourFiling.disAssociatePeopleWithFiling( req.body, function(jsonOutput, rowsAffected, err ){

			var sWho = sOuterWho + "::disAssociatePeopleWithFiling";

			if( jsonOutput instanceof Array){
				console.log( sWho + "(): jsonOutput is an Array with jsonOutput.length = " + jsonOutput.length );
			}

			console.log(sWho + "(): jsonOutput =", jsonOutput );
	
			console.log(sWho + "(): rowsAffected = " + rowsAffected );
	
			if(err){
				logger.error(sWho + "(): err = ", err, "...calling return next(err)...");
				return next(err);
			}
	
			console.log("Sending res.json(jsonOutput)...");
	
			res.header({
				"Access-Control-Allow-Origin": "*"
			});
			res.json( jsonOutput );
	
			console.log(sWho + "(): Done!");

		});/* ourFiling.disAssociatePeopleWithFiling() */

	}
	else if( req.body.action == "addFiling" ){

		var newFiling = {
			"accession_number": req.body.accession_number,
			"form_type": req.body.form_type,
			"date_filed": req.body.date_filed,
			"username": req.auth.username
		};
	
		console.log(sWho + "(): ourFiling.putFiling( newFiling = " + JSON.stringify( newFiling ) + "...");
	
		ourFiling.putFiling( newFiling, function(jsonOutput, rowsAffected, err ){
	
			//console.log(sWho + "(): jsonOutput =");
			//console.log( jsonOutput );
			
			console.log( sWho + "(): jsonOutput.length = " + jsonOutput.length );
	
			console.log(sWho + "(): rowsAffected = " + rowsAffected );
	
			console.log(sWho + "(): err =", err, "..." );
	
			if(err){
				console.log(sWho + "(): Calling return next(err)...");
				return next(err);
			}
	
			console.log("Sending res.json(newFiling)...");
	
			res.header({
				"Access-Control-Allow-Origin": "*"
			});
			res.json( newFiling );
	
			console.log("Done!");
		});/* ourFiling.putFiling() */


	} /* if( req.body.action == "addFiling" ) */

	//res.send(201);
	// express deprecated res.send(status): Use res.sendStatus(status) instead server.js:33:6
	//res.sendStatus(201);

	// express deprecated res.send(body, status): Use res.status(status).send(body) instead server.js:36:6
	//res.send("Let off some steam, Bennett!", 201);
	//res.status(201).send("<html><body><p>Let off some steam, Bennett!</p></body></html>");

	//console.log(sWho + "(): Done.");
});

module.exports = router;
