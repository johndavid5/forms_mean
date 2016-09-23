var deepcopy = require('deepcopy');
//var TYPES = require('tedious-ntlm').TYPES

var jutils = require('../jutils'); // jutils.js
var sharedUtils = require('../ng/shared-utils.svc.js'); 

//var log4js = require('../lib/log4js-node/lib/log4js');
//var logger = log4js.getLogger();
var logger = require('../logger'); 

var config = require('../config'); 

//var Database = require('../Database'); // Database.js
//var Entity = require('./entity'); // use to get Entity's associated with a filing...
//var Person = require('./person'); // use to get Person' associated with a filing...
//var Company = require('./company'); // use to get Company's associated with a filing...
var MongoClient = require('mongodb').MongoClient;

function Form(){

	//this.database = new Database();


	// Place copy of this into outerThis as "closure" (or "private member") so you can see this as outerThis inside the callbacks...
	var outerThis = this; 

	/**
	* void getForms(Object options, Function callback)
	*
	* "callback" should be of form
	*   function( filingRows, filingRowsAffected, filingErr )
	*
	* Supported options:
	*   e.g., { countOnly: true }
	*         { lowRow: 1, highRow: 100 }
	*
	*   e.g., { filingsId: '12345' } or { filingsId: ['12345', '54321'] }
	*         { entitiesId: '12345' } or { entitiesId: ['12345', '54321'] }
	*         { withFilingsAttributesOnly: true } // e.g., if for demo, you don't want to grab filings that don't have any filings_attributes...
	*         { fetchChildrenToo: true } // By default, will not fetch in "children" such as Entity's...
	*         { maxRows: 100 } // By default, fetches all rows...
	*/
	/* Example Form Doc:
	*			{
	*				"_id" : ObjectId("57b79ead3a23bb255c0046c4"),
	*				"cik" : 1000152,
	*				"form_type" : "X-17A-5",
	*				"date_filed" : "2016-03-31",
	*				"file_name" : "edgar/data/1000152/9999999997-16-022382.txt",
	*				"accession_number" : "9999999997-16-022382",
	*				"index_url" : "file:///../edgar/ftp.sec.gov--edgar--full-index--2016--QTR1--master.idx",
	*				"form_processing_attempts" : [
	*					{
	*						"when" : ISODate("2016-08-20T23:21:53.498Z"),
	*						"success" : true
	*					}
	*				],
	*				"filers" : [
	*					{
	*						"company_data" : {
	*							"company_conformed_name" : "WESTERN INTERNATIONAL SECURITIES, INC.",
	*							"central_index_key" : "0001000152",
	*							"standard_industrial_classification" : "",
	*							"irs_number" : "841314321",
	*							"state_of_incorporation" : "CO",
	*							"fiscal_year_end" : "1231"
	*						},
	*						"filing_values" : {
	*							"form_type" : "X-17A-5",
	*							"sec_act" : "1934 Act",
	*							"sec_file_number" : "008-48572",
	*							"film_number" : "16002216"
	*						},
	*						"business_address" : {
	*							"street_1" : "70 SOUTH LAKE AVENUE",
	*							"street_2" : "SUITE 700",
	*							"city" : "PASADENA",
	*							"state" : "CA",
	*							"zip" : "91101",
	*							"business_phone" : "626-793-7717"
	*						},
	*						"mail_address" : {
	*							"street_1" : "70 SOUTH LAKE AVE-STE. 700",
	*							"street_2" : "",
	*							"city" : "PASADENA",
	*							"state" : "CA",
	*							"zip" : "91101"
	*						}
	*					}
	*				],
	*				"issuer" : {
	*					"company_data" : {
	*						"company_conformed_name" : "",
	*						"central_index_key" : "",
	*						"standard_industrial_classification" : "",
	*						"irs_number" : "",
	*						"state_of_incorporation" : "",
	*						"fiscal_year_end" : ""
	*					},
	*					"business_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : "",
	*						"business_phone" : ""
	*					},
	*					"mail_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : ""
	*					}
	*				},
	*				"reporting_owner" : {
	*					"owner_data" : {
	*						"company_conformed_name" : "",
	*						"central_index_key" : ""
	*					},
	*					"filing_values" : {
	*						"form_type" : "",
	*						"sec_act" : "",
	*						"sec_file_number" : "",
	*						"film_number" : ""
	*					},
	*					"business_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : "",
	*						"business_phone" : ""
	*					},
	*					"mail_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : ""
	*					}
	*				},
	*				"filed_by" : {
	*					"company_data" : {
	*						"company_conformed_name" : "",
	*						"central_index_key" : "",
	*						"standard_industrial_classification" : "",
	*						"irs_number" : "",
	*						"state_of_incorporation" : "",
	*						"fiscal_year_end" : ""
	*					},
	*					"filing_values" : {
	*						"form_type" : "",
	*						"sec_act" : "",
	*						"sec_file_number" : "",
	*						"film_number" : ""
	*					},
	*					"business_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : "",
	*						"business_phone" : ""
	*					},
	*					"mail_address" : {
	*						"street_1" : "",
	*						"street_2" : "",
	*						"city" : "",
	*						"state" : "",
	*						"zip" : ""
	*					}
	*				},
	*				"dn_company_central_index_key" : "0001000152",
	*				"dn_company_conformed_name" : "WESTERN INTERNATIONAL SECURITIES, INC.",
	*				"dn_company_standard_industrial_classification" : "",
	*				"dn_company_state_of_incorporation" : "CO",
	*				"dn_company_business_address_street_1" : "70 SOUTH LAKE AVENUE",
	*				"dn_company_business_address_street_2" : "SUITE 700",
	*				"dn_company_business_address_city" : "PASADENA",
	*				"dn_company_business_address_state" : "CA",
	*				"dn_company_business_address_zip" : "91101",
	*				"dn_company_business_address_business_phone" : "626-793-7717",
	*				"dn_denormalized" : true
	*			},
	*/
	this.getForms = function( options, callback ){

		var sWho = "Form::getForms";

		console.log(sWho + "(): options = " + JSON.stringify( options ) );

		if( options == null ){
			options = {};
		}

		logger.info(sWho + "(): Connecting to \"" + config.mongoDbUrl + "\"...");

		MongoClient.connect(config.mongoDbUrl, function(err, db) {

			if( err ){
				logger.error(sWho + "(): Trouble with connect: \"" + err + "\"...");
				var rows = [
					{ error: err }
				];
				callback( rows, 0, err );
				return;
			}

			logger.info(sWho + "(): Using collection \"" + config.mongoDbFormsCollection + "\"...");
  			var collection = db.collection( config.mongoDbFormsCollection );

			//var query = {};
			//var query = { "form_processing_attempts.success" : { "$eq": true } };
			var query = { "dn_denormalized" : { "$eq": true }, "dn_company_conformed_name": { "$ne": "" } };

			if( options.accessionNumber ){
				// For example the Form Details page...
				query.accession_number = { "$eq": options.accessionNumber };
			}

			if( options.accession_number_filter ){
				// MongoDB 2.4: You can also use text index...
				// see http://stackoverflow.com/questions/10610131/checking-if-a-field-contains-a-string
				// e.g., { <field>: { $regex: /pattern/, $options: '<options>' } }
				query.accession_number = { "$regex": new RegExp('.*' + options.accession_number_filter + '.*', 'i') };
			}

			if( options.form_type_filter ){

				logger.info(sWho + "(): SHEMP: Moe, found options.form_type_filter = ", options.form_type_filter , "...");

				var a_form_type_filters =[];

				if( options.form_type_filter instanceof Array ){
					a_form_type_filters = deepcopy( options.form_type_filter );	
					logger.info(sWho + "(): SHEMP: options.form_type_filter is an Array, setting a_form_type_filters to a deepcopy of it, Moe...");
				}
				else{
					a_form_type_filters = jutils.csvSplit( options.form_type_filter );	
					logger.info(sWho + "(): SHEMP: options.form_type_filter is not an Array, setting a_form_type_filters to output of csvSplit on it, Moe...");
				}

				// e.g., { qty: { $in: [ 5, 15 ] } }
				query.form_type = { "$in": a_form_type_filters };

			}/* if( options.form_type_filter ) */

			if( options.date_filed_from_filter && sharedUtils.isDateStringValid(options.date_filed_from_filter) ){
				query.date_filed = { "$gte": options.date_filed_from_filter }; 
			}

			if( options.date_filed_to_filter && sharedUtils.isDateStringValid(options.date_filed_to_filter) ){
				if( query.date_filed ){
					// query.date_filed object already exists, so set "$lte" field...
					query.date_filed["$lte"] = options.date_filed_to_filter; 
				}
				else {
					// query.date_filed object does not already exist, so create it...
					query.date_filed = { "$lte": options.date_filed_to_filter }; 
				}
			}

			if( options.filer_name_filter ){
				// e.g., { <field>: { $regex: /pattern/, $options: '<options>' } }
				query.dn_company_conformed_name = { "$regex": new RegExp('.*' + options.filer_name_filter + '.*', 'i') };
			}

			if( options.filer_cik_filter ){
				// e.g., { <field>: { $regex: /pattern/, $options: '<options>' } }
				query.dn_company_central_index_key = { "$regex": new RegExp('.*' + options.filer_cik_filter + '.*', 'i') };
			}

			logger.info(sWho + "(): query = ", query, "..."); 

			if( options.countOnly ){

				logger.info(sWho + "(): Calling collection.count()...");

  				collection.count(query, function(err, count){

					if( err ){
						logger.error(sWho + "(): Trouble with count: \"" + err + "\"...");
						var rows = [
							{ error: err }
						];
						callback( rows, 0, err );
						return;
					}

					logger.info(sWho + "(): Returning count = " + count + " to callback...");
					var rows = [
						{ "count": count }
					];
					callback( rows, count, null );  
					logger.info(sWho + "(): Calling db.close() and returning from function...");
					db.close();
					return;
				});
			}
			else {
				var iSkip;
				var iLimit;

				if( ! options.lowRow && ! options.highRow ){
					iSkip = 0;
					iLimit = 100;
				}
				else {
					// Cast them to integers to avoid hanky-panky...
					options.lowRow = parseInt(options.lowRow);
					options.highRow = parseInt(options.highRow);

					// Insanity Check...
					if(options.lowRow < 0){
						options.lowRow = 0;
					}

					// Insanity Check...
					if(options.highRow < 0){
						options.highRow = 0;
					}

					// Insanity Check: swap them if lowRow is not less than or equal to highRow
					if(options.lowRow > options.highRow){
						var temp = options.highRow;
						options.highRow = options.lowRow;
						options.lowRow = options.highRow;
					}

					iSkip = options.lowRow - 1;
					iLimit = options.highRow - options.lowRow + 1;
				}


				var sort = {};

				if( options.orderBy ){

					if( options.orderBy == "date_filed" ){
						if( options.ascDesc == "desc" ){
							sort = {"date_filed": -1 };
						}
						else {
							sort = {"date_filed": 1 };
						}
					}
					else if( options.orderBy == "accession_number" ){
						if( options.ascDesc == "desc" ){
							sort = {"accession_number": -1 };
						}
						else {
							sort = {"accession_number": 1 };
						}
					}
					else if( options.orderBy == "form_type" ){
						if( options.ascDesc == "desc" ){
							sort = {"form_type": -1 };
						}
						else {
							sort = {"form_type": 1 };
						}
					}
					else if( options.orderBy == "dn_company_conformed_name" ){
						if( options.ascDesc == "desc" ){
							sort = {"dn_company_conformed_name": -1 };
						}
						else {
							sort = {"dn_company_conformed_name": 1 };
						}
					}
					else if( options.orderBy == "dn_company_central_index_key" ){
						if( options.ascDesc == "desc" ){
							sort = {"dn_company_central_index_key": -1 };
						}
						else {
							sort = {"dn_company_central_index_key": 1 };
						}
					}

				}/* if( options.orderBy ) */

				console.log(sWho + "(): Calling collection.find(), query = ", query, "sort = ", sort, "skip=" + iSkip + ", limit=" + iLimit + "...");

  				collection.find(query).sort(sort).skip(iSkip).limit(iLimit).toArray(function(err, items) {

					if( err ){
						logger.error(sWho + "(): Trouble with count: \"" + err + "\"...");
						var rows = [
							{ error: err }
						];
						callback( rows, 0, err );
						return;
					}

					logger.info(sWho + "(): items.length = " + items.length + "...");
					if( items.length >= 1 ){
						logger.info(sWho + "(): items[0] = " + items[0] + "...");
					}
					//logger.info(sWho + "(): Returning items = ",  items, " to callback...");
					logger.info(sWho + "(): Returning items to callback...");

					callback( items, items.length, null );

					logger.info(sWho + "(): Calling db.close() and returning from function...");
					db.close();
					return;
				});

			}/* else */

		});/* MongoClient.connect(config.mongoDbUrl, function(err, db) */

	}; /* getForms() */

	this.getFormTypes = function( options, callback ){

		var sWho = "Form::getFormTypes";

		console.log(sWho + "(): options = " + JSON.stringify( options ) );

		if( options == null ){
			options = {};
		}

		logger.info(sWho + "(): Connecting to \"" + config.mongoDbUrl + "\"...");

		MongoClient.connect(config.mongoDbUrl, function(err, db) {

			if( err ){
				logger.error(sWho + "(): Trouble with connect: \"" + err + "\"...");
				var rows = [
					{ error: err }
				];
				callback( rows, 0, err );
				return;
			}

			logger.info(sWho + "(): Using collection \"" + config.mongoDbFormsCollection + "\"...");
  			var collection = db.collection( config.mongoDbFormsCollection );

			//var query = {};
			//var query = { "form_processing_attempts.success" : { "$eq": true } };
			//var query = { "dn_denormalized" : { "$eq": true } };
			var query = { "dn_denormalized" : { "$eq": true }, "dn_company_conformed_name": { "$ne": "" } };

			var field = "form_type";
			console.log(sWho + "(): Calling collection.distinct( \"" + field + "\", ", query, " )...");

			collection.distinct( field, query, function(err, items) {

				if( err ){
					logger.error(sWho + "(): Trouble with count: \"" + err + "\"...");
					var rows = [
						{ error: err }
					];
					callback( rows, 0, err );
					return;
				}

				logger.info(sWho + "(): items.length = " + items.length + "...");
				if( items.length >= 1 ){
					logger.info(sWho + "(): items[0] = " + items[0] + "...");
				}
				if( items.length >= 2 ){
					logger.info(sWho + "(): items[1] = " + items[1] + "...");
				}
				if( items.length >= 3 ){
					logger.info(sWho + "(): items[2] = " + items[2] + "...");
				}
				//logger.info(sWho + "(): Returning items = ",  items, " to callback...");
				logger.info(sWho + "(): Returning items to callback...");

				callback( items, items.length, null );

				logger.info(sWho + "(): Calling db.close() and returning from function...");
				db.close();
				return;

			});/* collection.distinct( field, query, function(err, items) */

		});/* MongoClient.connect(config.mongoDbUrl, function(err, db) */

	}; /* getFormTypes() */

	this.getFormsFaux = function( options, callback ){

		var sWho = "Form::getForms";

		console.log(sWho + "(): options = " + JSON.stringify( options ) );

		if( options == null ){
			options = {};
		}

		var filingErr = null;

		if( options.countOnly ){
			var filingRows = [
				{ count: "3" }
			];
			callback( filingRows, 3, filingErr );  
			return;
		}

		var filingRows = [
			{ accession_number: "1", form_type: "D", date_filed: "2016-09-18", company_name: "ACME Plumbing", company_cik: "112233" },
			{ accession_number: "2", form_type: "10-K", date_filed: "2016-09-19", company_name: "Kovacs Plumbing", company_cik: "112234" },
			{ accession_number: "3", form_type: "10-Q", date_filed: "2016-09-20", company_name: "ABC Plumbing", company_cik: "112235" },
		];
		callback( filingRows, filingRows.length, filingErr );  

	}; /* getFormsFaux() */

	

} /* function Form() */

module.exports = Form;
