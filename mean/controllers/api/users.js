/** This controller has two actions:
* one to get an existing user, and the other
* to create a new user...
*/
var router = require('express').Router();
var bcrypt = require('bcrypt-nodejs');
var jwt = require('jwt-simple');
var User = require('../../models/user');
var config = require('../../config');



// Client passes x-auth=token in HTTP header
// and we authenticate it...
router.get('/', function(req, res, next){
	var sWho = "/api/users GET";

	console.log("Handling GET /user...");

	if( ! req.headers['x-auth']){
		console.log("WARNING: x-auth HTTP header element not supplied, returning code 401 (unauthorized)...");
		return res.sendStatus(401); // unauthorized
	}

	var token = req.headers['x-auth'];

	console.log("Received token = '" + token + "' in HTTP header...");

	// Hack: For some reason, client is passing extra double quote characters
	// on each end of the token, e.g, token = "\"segment1.segment2.segment3\""
	// so we trim it down to token = "segment1.segment2.segment3"
	// Update: Actually, we started using res.send(token) instead of res.json(token)
	// in /api/users POST, so we don't have extra quotes anymore...
	if( token.charAt(0) == "\"" && token.charAt(token.length-1) == "\"" ){
		token = token.substring(1, token.length-1);
		console.log("token seemed to have extra double quotes at beginning and end, trimmed them just now...");
	}

	console.log("token = '" + token + "', decoding now...via jwt.decode( token, \"" + config.secret + "\" )...");

	try {
		var auth = jwt.decode(token, config.secret);
	}catch( e ){
		console.log(sWho + "(): Exception during jwt.decode(): name = \"" + e.name + "\", message = \"" + e.message + "\"...");
		console.log(sWho + "(): Assuming bad token...returning code 401 (unauthorized)...");
		return res.sendStatus(401); // unauthorized
	}

	console.log("Token successfully decoded: auth = " + JSON.stringify(auth) );

	console.log("Looking up auth.username=\"" + auth.username + "\" in User DB...\n");

	User.findOne({"username": auth.username}, function(err, user){
		if( err ){
			console.log("ERROR during User.findOne(): err = " + JSON.stringify(err) + ", calling next(err)...");
			return next(err);
		}
		console.log("DB Lookup of auth.username = \"" + auth.username + "\" succeeded, returning user = " + JSON.stringify(user) + " to client...\n");
		res.json(user);
	});

});
	

// Create new user: client passes username and password for new user
// and you create a new user...add to the User database using hash
// of the password...
router.post('/', function(req, res, next){
	var user = new User({"username": req.body.username});

	var iNumRounds = 10;

	bcrypt.genSalt( iNumRounds, function(err, salt){ 
		if( err ){
			console.log("Trouble with bcrypt.genSalt( iNumRounds = " + iNumRounds + "): err = " + JSON.strinfigy(err) + "..., calling next(err)...");
			return next(err);
		}
		bcrypt.hash( req.body.password, salt, 
				function(){
					//console.log("Making progress, Doc-tor Cy-a-nide...!");
				},
				function(err, hash){
			if( err ){
				console.log("Trouble with bcrypt.hash(): err = " + JSON.stringify(err) + "..., calling next(err)...");
				return next(err);
			}

			var user = new User({"username": req.body.username});

			user.password = hash;

			console.log("Saving new user to User DB, user = " + JSON.stringify( user ) + "...\n");
			user.save(function(err, user){
				if( err ){
					console.log("Trouble with user.save(): err =\n");
					console.log( err );
					return next(err);
				}
				console.log("Saved new user username = \"" + req.body.username + "\"..., returning code 201...\n");
				res.sendStatus(201);
			});
		});
	});
});

module.exports = router;
