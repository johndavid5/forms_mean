var fs = require('fs'); // Node's built-in file-system library...

var JUtils = require('./lib/jutils.js'); 
var Curl = require('node-curl/lib/Curl');

var FileMode = 0666; /* read-write */

var p = console.log;

for( var i=0; i < process.argv.length; i++ ){
	p("\t" + "argv[" + i + "] = '" + process.argv[i] + "'\n");
}
var url = process.argv[2];

var curl = new Curl();

if (!url){
    url = 'www.yahoo.com';
}

console.log("url = '" + url + "'...");

var baseName = JUtils.baseName( url );

console.log("baseName = '" + baseName + "'...");

console.log("Opening '" + baseName + "' for writing...");

var ourFd;
try {
	ourFd = fs.openSync( baseName, 'w', FileMode );
	console.log("ourFd = ", ourFd );
}
catch( err ){
	console.log("FATAL: Trouble opening file '" + baseName + "' for writing: '", err , "'");
	process.exit(1);
}

curl.setopt('URL', url);
curl.setopt('CONNECTTIMEOUT', 2);
curl.setopt('VERBOSE', 1);
//curl.setopt('RAW', 1); // TypeError: Cannot set property 'RAW' of undefined

// on 'data' must be returns chunk.length, or means interrupt the transfer
curl.on('data', function(chunk) {
    p("data: received " + chunk.length + " bytes...");
    p("data: chunk = " + chunk );
    p("data: Writing to fd: fs.writeSync( fd=" + ourFd + ", buffer=chunk, offset=0, length=" + chunk.length + ", position=null )...");
	//fs.writeSync(fd, buffer, offset, length, position)
	fs.writeSync( ourFd, chunk, 0, chunk.length, null );	
    return chunk.length;
});

curl.on('header', function(chunk) {
    p("header: received " + chunk.length + " bytes...");
    p("data: chunk = " + chunk );
    return chunk.length;
})

// curl.close() should be called in event 'error' and 'end' if the curl won't use any more.
// or the resource will not release until V8 garbage mark sweep.
curl.on('error', function(e) {
    p("error: " + e.message);
    curl.close();
	fs.closeSync( ourFd );
	process.exit(1);
});

curl.on('end', function() {
    p('end: code: ' + curl.getinfo('RESPONSE_CODE'));
    p('end: done.');
    curl.close();
	fs.closeSync( ourFd );
	process.exit(0);
});

curl.perform();

