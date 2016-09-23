/* Cleverly extend Node's Readable object in
* such a way as to generate CSV
* output.
*/
var deepcopy = require('deepcopy');

var Readable = require('stream').Readable;

var util = require('util');

var jutils = require('../jutils');
var sharedUtils = require('../ng/shared-utils.svc.js');
var logger = require('../logger');

util.inherits(CsvInator, Readable); // extend Readable...

function CsvInator(options) {

	var sWho = "CsvInator";

	Readable.call(this, options);

	// Vagabond Loafers, c. 6:29
 	logger.info( sWho + "(): SHEMP: Hey, Moe, no wonder da water don't woyk...these pipes are clogged up widh wires...look...!");
 	logger.info( sWho + "(): MOE: Well, yank 'em out and we'll hook the pipes up all over again...you oyster brain, go on...!");
 	logger.info( sWho + "(): SHEMP: All right...!");

 	logger.debug( sWho + "(): BEFORE: this = ", this, "...");
 	logger.debug( sWho + "(): BEFORE: options = ", options, "...");

	this._options = options;
	this._entries_per_page = 100;
	this._page_number = -1;
	this._total_rows = -1;
	this._total_num_pages = -1;

    // Safety valve...?
	//this._max_pages = 100; 
	this._max_pages = -1;

 	logger.debug( sWho + "(): AFTER: this = ", this, "...");

}/* CsvInator(options) */

/* 
* http://localhost:3000/api/filings?format=csv&accession_number_filter=45&form_type_filter=10-K,%2010-Q&date_filed_from_filter=2015-07-02&date_filed_to_filter=2015-07-07
*/
CsvInator.prototype._read = function() {

	var sWho = "CsvInator::_read";

	// So Hansel and Gretel can find their way back to "this" in an inner anonymous function...
	var outerThis = this;

	var str_row = "";
	var str = "";
	var buf = null;
	var a_csv_row = [];

	this._page_number++;

 	logger.debug( sWho + "(): SHEMP: Moe, this._page_number = ", this._page_number, ": this._total_rows = ", this._total_rows );
 	logger.debug( sWho + "(): SHEMP: Moe, this._page_number = ", this._page_number, ": this._total_num_pages = ", this._total_num_pages );


	if (this._max_pages > 0 && this._page_number > this._max_pages ){
 		logger.debug( sWho + "(): this._page_number > this._max_pages: SHEMP: Sorry, Moe, gonna hafta invoke the circuit breaker...pushing null to the stream...");
		this.push(null); // finito
	}
	else if (this._page_number > 0 && this._total_rows < -1 ){
 		logger.debug( sWho + "(): this._page_number > 0 && this._total_rows < -1 : SHEMP: Sorry, Moe, looks like there ain't no rows, gonna hafta bail out...pushing null to the stream...");
		this.push(null); // finito
	}
	else if (this._total_num_pages >= 0 && this._page_number > this._total_num_pages ){ 
 		logger.debug( sWho + "(): this._total_num_pages >= 0 && this._page_number > this._total_num_pages : SHEMP: Sorry, Moe, looks like there ain't no more rows left, gonna hafta bail out now...pushing null to the stream...");
		this.push(null); // finito
	}
	else if( this._page_number == 0 ){


 		logger.debug( sWho + "(): outerThis._page_number = ", outerThis._page_number, ": Calling to fetch count via this._options.getter = ", this._options.getter, "...");

		var ourOptions = deepcopy( this._options );
		ourOptions.countOnly = true;

 		logger.debug( sWho + "(): outerThis._page_number = ", outerThis._page_number, ": ...with ourOptions = ", ourOptions, "...");

		this._options.getter( ourOptions, function(rows, rowsAffected, err){

			sWho += "::this._options.getter";

 			logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, I got back rows=", rows, ", rowsAffected=", rowsAffected, ", err=", err, "...");
 			logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, (rows instanceof Array) is ", (rows instanceof Array), "...");
			

			if( rows && rows instanceof Array && rows.length >= 1 && typeof rows[0].count != "undefined"  ){
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Setting outerThis._total_rows = rows[0].count..."); 
				outerThis._total_rows = rows[0].count;		
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: The deed is done, outerThis._total_rows = ", outerThis._total_rows, "...");

				outerThis._total_num_pages = sharedUtils.getNumPages( outerThis._total_rows, outerThis._entries_per_page );
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Moe, I just set outerThis._total_num_pages = ", outerThis._total_num_pages, "...");

 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: OK, Moe...turkey ready for workey...");
			}
			else {
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Sorry, Moe, it don't look kosher...I ain't setting outerThis._total_rows...");
			}

			// IMPORTANT: Only do *one* this.push(buf) here, or else you'll get
			// a write after EOF error...thus we'll append the front-matter
			// and the header into one big happy buffer and push it...

			str = "";

			if( outerThis._options.frontMatter && outerThis._options.frontMatter instanceof Array ){
 				logger.debug( sWho + "(): I got front matter, Moe..."); 
				for( var i = 0; i < outerThis._options.frontMatter.length; i++ ){
					str +=  "\"" + outerThis._options.frontMatter[i] + "\"" + "\n";
					if( i == outerThis._options.frontMatter.length-1 ){
						str += "\n"; // Place extra horizontal space at end of front matter...
					}
				}
			}
			else {
 				logger.debug( sWho + "(): I don't got no front matter, Moe..."); 
			}

			/*
			str = '"Filings"' + "\n";
 			logger.debug( sWho + "(): pushing '" + str + "' to stream...");
			buf = new Buffer(str, 'ascii');
			this.push(buf);
	
			str = '"Accession Number:", "' + this._options.accesion_number_filter + "'" + "\n";
	 		logger.debug( sWho + "(): pushing '" + str + "' to stream...");
			buf = new Buffer(str, 'ascii');
			this.push(buf);
	
			str = '"Form Types:", "' + this._options.form_types_filter + "'" + "\n";
	 		logger.debug( sWho + "(): pushing '" + str + "' to stream...");
			buf = new Buffer(str, 'ascii');
			this.push(buf);
	
			str = '"Date Filed From:", "' + this._options.date_filed_from_filter + "'" + "\n";
	 		logger.debug( sWho + "(): pushing '" + str + "' to stream...");
			buf = new Buffer(str, 'ascii');
			this.push(buf);
	
			str = '"Date Filed To:", "' + this._options.date_filed_to_filter + "'" + "\n";
	 		logger.debug( sWho + "(): pushing '" + str + "' to stream...");
			buf = new Buffer(str, 'ascii');
			this.push(buf);
			*/

			//str = jutils.arrayToCsv( outerThis._options.csv.fieldsPretty );
			str += jutils.arrayToCsv( outerThis._options.csv.fieldsPretty ) + "\n";
	 		logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: pushing header row '" + str + "' to the stream, Moe...");
			buf = new Buffer(str, 'ascii');
			//buf = new Buffer(str + "\n", 'ascii');
			//outerThis.push(buf + "\n");
			try {
				outerThis.push(buf);
			}
			catch( ex ){
 				logger.error( sWho + "(): I got twouble with header row, Moe:", ex, "...");
			}
		});
	}
	else {

		var outOptions = {};

		sharedUtils.getRangesForPage( this._total_rows, this._entries_per_page, this._page_number, outOptions );

		logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Moe, I just called sharedUtils.getRangesForPage( " +
						"this._total_rows = ", this._total_rows, ", this._entries_per_page = ", this._entries_per_page,
						", this._page_number = ", this._page_number, "...and I got back outOptions = ", outOptions, "..."
		);

		var ourOptions = deepcopy( this._options );
		ourOptions.maxRows = undefined;
		ourOptions.countOnly = false;
		ourOptions.lowRow = outOptions.lowEntryOut;
		ourOptions.highRow = outOptions.highEntryOut;

 		logger.debug( sWho + "(): outerThis._page_number = ", outerThis._page_number, ": Calling to fetch row via this._options.getter = ", this._options.getter, "...");

 		logger.debug( sWho + "(): outerThis._page_number = ", outerThis._page_number, ": ...with ourOptions = ", ourOptions, "...");

		this._options.getter( ourOptions, function(rows, rowsAffected, err){

			sWho += "::this._options.getter";

 			logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, I got back rows = " +  rows + ", rowsAffected=", rowsAffected, ", err=", err, "...");
 			logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, (rows instanceof Array) is ", (rows instanceof Array), "...");
			

			if( rows && rows instanceof Array ){

 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, rows.length = ", rows.length, "...");
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, outerThis._options.csv.fieldsActual = ", outerThis._options.csv.fieldsActual, "..." );

				for( var i = 0; i < rows.length; i++ ){

					var le_row = deepcopy( rows[i] );

 					logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, here's a row: row[" + i + "] = ", le_row, "...");

 					logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, outerThis._options.csv.fieldTransforms = ", outerThis._options.csv.fieldTransforms, "...");

					for( var j = 0; j < outerThis._options.csv.fieldTransforms.length; j++ ){ 
 						logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe, fieldTransforms[" + j + "] = ", outerThis._options.csv.fieldTransforms[j], "...");
 						logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: fieldTransforms[" + j + "]: BEFORE TRANSFORM: le_row = ", le_row, "...");
						var miss_field = outerThis._options.csv.fieldTransforms[j].field;
						var miss_transform = outerThis._options.csv.fieldTransforms[j].transform;
						le_row[miss_field] = miss_transform( le_row ); 
 						logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: fieldTransforms[" + j + "]: AFTER TRANSFORM: le_row = ", le_row, "...");
					}

					a_csv_row = jutils.objectToArray( le_row, outerThis._options.csv.fieldsActual );

 					logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe: row[" + i + "]: a_csv_row = ", a_csv_row );

					str_row = jutils.arrayToCsv( a_csv_row );

 					logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Hey, Moe: row[" + i + "]: str_row = \"", str_row, "\"...");
					str += str_row + "\n";
				}

	 			logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: pushing '" + str + "' to the stream, Moe...");
				buf = new Buffer(str, 'ascii');

				//outerThis.push(buf);

				try {
					outerThis.push(buf);
				}
				catch( ex ){
 					logger.error( sWho + "(): I got twouble with data row, Moe:", ex, "...");
				}
			}
			else {
 				logger.debug( sWho + "(): _page_number = ", outerThis._page_number, ": SHEMP: Sorry, Moe, rows don't look kosher...");
			}
		});

	}
};

module.exports = CsvInator;
