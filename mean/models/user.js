var db = require('../db'); // Mongoose-Mongo

var user = db.Schema({
	//username: String,
	// Need to explicitly say select: true,
	// or else we don't get username unless we 
	// chain .select('username') to findOne()...
	username: { type: String, required: true, select: true},

	// By default, do not return hashed password from query...
	// return it only if they ask for it...
	// "Security, Doc-tor Cy-a-nide...!"
	password: { type: String, required: true, select: false}
});

module.exports = db.model('User', user);
