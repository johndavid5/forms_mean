var fs = require('fs'); // File System
var nodeFs = require('node-fs'); // File System - Extended Stuff
var FtpClient = require('ftp');

var file_name = 'edgar/data/1494316/0001544824-14-000029.txt';

//var ftp_host = "ftp.sec.gov@proxy.prnewswire.com";
//var ftp_host = "ftp.sec.gov";
var ftp_host = "proxy.prnewswire.com";
var ftp_port = 8080;
var ftp_user = "anonymous@ftp.sec.gov";
var ftp_password = "johndavid5@yahoo.com";

				var baseDir = file_name;

				// e.g., formsIndexes[i].file_name: 'edgar/data/1494316/0001544824-14-000029.txt'
				// But first make sure the directory exists... 
				// TODO: Use a more sophisticated way to determine the baseDir...a node module,
				// or part of your "utils"...maybe lo-dash...?
				var iWhere = file_name.lastIndexOf("/");

				if( iWhere >= 0 ){
					baseDir = file_name.substring(0, iWhere);
				}

				console.log("Calling nodeFs.mkdirSync( '" + baseDir + "' )...");
				// node-fs: mkdirSync(path, mode, [recursive]):
				nodeFs.mkdirSync( baseDir, 0777, true );


				var ftpClient = new FtpClient();

				var connect_options = {"host": ftp_host, "port": ftp_port, "user": ftp_user, "password": ftp_password };

				ftpClient.on('ready', function(){

					console.log("*** ftpClient.get( '" + file_name + "' )...");

					ftpClient.get( file_name, function(err, stream){
						if(err){ console.log("Error with download of '" + file_name + "': ", err ); }
						stream.once('close', function(){ ftpClient.end(); });
						console.log("*** stream.pipe(fs.createWriteStream( '" + file_name + "' )...");
						stream.pipe(fs.createWriteStream( file_name ) );
					});
				});

				ftpClient.on('greeting', function(msg){

					console.log("*** on('greeting'): msg = '" + msg + "'...");

				});

				ftpClient.on('error', function(err){
					console.log("*** ftpClient.on('error'): err = ", err );
				});

				console.log("*** ftpClient.connect( ", connect_options, " )...");
				ftpClient.connect( connect_options );
