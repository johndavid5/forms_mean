var deepcopy = require('deepcopy');

var logger = require('../../logger');

var Form = require('../../models/form');
var ourForm = new Form();

var router = require('express').Router();


router.get('/', function(req, res, next){

	var sWho = "formTypes: router.get";

	console.log(sWho + '(): GET /api/formTypes received...');

	console.log(sWho + "(): req.query = ", req.query );

	//
	//e.g., res.json([
	//	{ form_type_varchar: "10-K" },
	//	{ form_type_varchar: "10-Q" },
	//	{ form_type_varchar: "10-Z" }
	//]);
	//
	// ...or if you prefer sweet simplicity...
	//
	//e.g., res.json([
	//	"10-K",
	//	"10-Q",
	//	"10-Z"
	//]);
	//

	console.log(sWho + "(): Calling ourForm.getFormTypes( req.query )..." );

	ourForm.getFormTypes( req.query, function(jsonOutput, rowsAffected, err ){

		//console.log("Sending jsonOutput = ", jsonOutput, "..." );
		
		console.log(sWho + "(): rowsAffected = " + rowsAffected );
		console.log(sWho + "(): err = ", err, "..." );
		if(jsonOutput instanceof Array){ 		
			console.log( sWho + "(): jsonOutput.length = " + jsonOutput.length );
		}
		for( var i = 0; i <= 2 && i < jsonOutput.length; i++ ){
			console.log( sWho + "(): jsonOutput[" + i + "] = ", jsonOutput[i], "...");
		}

		//if(err){
		//	console.log(sWho + "(): Calling return next(err)...");
		//	return next(err);
		//}

		console.log(sWho + "(): Sending jsonOutput to client...");

		res.header({
			"Access-Control-Allow-Origin": "*"
		});
		res.json( jsonOutput );

		console.log("Done!");
	});

});

module.exports = router;
