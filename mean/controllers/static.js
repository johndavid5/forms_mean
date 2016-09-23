var express = require('express');
var router = express.Router();

// Get Express to automatically serve up static assets and templates...
router.use( express.static(__dirname + '/../assets') );
router.use( express.static(__dirname + '/../templates') );

// For default "/" serve up Single Page Application
// layout page ./layouts/app.html...
router.get('/', function(req, res){
	var sWho = "static.js router.get";

	var DEFAULT_PAGE = "layouts/app.html";

	console.log(sWho + "(): GET / received...serving up DEFAULT_PAGE = \"" + DEFAULT_PAGE + "\"...");

	// sendfile() is deprecated, use sendFile() instead...
	//res.sendfile('layouts/filings.html');

	// sendFile() requires you to specify absolute path
	// or root directory as second argument...
	res.sendFile(DEFAULT_PAGE, {root: __dirname + '/../'} );
});


module.exports = router;
