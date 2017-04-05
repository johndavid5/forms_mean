/**
* 
* Stuff shared between node.js and angular.js
*
* Many thanks to...
*   http://stackoverflow.com/questions/25069777/sharing-code-between-angularjs-and-nodejs
* for this hack that allows us to share
* objects between node and angular...
*
* var mydata = {};
* 
* if(typeof window !== 'undefined'){
* 	// angular
*     window.mydata = mydata;
* } else {
* 	 // node
*      module.exports = mydata;
* }
*
*/


var SharedUtils = {

	debug: 0,

	isIntegerValid: function(input){

		input = "" + input; // Coerce to a String -- ironically -- to test to see if it's a valid Int...	

	    var re = /^[+|\-]?\d+$/;

   	 	return re.test(input);
	},

	/**
	* Many thanks to 
	*   http://stackoverflow.com/questions/46155/validate-email-address-in-javascript
	*/
	isEmailValid: function(email) {

		if( ! email ){
			return false;
		}

	    var re = /^([\w-]+(?:\.[\w-]+)*)@((?:[\w-]+\.)*\w[\w-]{0,66})\.([a-z]{2,6}(?:\.[a-z]{2})?)$/i;
   	 	return re.test(email);
	},/* isEmailValid() */

	/** 
	* Expects dateString
	*/
	isDateStringValid: function(dateString, options) {

		var sWho = "isDateStringValid";

	    var re = /^(\d{4})-(\d{1,2})-(\d{1,2})$/;

   	 	//return re.test(dateString);

		SharedUtils.debugPrint(sWho + "(): dateString = '" + dateString + "'...");

		var m = re.exec(dateString);

		SharedUtils.debugPrint(sWho + "(): m = ", m );

		if (m) {

			var year = m[1];
			var month = m[2];
			var day = m[3];

	        SharedUtils.debugPrint(sWho + "(): Looks like a match, Moe: year = '" + year + "', month = '" + month + "', day = '" + day + "'...");

			if( SharedUtils.isDateValid( year, month, day ) ){

				if( options && options instanceof Object ){
					options.formattedDateStringOut = SharedUtils.formatDateAsString( year, month, day );
					options.dateObjectOut = new Date( year, month, day, 0, 0, 0, 0 )
					// new Date(year, month, day, hours, minutes, seconds, milliseconds)
				}

	        	SharedUtils.debugPrint(sWho + "(): Moe, isDateValid() returned true, so I'm returning true...");

				return true;
			}
			else {
	        	SharedUtils.debugPrint(sWho + "(): Sorry, Moe, isDateValid() returned false, so I'm returning false...");
				return false;
			}
	    }
		else {
	        SharedUtils.debugPrint(sWho + "(): Sorry, Moe, doesn't appear to be a match...");
			return false;
		}
	},/* isDateStringValid() */


	isDateValid: function(year, month, day) {

		var sWho = "isDateValid";

	    var re = /^(\d{4})-(\d{1,2})-(\d{1,2})$/;

		SharedUtils.debugPrint(sWho + "(): year = '" + year + "', month = '" + month + "', day = '" + day + "'...");

		if( month < 1 || month > 12 ){
			return false;
		}

		if( day < 1 ){
			return false;
		}
		
		var numDaysInMonth = SharedUtils.daysInMonth( year, month );

		if( day > numDaysInMonth ){
			return false;
		}

		return true;

	},/* isDateValid() */

	daysInMonth: function( year, month ){
		/* "30 days hath September, April, June and November. 
		* All the rest have 31, except February alone, which
		* hath 28 days clear, and 29 each leap year."
		*/		
		if( month == 9 || month == 4 || month == 6 || month == 11 ){
			return 30;
		}
		else if( month == 2 ){
			if( SharedUtils.isLeapYear( year ) ){
				return 29;
			}
			else {
				return 28;
			}
		}
		else {
			return 31;
		}
	},/* daysInMonth() */

	/**
	* Many thanks to "MMeersseman" at...
	*   http://stackoverflow.com/questions/16353211/check-if-year-is-leap-year-in-javascript
	*/
	isLeapYear: function( year ){
		return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
	},/* isLeapYear() */

	/** SOURCE: http://stackoverflow.com/questions/13859538/simplest-inline-method-to-left-pad-a-string */
	lpad: function(input, padString, length) {

		input = "" + input; // coerce to String

		while (input.length < length){
	        input = padString + input;
		}

		return input;
	},/* lpad() */

	formatDateAsString: function( year, month, day ){
		return year + "-" + SharedUtils.lpad(month, "0", 2) + "-" + SharedUtils.lpad(day, "0", 2);
	}, /* formatDateAsString() */

	firstDayOfThisMonthAsString: function(){
		var date = new Date();
		var thisMonth = date.getMonth()+1;	
		var thisYear = date.getYear()+1900;	
		var firstDayOfMonth = 1;
		return SharedUtils.formatDateAsString( thisYear, thisMonth, firstDayOfMonth );
	},

	/* Should return something like "2014-01-08" for date =
    *    Wed Jan 08 2014 00:00:00 GMT-0500 (Eastern Standard Time)
	*/
	formatDateObjectAsString: function( date ){
		if( ! date ){
			return "";
		}
		else {
			return SharedUtils.formatDateAsString( date.getYear()+1900, date.getMonth()+1, date.getDate() );
		}
	}, /* formatDateObjectAsString() */

	getNumPages: function( totalEntries, entriesPerPage ){

			var sWho = "*** getNumPages";

			this.debugPrint(sWho + "(): totalEntries = " + totalEntries + ", entriesPerPage = " + entriesPerPage + "...");

			var dividend = totalEntries / entriesPerPage;

			this.debugPrint(sWho + "(): totalEntries / entriesPerPage = "
							+ dividend + "..."
			);

			var output = Math.ceil( dividend );

			this.debugPrint(sWho + "(): Math.ceil( totalEntries / entriesPerPage ) = "
							+ output + "..."
			);

			return output;

	}, /* getNumPages() */

	/**
	* void getRangesForPage( int totalEntries, int entriesPerPage, int pageNumber, Object& outOptions )
	*
	* Note that you MUST supply an options = {}; object.  It
	* will be filled in with the xxxOut params as follows:
	*
	*  outOptions = {lowEntryOut: w, highEntryOut: x, offsetOut y, limitOut z}
	*
	*  fringe condition:
	*    search(): UtilSvs.getRangesForPage( totalEntries =  0 , entriesPerPage =  100 , pageNumber =  0  )
	*    yielded rangeOutOptions =  Object {lowEntryOut: -99, highEntryOut: 0, offsetOut: -100, limitOut: 100} ...*
	*/
	getRangesForPage: function(totalEntries, entriesPerPage, pageNumber, outOptions /* in-out */){ 

			totalEntries = parseInt(totalEntries);
			entriesPerPage = parseInt(entriesPerPage);
			pageNumber = parseInt(pageNumber);

			if( totalEntries <= 0 || pageNumber <= 0 ){
				outOptions.lowEntryOut = 0;
				outOptions.highEntryOut = 0;

				outOptions.offsetOut = 0;
				outOptions.limitOut = 100;
			}
			else {
				outOptions.lowEntryOut = ( pageNumber - 1 ) * entriesPerPage  + 1;
				outOptions.highEntryOut = Math.min ( totalEntries , (pageNumber * entriesPerPage) ); 

				outOptions.offsetOut = outOptions.lowEntryOut - 1;
				outOptions.limitOut = outOptions.highEntryOut - outOptions.lowEntryOut + 1;
			}

	}, /* this.getRangesForPage() */

	/** 
	 * String ucFirstAllWords( String str, Bool boolRemainingLowercase = true )
	 *
	 * Capitalize the first letter of each word.
	 *
	 * Many thanks to John Conde's November 30, 2011 post at...
	 *    http://stackoverflow.com/questions/1026069/capitalize-the-first-letter-of-string-in-javascript
	 *
	 * JDA 2015-06-09: Added boolRemainingLowercase = true
	 * option, lowercasing the entire input string
	 * before the uppercasing operations,
	 * just in case some joker submitted something
	 * sIlLy LiKe ThIs...
	 */
	ucFirstAllWords: function( str, boolRemainingLowercase ){ 
	
			if( ! str ){
				return "";
			}
	
			if( boolRemainingLowercase ){
				str = str.toLowerCase();
			}
	
			var pieces = str.split(" ");
	
		    for ( var i = 0; i < pieces.length; i++ )
			{
				var j = pieces[i].charAt(0).toUpperCase();
				pieces[i] = j + pieces[i].substr(1);
			}
	
			return pieces.join(" ");
	
	}, /* ucFirstAllWords() */

	/**
	* Deduce the Edgar SEC Main Filing Page URL from
	* Edgar File Name, which is the path found in the
	* edgar index files, and returns it.
	*
	* For example, if 
	*   edgar_file_name = "edgar/data/1000275/0001214659-15-008322.txt"
	* (which would be found at URL = ftp://ftp.sec.gov/edgar/data/1000275/0001214659-15-008322.txt)
	* ...then return string would be...
	*   "http://www.sec.gov/Archives/edgar/data/1000275/0001214659-15-008322-index.htm"
	*
	* If puzzled by the edgar_file_name, returns empty string "".
	*/
	edgarFileNameToEdgarSecFilingPageUrl: function( edgar_file_name ) {
	
		var le_target = ".txt";

		// Add some bulletproofing...may get called before edgar_file_name is available...
		if( ! edgar_file_name ){
			return ""; 
		}
	
		var i_where = edgar_file_name.lastIndexOf( le_target );
	
		var edgar_sec_filing_page_url = "";
	
		if( i_where != -1 && i_where == ( edgar_file_name.length - le_target.length ) ){
			edgar_sec_filing_page_url = edgar_file_name.substring( 0, i_where );
			edgar_sec_filing_page_url = "http://www.sec.gov/Archives/" +  edgar_sec_filing_page_url + "-index.htm";
		}
	
		return edgar_sec_filing_page_url;
	
	}, /* edgarFileNameToEdgarSecFilingPageUrl() */

	/* In case we're using nginx as a reverse proxy and
	* prefixing everything with app name, for example,
	* http://api/joe would become http://myapp/api/joe
	*/
	getUrlPrefix: function( ) {
		return "/sec_forms";
	},


	debugPrint: function( s_msg ){
		if( SharedUtils.debug ){
			console.log( s_msg );
		}
	},

}; /* var SharedUtils =  */

if( typeof window !== 'undefined' ){

    // for angular.js...
	
	angular.module('waldoApp')
	.service('SharedUtilsSvc', function(){

		/* "extend" SharedUtils via prototype chaining...
		* ...if we want to be fancy, we should reset
		* the prototype as well...and/or use angular.extend(),
		* if we're in angular, since the prototype chaining only
		* seems to work for node...
		*/
		//this.prototype = SharedUtils;	

		/* Or do it the old fashioned way and delegate each one individually...
		* ...in case prototype chaining gets us into a trouble bubble...
		*/
		this.isIntegerValid = SharedUtils.isIntegerValid;
		this.isEmailValid = SharedUtils.isEmailValid;
		this.isDateStringValid = SharedUtils.isDateStringValid;
		this.isDateValid = SharedUtils.isDateValid;
		this.daysInMonth = SharedUtils.daysInMonth;
		this.isLeapYear = SharedUtils.isLeapYear;
		this.lpad = SharedUtils.lpad;
		this.formatDateAsString = SharedUtils.formatDateAsString;
		this.firstDayOfThisMonthAsString = SharedUtils.firstDayOfThisMonthAsString;
		this.formatDateObjectAsString = SharedUtils.formatDateObjectAsString;
		this.getNumPages = SharedUtils.getNumPages;
		this.getRangesForPage = SharedUtils.getRangesForPage;
		this.ucFirstAllWords = SharedUtils.ucFirstAllWords;
		this.edgarFileNameToEdgarSecFilingPageUrl = SharedUtils.edgarFileNameToEdgarSecFilingPageUrl;
		this.getUrlPrefix = SharedUtils.getUrlPrefix;

		/* Actually, you can do this in a for loop, Escamillo...
		*  ...a crude member-by-member style of function inheritance, actually...
		*  ...but I don't think this worked, did it...?
		*/
		/*
		  for( var property in SharedUtils ){
			if( typeof property == "function" ){
				this.property = SharedUtils.property;
			}
		}
		*/

		this.debug = 0;
	
		this.debugPrint = function( s_msg ){
			if( this.debug ){
				this.debugPrint( s_msg);
			}
		};
	
	});

} else {
	// node.js
	module.exports = SharedUtils;
}
