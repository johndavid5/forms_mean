var fs = require('fs'); // File System
var LineByLineReader = require('line-by-line');
var nodeFs = require('node-fs'); // File System - Extended Stuff
var FtpClient = require('ftp');

var config = require('./config');
var FormsIndexModel = require('./models/forms_index.js');

var daily_index_path = "./edgar/daily-index/master.20141031.idx";

//var ftp_host = "ftp.sec.gov@proxy.prnewswire.com";
//var ftp_host = "ftp.sec.gov";
var ftp_host = "proxy.prnewswire.com";
var ftp_port = 8080;
var ftp_user = "anonymous@ftp.sec.gov";
var ftp_password = "johndavid5@yahoo.com";

//loadDailyIndex( daily_index_path );

downloadAForm();


function downloadAForm(){

	var sWho = "downloadAForm";

	console.log(sWho + "()...");

	FormsIndexModel.find()	
	//.sort('-date')
	.exec(function(err, formsIndexes){ 
		if(err){
			console.log("Error during FormsIndexModel.find(): err = ", err );
		}

		console.log("formsIndexes.length = " + formsIndexes.length );

		for( var i = 0; i < formsIndexes.length; i++ ){

			console.log("formsIndexes[" + i + "] = ", formsIndexes[i] );

			if( fs.existsSync( formsIndexes[i].file_name ) ){

				console.log("formsIndexes[" + i + "].file_name = '" + formsIndexes[i].file_name + "' seems to already exist locally...");

			}
			else {

				console.log("formsIndexes[" + i + "].file_name = '" + formsIndexes[i].file_name + "' does not seem to exist locally, downloading it now...");

				//for example, url = "ftp://ftp.sec.gov/edgar/data/1600037/14/000159958014000001/primary_doc.xml";

				var baseDir = formsIndexes[i].file_name;
				// e.g., formsIndexes[i].file_name: 'edgar/data/1494316/0001544824-14-000029.txt'
				// But first make sure the directory exists... 
				// TODO: Use a more sophisticated way to determine the baseDir...a node module,
				// or part of your "utils"...maybe lo-dash...?
				var iWhere = formsIndexes[i].file_name.lastIndexOf("/");

				if( iWhere >= 0 ){
					baseDir = formsIndexes[i].file_name.substring(0, iWhere);
				}
				console.log("Calling nodeFs.mkdirSync( '" + baseDir + "' )...");
				// node-fs: mkdirSync(path, mode, [recursive]):
				nodeFs.mkdirSync( baseDir, 0777, true );

				var ftpClient = new FtpClient();

				var connect_options = {"host": ftp_host, "port": ftp_port, "user": ftp_user, "password": ftp_password };


				ftpClient.on('ready', function(){

					console.log("*** ftpClient.get( '" + formsIndexes[i].file_name + "' )...");

					ftpClient.get( formsIndexes[i].file_name, function(err, stream){
						if(err){ console.log("Error with download of '" + formsIndexes[i].file_name + "': ", err ); }
						stream.once('close', function(){ ftpClient.end(); });
						console.log("*** stream.pipe(fs.createWriteStream( '" + formsIndexes[i].file_name + "' )...");
						stream.pipe(fs.createWriteStream( formsIndexes[i].file_name ) );
					});
				});

				ftpClient.on('error', function(err){
					console.log("*** ftpClient.on('error'): err = ", err );
				});

				console.log("*** ftpClient.connect( ", connect_options, " )...");
				ftpClient.connect( connect_options );

				//console.log("*** Got one or failed one...so breaking out of loop...");
				//break;
			}

		}/* for( var i = 0; i < formsIndexes.length; i++ ) */
	});
}


function loadDailyIndex( daily_index_path ){

	console.log("Reading file '" + daily_index_path + "'...");

	var lr = new LineByLineReader( daily_index_path );
	var line_count = 0;

	lr.on('error', function(err){
		console.log(
		"Trouble reading file '" + daily_index_path + "', aborting, err = ", err );
		return;
	});

	lr.on('line', function(line){
		line_count++;
		console.log("line " + line_count + ": \"" + line + "\"...");
		lineToJson( line, function(success, json){
			if( success ){
				console.log( "->" + "line " + line_count + ": json = ", json );
				if( json.form_type == "D" || json.form_type == "D/A" ){
					console.log("-->" + "line " + line_count + ": Hot diggity dog!  Looks like a Form D entry...let's stick it in the database...!");
					var formsIndexModel = new FormsIndexModel({		
						index_file: daily_index_path,
						company_name: json.company_name,	
						form_type: json.form_type,	
						date_filed: json.date_filed,	
						file_name: json.file_name,	
					});
				
					formsIndexModel.save(
						function(err, post){
							if(err){
								console.log("Error during formsIndexModel.save(): err = ", err );
							}
						}
					);

				}
				else {
					console.log("->*" + "line " + line_count + ": Does not appear to be a Form D entry...");
				}

			}
			else {
				console.log("*" + "line " + line_count + ": Does not appear to be an entry...");
			}
		});
	});

	lr.on('end', function(){
		console.log("That's all she wrote!  line_count = " + line_count );
	});
}

/*
* The index file looks like this:
*
* CIK|Company Name|Form Type|Date Filed|File Name
* 1000180|SANDISK CORP|4|20141031|edgar/data/1000180/0001242648-14-000081.txt
* 1012037|COMPAGNIE DE SAINT GOBAIN|D|20141031|edgar/data/1012037/0001012037-14-000001.txt
* 1084226|Searchlight Minerals Corp.|D/A|20141031|edgar/data/1084226/0001144204-14-064336.txt"...
* 1084226|Searchlight Minerals Corp.|D|20141031|edgar/data/1084226/0001144204-14-064319.txt"...
*
*/
function lineToJson( line, callback ){

    var CIK_IDX = 0;	
	var COMPANY_NAME_IDX = 1;	
	var FORM_TYPE_IDX = 2;	
	var DATE_FILED_IDX = 3;	
	var FILE_NAME_IDX = 4;	

	var field_map = [ 
		{name: "cik", index: 0 }
		,{name: "company_name", index: 1 }
		,{name: "form_type", index: 2 }
		,{name: "date_filed", index: 3 }
		,{name: "file_name", index: 4 }
	];

	var mr_fields = line.split("|");	

	var mr_json = {};
	var success = false;

	if( mr_fields.length >= field_map.length ){ 

		for( var i = 0; i < field_map.length; i++ ){
			var name = field_map[i].name;
			var index = field_map[i].index;

			mr_json[ name ] = mr_fields[ index ];
		}

		success = true;
	}

	callback( success, mr_json );
}
