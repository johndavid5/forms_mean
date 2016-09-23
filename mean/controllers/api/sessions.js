var router = require('express').Router();
var User = require('../../models/user');
var bcrypt = require('bcrypt-nodejs');
var jwt = require('jwt-simple');
var config = require('../../config');

// Client feeds in username and password, you 
// authenticate and pass back a token if 
// successful...
router.post('/', function(req, res, next){
	var sWho = "api/sessions.js: post";

	console.log("Handling POST /session...");
	console.log("Got req.body.username = '" + req.body.username + "'...looking up in User...");

	User.findOne({"username": req.body.username})
		.select('password') // explicitly ask for password in result...
		.exec( function(err, user){

			if( err ){	
				console.log("ERROR during User.findOne(): err = " + JSON.stringify(err) + ",...returning next(err)...");
				return next(err);
			}

			if( ! user ){
				console.log("WARNING: Can't find username = \"" + req.body.username + "\" in User DB..., returning code 401 (unauthorized)...");
				return res.sendStatus(401);
			}

			console.log(sWho + "(): Found username = \"" + req.body.username + "\" in User DB...user (from DB) = " + JSON.stringify( user ) + ", now validating password asynchronously...");

			bcrypt.compare( req.body.password, user.password,
				function(err, valid){
					if( err ){
						console.log("ERROR during bcrypt.compare(): err = " + JSON.stringify( err ) + ", returning next(err)...\n");
						return next(err);
					}

					if( !valid ){
						console.log("WARNING: Password not valid, returning code 401 (unauthorized)...");
						return res.sendStatus(401); // unauthorized
					}

					var encodee = {"username": user.username};

					console.log(sWho + "(): Password is valid...generating token via jwt.encode( encodee, \"" + config.secret + "\" )...");

				   	console.log("encodee = ");
					console.log( encodee );

					try {
						var token = jwt.encode(encodee, config.secret );
					}
					catch(e){
						console.log("Exception during jwt.encode(): " + JSON.stringify( e ) + ", returning next(e)...");
						return next(e);
					}

					console.log("token = '" + token + "'...");
					console.log("JSON.stringify(token) = '" + JSON.stringify( token ) + "'...");

					// When we use res.json(token), the client winds up with
					// a double quote char on each side of their token,
					// so we'll use res.send(token) instead...just like
					// Dickey did in his code...

					//console.log("Returning token to user via res.json(token)...");
					//res.json(token);
					
					console.log("Returning token to user via res.send(token)...");
					res.send(token);

		});

	});
	
});

module.exports = router;
