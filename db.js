// Base Database Connection Logic
var mongoose = require('mongoose');
var config = require('./config');

//var db_url = 'mongodb://localhost/social';
var db_url = config.db_url;

console.log("db.js: Connecting to '" + db_url + "'...");

mongoose.connect( db_url, 
	function(){
		console.log("mongodb connected to '" + db_url + "'...");
	}
);
console.log("db.js: Done connecting to '" + db_url + "'...");

module.exports = mongoose;
