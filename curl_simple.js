curl = require('node-curl');

var url = process.argv[2];

console.log("Getting '" + url + "'...");

curl(url, {VERBOSE: 1, RAW: 1}, function(err) {

	if( err ){
		console.log("*** err = ", err );
	}

    console.log("info...");
	console.info(this);

    console.log("status = ", this.status);
    console.log('-----');
    console.log("body = ", this.body);
    console.log('-----');
    console.log("SIZE_DOWNLOAD = ", this.info('SIZE_DOWNLOAD'));

  });

