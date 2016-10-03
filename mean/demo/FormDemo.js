var logger = require('../logger');

var Form = require('../models/Form');
var ourForm = new Form();

var sWho = "FormTest";

//logger.setLevel('INFO');
logger.setLevel('TRACE');
console.log(sWho + "(): logger.getLevel= " + logger.getLevel() + "...");

var optionsArray = [
	{},
	{accessionNumber: "0001652577-15-000012"}
];

var optionsArrayIndex = 0;

function doNextTest(){

	var sWho = "doNextTest";

	console.log(sWho + "(): optionsArrayIndex = " + optionsArrayIndex + "...");

	if( optionsArrayIndex > optionsArray.length-1 ){	
		console.log(sWho + "(): SHEMP: Moe, optionsArrayIndex  = " + optionsArrayIndex + ", is greater than max index, exiting the test loop now...");
		console.info(sWho + "(): Let off some steam, Bennett!");
		return;
	}

	console.log(sWho + "(): optionsArrayIndex = " + optionsArrayIndex + ": Running ourForm.getForms(", optionsArray[optionsArrayIndex], ")..."); 

	ourForm.getForms( optionsArray[optionsArrayIndex], function( rows, rowCount, err ){
		console.info(sWho + "(): err = ", err, "...");
		console.info(sWho + "(): rowCount = ", rowCount, "...");
		console.info(sWho + "(): rows = ", rows, "...");

		optionsArrayIndex++;
		doNextTest();
	});

}/* doNextTest() */

doNextTest();

