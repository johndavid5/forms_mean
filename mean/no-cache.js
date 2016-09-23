/**
* Middleware that will automagically attach no-cache headers 
* to the response to prevent browsers from caching any page.
*/
var config = require('./config');

/*
* http://stackoverflow.com/questions/11474345/force-browser-to-refresh-css-javascript-etc
*
* header("Expires: Tue, 01 Jan 2000 00:00:00 GMT");
* header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
* header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
* header("Cache-Control: post-check=0, pre-check=0", false);
* header("Pragma: no-cache");
*/

module.exports = function(req, res, next){
	if( typeof config.noCache == "undefined" || config.noCache === true ){
		//console.log("./no-cache: Adding no-cache headers to response...");
		res.header({
			"Expires": "Thu, 01 Jan 1970 00:00:00 GMT",
			"Cache-Control": "no-store, no-cache, must-revalidate, max-age=0",
			//"Cache-Control" : "post-check=0, pre-check=0",
			"Pragma": "no-cache"
		});
	}
	next();
};


//console.log("./no-cache, I am here...!");
