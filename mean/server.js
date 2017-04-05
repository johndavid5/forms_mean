var express = require('express');
var bodyParser = require('body-parser');

var logger = require('./logger');
var Config = require('./config'); // config parameters in config.js

var sharedUtils = require('./ng/shared-utils.svc.js'); // stuff shared between Node Land and Angular Land...

var app = express();

var sWho = "server.js";

logger.setLevel('INFO');
//logger.setLevel('TRACE');
logger.info(sWho + ": logger.getLevel= " + logger.getLevel() + "...");

/** 
* @function bodyParser.json(options)
*
* Returns middleware that only parses json. This parser accepts any Unicode encoding of the body
* and supports automatic inflation of gzip and deflate encodings.
* 
* A new body object containing the parsed data is populated on the request object after
* the middleware (i.e. req.body).
*
* @reference: https://github.com/expressjs/body-parser
*/
app.use(bodyParser.json());

/* Attach our "auth" middleware to the servers...
* It will automagically attach an "auth" object
* to the request (req) object...if it finds
* a valid x-auth in the HTTP request header...
* oh, yeah, baby!!
*/
app.use(require('./auth'));

/* Attach our custom patented "no-cache" middleware to the servers...
* End the sado-masochism of caching NOW...!
*/
app.use(require('./no-cache'));

// Mount the controllers, Escamillo...
app.use('/api/forms', require('./controllers/api/forms'));
// And mount for possible reverse-proxy prefix as well...
if( sharedUtils.getUrlPrefix().length > 0 ){
	app.use(sharedUtils.getUrlPrefix() + '/api/forms', require('./controllers/api/forms'));
}

app.use('/api/formTypes', require('./controllers/api/formTypes'));
if( sharedUtils.getUrlPrefix().length > 0 ){
	app.use(sharedUtils.getUrlPrefix() + '/api/formTypes', require('./controllers/api/formTypes'));
}

//app.use('/api/sessions', require('./controllers/api/sessions'));
//app.use('/api/users', require('./controllers/api/users'));

// And mount the static controller for static content...
// In case we're coming in directly or via reverse proxy via /...
app.use('/', require('./controllers/static'));

if( sharedUtils.getUrlPrefix().length > 0 ){
	app.use(sharedUtils.getUrlPrefix() + "/", require('./controllers/static'));
}
// In case we're coming in for static content via a
// reverse proxy via /sec_forms...
// app.use('/sec_forms', require('./controllers/static'));

var port = Config.port;

app.listen( port, function(){
	console.log("Server listening on port ", port, ", Mr. Bond..." );
});

console.log("Let off some steam, Bennett!");
