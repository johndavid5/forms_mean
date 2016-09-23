var logger = require('../logger');

var Form = require('../models/Form');
var ourForm = new Form();

var sWho = "FormTypeTest";

//logger.setLevel('INFO');
logger.setLevel('TRACE');
console.log(sWho + "(): logger.getLevel= " + logger.getLevel() + "...");

console.log(sWho + "(): Running ourForm.getFormTypes()..."); 

ourForm.getFormTypes( {}, function( rows, rowCount, err ){

	console.info(sWho + "(): err = ", err, "...");
	console.info(sWho + "(): rowCount = ", rowCount, "...");
	console.info(sWho + "(): rows = ", rows, "...");
	console.info(sWho + "(): Let off some steam, Bennett!");

});

