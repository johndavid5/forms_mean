// client.js
var ftp = require('ftp-get')

var url = process.argv[2];
var local_file = process.argv[3];

console.log("Getting '" + url + "' to local file '" + local_file + "'...");

ftp.get(url, local_file, function (err, res) {
    console.log(err, res);
})

console.log("Done!");
