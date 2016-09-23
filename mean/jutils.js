var log4js = require('./lib/log4js-node/lib/log4js');
var logger = log4js.getLogger();

var JUtils = {

	debug: 0, 

	endsWith: function(str, suffix){
		return str.indexOf(suffix, str.length - suffix.length) !== -1;
	},

	/**
	* String arrayWheresToStringWheres( Array aWheres, String sPrefix = "", String sTerminalSuffix = "")
	*
	* e.g.,	sWheres = arrayWheresToStringWheres( ["fa.filings_size_bigint IS NOT NULL"
	*					  "f.date_filed >= '2015-01-01'"
	*					  "f.filings_id = '234'"], "\t" );
	* ...then...
	* sWheres = 
	* "\t" + "WHERE fa.filings_size_bigint IS NOT NULL\n" +
	* "\t" + "AND f.date_filed >= '2015-01-01'\n" +
	* "\t" + "AND f.filings_id = '234'"
	*
	* For SQL Building.
	*/
	arrayWheresToStringWheres: function( aWheres, sPrefix, sTerminalSuffix ){

		var sOutput = "";

		if( typeof sPrefix == 'undefined' ){
			sPrefix = "";
		}

		if( typeof sTerminalSuffix == 'undefined' ){
			sTerminalSuffix = "";
		}

		for( var i = 0; i < aWheres.length; i++ ){
			if( i == 0 ){
				sOutput += "" + sPrefix + "WHERE ";		
			}
			else {
				sOutput += "\n" + sPrefix + "AND ";		
			}

			sOutput += "" + aWheres[i];
		}

		if( sOutput ){
			return sOutput + sTerminalSuffix;
		}
		else {
			return sOutput;
		}
	}, /* arrayWheresToStringWheres() */

	/**
	* String arraySelectsToStringSelects( Array aSelects, String sPrefix = "")
	*
	* @param sPrefix = ""
	*  For cosmetic purposes only...prefix for each line of output...
	*
	* @param options = {}
	* e.g., { "as_em" : true }
	* Set options.as_em to true if you want...
	*     "e.entities_id as ent_id" to become just "ent_id".
	*       ...also...
	*     "e.entities_id" becomes just "entities_id".
	*
	* For SQL Building.
	*/
	arraySelectsToStringSelects: function( aSelects, sPrefix, options ){

		var sOutput = "";

		if( typeof sPrefix == 'undefined' ){
			sPrefix = "";
		}

		if( typeof options == 'undefined' || ! options instanceof Object ){
			options = {};
		}

		var bAsEm = false;
		if( options.as_em ){
			bAsEm = true;
		}

		var i;
		for( i = 0; i < aSelects.length; i++ ){

			if( i > 0 ){
				sOutput += ",\n";
			}
			else {
				sOutput += "";
			}

			if( bAsEm ){
				sOutput += "" + JUtils.asIt( aSelects[i] );
			}
			else {
				sOutput += "" + aSelects[i];
			}

		}

		return sOutput;

	}, /* arraySelectsToStringSelects() */

	/* For SQL Building 
	*
	*  e.g., asIt( "e.entities_id as ent_id" ) = "ent_id"
	*        asIt( "e.entities_id" ) = "entities_id"
	*        asIt( "entities_id" ) = "entities_id"
	*/
	asIt: function( sInput ){

		var sWho = "jutils::asIt";

		sInput = "" + sInput; // Armenian-style cast to String 

		JUtils.debugPrint( sWho + "(): sInput = \"" + sInput + "\"...\n");

		var matchees = sInput.match(/(\w+)\.(\w+)(\s+)as(\s+)(\w+)/); 

		JUtils.debugPrint( sWho + "(): I: matchees = " + JSON.stringify( matchees ) + "...\n");

		// jutils::asIt(): sInput = "e.entities_id as ent_id"...
		//jutils::asIt(): matchees = ["e.entities_id as ent_id","e","entities_id"," "," ","ent_id"]...

		if( matchees && matchees.length > 0 ){
			// Last match should be the "as"...
			return matchees[ matchees.length - 1 ];	
		}

		////////////////

		var matchees = sInput.match(/(\w+)\.(\w+)/); 

		JUtils.debugPrint( sWho + "(): II: matchees = \"",  JSON.stringify( matchees ) + "\"...\n");

		// jutils::asIt(): sInput = "e.entities_id"...
		// jutils::asIt(): I: matchees = null...
		// jutils::asIt(): II: matchees = " ["e.entities_id","e","entities_id"]"...

		if( matchees && matchees.length > 0 ){
			// As before, last match should be the "as"...
			return matchees[ matchees.length - 1 ];	
		}

		////////////////

		// jutils::asIt(): sInput = "entities_id"...
		// jutils::asIt(): I: matchees = null...
		// jutils::asIt(): II: matchees = " null"...

		/* Assume simple expression such as "entities_id". */
		return sInput;

		///////////////
	}, /* asIt */

	/** For SQL Building. */
	arrayJoinsToStringJoins: function( aJoins, sPrefix, sTerminalSuffix ){

		var sOutput = "";

		if( typeof sPrefix == 'undefined' ){
			sPrefix = "";
		}

		if( typeof sTerminalSuffix == 'undefined' ){
			sTerminalSuffix = "";
		}

		for( var i = 0; i < aJoins.length; i++ ){

			if( i > 0 ){
				sOutput += "\n";
			}

			sOutput += "" + aJoins[i];
		}

		if( sOutput ){
			return sOutput + sTerminalSuffix;
		}
		else {
			return sOutput;
		}

	}, /* arrayJoinsToStringJoins() */

	/** For SQL Building. */
	idsToInOrEquals: function( sEntity, ids, sQuoteChar, sPrefix ){

		var sWho = "JUtils.idsToInOrEquals";

		//logger.trace(sWho + "(): sEntity = ", sEntity, ", ids = ", ids, ", sQuoteChar = ", sQuoteChar, ", sPrefix =", sPrefix,  ",...");

		if( ! sQuoteChar ){
			sQuoteChar = "'";
		}

		if( ! sPrefix ){
			sPrefix = "";
		}

		if( ! ids ){
			return "";
		}

		var bIsArray = ids instanceof Array;

		//logger.trace(sWho + "(): bIsArray = \"" + bIsArray + "\"...");

		if( bIsArray ){
			if( ids.length == 0 ){			
				return "";
			}
			else if( ids.length == 1 ){			
				return sPrefix + sEntity + " = " + sQuoteChar + ids[0] + sQuoteChar;
			}
			else {
				return sPrefix + sEntity + " IN (" + JUtils.arrayToStringList(ids, sQuoteChar) + ")"; 
			}
		}
		else{
			return sPrefix + sEntity + " = " + sQuoteChar + ids + sQuoteChar;
		}

	}/* idsToInOrEquals() */,

	/**
	* String arrayToStringList ( Array aElements, String sQuoteChar = "'")
	* e.g., arrayToStringList( [1, 2, 3] ) = "'1', '2', '3'"
	* 		arrayToStringList( [1, 2, 3], "'" ) = "'1', '2', '3'"
	* 		arrayToStringList( [1, 2, 3], "" ) = "1, 2, 3"
	*/
	arrayToStringList: function( aElements, sQuoteChar ){

		var sOutput = "";

		if( typeof sQuoteChar == 'undefined' ){
			sQuoteChar = "'";
		}

		for( var i = 0; i < aElements.length; i++ ){

			if( i > 0 ){
				sOutput += ",";
			}

			sOutput += sQuoteChar + aElements[i] + sQuoteChar;
		}

		return sOutput;
	},/* arrayToStringList() */

	arrayPush: function( leArray, leElement ){

		//leArray[ leArray.length ] = leElement;

		// Supposedly leArray.push(element) is faster than
		// leArray[ leArray.length ] = element.
		// See http://stackoverflow.com/questions/614126/why-is-array-push-sometimes-faster-than-arrayn-value
		leArray.push( leElement);

	},/* arrayPush() */

	strTimesN: function( input, n ){

		var output = "";

		if( ! n ){
			n = 0;
		}

		for( var i = 1; i <= n; i++ ){
			output += "" + input;	
		}

		return output;
	},


	objectToString: function (o, args, nestingLevel) {

		var withTypes = false;
		if( args && args.withTypes )
		{
			withTypes = true;	
		}

		if( ! nestingLevel ){
			nestingLevel = 0;
		}

		var sPrefix = JUtils.strTimesN(" ", nestingLevel);

		if(o === null)	
		{
			return (withTypes?"(object)":"") + "null";
		}
	
		var type = typeof o;
	
		if (type == "number") { 
				return (withTypes?"(number)":"") + o;
		} 
		else if (type == "string") { 
				return (withTypes?"(string)":"") + "\"" + o + "\"";
		} 
		else if (type == "boolean") { 
				return (withTypes?"(boolean)":"") + o;
		} 
		else if (type == "function") { 
				return (withTypes?"(function)":"") + o;
		} 
		else if (type == "undefined") { 
				return (withTypes?"(undefined)":"") + "undefined";
		} 
	
		var is_array = o instanceof Array;
		//alert('o instanceof Array=' + (o instanceof Array));
		//alert("is_array = " + is_array );
	
		var output="";
	
		if(is_array)
		{
			output += (withTypes?"(array)":"") + "[";
			var length=o.length;
			for(var i=0; i<length; i++) 
			{
				val=JUtils.objectToString(o[i], args, nestingLevel+1);
	
				if(i > 0)
				{
					output += "" + ",";
				}
		
				output += val;
			}
			output += "]";
		}
		else
		{
			output += (withTypes?"(object)":"") + "{";		
			var count=0;
			var s_own_property="";
			for(var property in o)
			{
				count++;
				//alert("property='" + property + "'");
				if(o.hasOwnProperty(property))
				{
					//s_own_property="(OWN PROPERTY)";
					s_own_property="";
				}
				else
				{
					//s_own_property="(PROTO PROPERTY)";
					s_own_property="*";
				}
		
				try	
				{
				  var rhs = JUtils.objectToString(o[property], args, nestingLevel+1);
				}
				catch(e)
				{
					rhs = "Exception: '" + e.message + "'";
				}
		
				//alert("rhs='" + rhs + "'");
		
				if(count > 1)
				{
					//output += "" + ",\n";
					output += "" + ",";
				}
				else
				{
					output += "" + "\n";
				}
		
				output += "" + sPrefix + s_own_property + property + ": " + rhs;
			}
			output += "}";		
		}

	//alert("output='" + output + "'");
	return output;

},/* objectToString() */

/** SOURCE: http://stackoverflow.com/questions/13859538/simplest-inline-method-to-left-pad-a-string */
lpad: function(input, padString, length) {
	input = "" + input; // coerce to String
    while (input.length < length){
        input = padString + input;
	}
    return input;
},/* lpad() */

/** e.g., "2015-11-03 5:14:10 PM GMT-5.00" */
dateTimeCompact: function( date ){

	if( ! date ){
		date = new Date();
	}

	//var i_month = 0 + date.getMonth() + 1;
	//var s_month = "" + month;
	//s_month = JUtils.lpad( s_month, '0', 2);

	var i_hours_24 = date.getHours();

	var i_hours_12;
	var s_am_pm;

	if( i_hours_24 == 0 || i_hours_24 == 24 ){
		i_hours_12 = "12";
		s_am_pm = "AM";
	}
	else if( i_hours_24 >= 1 && i_hours_24 <= 12 ){
		i_hours_12 = i_hours_24;
		s_am_pm = "AM";
	}
	else if( i_hours_24 >= 13 && i_hours_24 <= 23 ){
		i_hours_12 = i_hours_24 - 12;
		s_am_pm = "PM";
	}

	i_timezone_offset_minutes = 1 * date.getTimezoneOffset();
	i_timezone_offset_hours = i_timezone_offset_minutes / 60;
	i_timezone_offset_hours = i_timezone_offset_hours.toFixed(2);

	return "" + date.getFullYear() +
		"-" + JUtils.lpad( 0 + date.getMonth() + 1 , '0', 2) +
		"-" + JUtils.lpad( date.getDate(), '0', 2 ) +
		//" " + JUtils.lpad( i_hours_12, ' ', 2) +
		" " + i_hours_12 +
		":" + JUtils.lpad( date.getMinutes(), '0', 2) +
		":" + JUtils.lpad( date.getSeconds(), '0', 2) +
		" " + s_am_pm + 
		" " + "GMT" + (i_timezone_offset_hours<0 ? "+" : "-") + i_timezone_offset_hours;
	;
},/* dateTimeCompact() */

/** getOrdinalSuffix( input )
* 
* Returns ordinal suffix for numerical input, e.g., 
* 
*  getOrdinalSuffix(0) = "th";
*  getOrdinalSuffix(1) = "st";
*  getOrdinalSuffix(2) = "nd";
*  getOrdinalSuffix(3) = "rd";
*  getOrdinalSuffix(4) = "th";
*  getOrdinalSuffix(5) = "th";
*  getOrdinalSuffix(6) = "th";
*  getOrdinalSuffix(7) = "th";
*  getOrdinalSuffix(8) = "th";
*  getOrdinalSuffix(9) = "th";
*  getOrdinalSuffix(10) = "th";
*  getOrdinalSuffix(11) = "th";
*  getOrdinalSuffix(12) = "th";
*  getOrdinalSuffix(13) = "th";
*  getOrdinalSuffix(14) = "th";
*  getOrdinalSuffix(15) = "th";
*  getOrdinalSuffix(16) = "th";
*  getOrdinalSuffix(17) = "th";
*  getOrdinalSuffix(18) = "th";
*  getOrdinalSuffix(19) = "th";
*  getOrdinalSuffix(20) = "th";
*  getOrdinalSuffix(21) = "st";
*  getOrdinalSuffix(22) = "nd";
*  getOrdinalSuffix(23) = "rd";
*  getOrdinalSuffix(24) = "th";
*  getOrdinalSuffix(25) = "th";
*  getOrdinalSuffix(26) = "th";
*  getOrdinalSuffix(27) = "th";
*  getOrdinalSuffix(28) = "th";
*  getOrdinalSuffix(29) = "th";
*  getOrdinalSuffix(30) = "th";
*  ...
*  getOrdinalSuffix(100) = "th";
*  getOrdinalSuffix(101) = "st";
*  getOrdinalSuffix(102) = "nd";
*  getOrdinalSuffix(103) = "rd";
*  getOrdinalSuffix(104) = "th";
*  ...
*  getOrdinalSuffix(1000) = "th";
*  getOrdinalSuffix(1001) = "st";
*  getOrdinalSuffix(1002) = "nd";
*  getOrdinalSuffix(1003) = "rd";
*  getOrdinalSuffix(1004) = "th";
* 
*/
getOrdinalSuffix: function( input ){ 

	var sWho = "JUtils::getOrdinalSuffix";

	input *= 1; // Armenian-style cast to numerical value...

	JUtils.debugPrint(sWho + "(): input = " + input + "...");

	if( input == 0 ){
		return "th";
	}
	else if( input == 1 ){
		return "st";
	}
	else if( input == 2 ){
		return "nd";
	}
	else if( input == 3 ){
		return "rd";
	}
	else if( input >= 4 && input <= 10 ){
		return "th";
	}
	else if( input >= 11 && input <= 19 ) {
		return "th";
	}

	var s_input = "" + input; // Armenian-style cast to String...

	//var last_digit = substr( $input, length($input)-1, 1);
	var last_digit = s_input.substr( s_input.length-1, 1);
	//var last_digit = s_input.substring( s_input.length-1, s_input.length);

	//var last_two_digits = substr( $input, length($input)-2, 2);
	var last_two_digits = s_input.substr( s_input.length-2, 2);
	//var last_two_digits = s_input.substring( s_input.length-2, s_input.length);

	//var first_of_last_two_digits = substr( $last_two_digits, 0, 1 );
	var first_of_last_two_digits = last_two_digits.substr( 0, 1 );

	// IMPORTANT: Coerce to numericals AFTER treating them as Strings above...
	last_digit *= 1;
	last_two_digits *= 1;
	first_of_last_two_digits *= 1;

	if( first_of_last_two_digits == 1 ){
		return "th";
	}

	if( last_digit == 0 ){
		return "th";	
	}
	else if( last_digit == 1 ){
		return "st";	
	}
	else if( last_digit == 2 ){
		return "nd";	
	}
	else if( last_digit == 3 ){
		return "rd";	
	}
	else if( last_digit >=4 && last_digit <= 9 ){
		return "th";	
	}

	return "";

},/* getOrdinalSuffix */

months: [ "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" ],

days: [ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"],

/** e.g., "Tuesday, November 3rd, 2015 5:14:10 PM GMT-5.00" */
dateTimePretty: function( date ){

	if( ! date ){
		date = new Date();
	}

	//var i_month = 0 + date.getMonth() + 1;
	//var s_month = "" + month;
	//s_month = JUtils.lpad( s_month, '0', 2);

	var i_hours_24 = date.getHours();

	var i_hours_12;
	var s_am_pm;

	if( i_hours_24 == 0 || i_hours_24 == 24 ){
		i_hours_12 = "12";
		s_am_pm = "AM";
	}
	else if( i_hours_24 >= 1 && i_hours_24 <= 12 ){
		i_hours_12 = i_hours_24;
		s_am_pm = "AM";
	}
	else if( i_hours_24 >= 13 && i_hours_24 <= 23 ){
		i_hours_12 = i_hours_24 - 12;
		s_am_pm = "PM";
	}

	i_timezone_offset_minutes = 1 * date.getTimezoneOffset();
	i_timezone_offset_hours = i_timezone_offset_minutes / 60;
	i_timezone_offset_hours = i_timezone_offset_hours.toFixed(2);

	return "" + JUtils.days[ date.getDay() ] + ", " +
	JUtils.months[ date.getMonth() ] + 
	" " + date.getDate() + JUtils.getOrdinalSuffix( date.getDate() ) + ", " +
	date.getFullYear() + " " +
	i_hours_12 +
	":" + JUtils.lpad( date.getMinutes(), '0', 2) +
	":" + JUtils.lpad( date.getSeconds(), '0', 2) +
	" " + s_am_pm + 
	" " + "GMT" + (i_timezone_offset_hours<0 ? "+" : "-") + i_timezone_offset_hours;
	;

},/* dateTimePretty() */

/** 
* String[] csvSplit( String s_input )
*
* Splits on "," then trims whitespace, and
* returns an array.
* 
* e.g., JUtils.csvSplit("moe@stooges.com, larry@stooges.com, curly@stooges.com, shemp@stooges.com")
*  returns the array ["moe@stooges.com", "larry@stooges.com", "curly@stooges.com", "shemp@stooges.com"].
*/
csvSplit: function( s_input ){

	// Armenian-style "cast": Make sure it's a String...
	s_input = "" + s_input;

	var a_input = s_input.split(",");

	for( var i = 0; i < a_input.length; i++ ){
		a_input[i] = a_input[i].trim();
	}

	return a_input;

},/* csvSplit */

stringToBool: function( s_input ){

	var sWho = "jutils::stringToBool";

	JUtils.debugPrint(sWho + "(): s_input = ", s_input, "...");

	var sTypeOf = typeof s_input;
	JUtils.debugPrint(sWho + "(): typeof s_input  = \"" + sTypeOf + "\"...");

	var sTrueyFalsey = "";
	if( s_input ){
		sTrueyFalsey = "TRUEY";
	}
	else {	
		sTrueyFalsey = "FALSEY";
	}

	JUtils.debugPrint(sWho + "(): s_input is " + sTrueyFalsey + "...");

	if( ! s_input ){
		return false;
	}
	else if( sTypeOf == "string" ){

		s_input = s_input.trim().toLowerCase();

		if( s_input == "false" || s_input == "0" ){
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}

},/* stringToBool() */


arrayToCsv: function( aInput ){ 

	if( ! aInput || ! aInput instanceof Array ){
		return "";
	}

	var sOut = "";

	for( var i = 0 ; i < aInput.length; i++ ){	
		if( i > 0 ){
			sOut += ",";
		}
		sOut += "\"" + aInput[i] + "\"";
	}

	return sOut;

},/* arrayToCsv() */

/**
*
* Array& objectToArray( Object& oInput, Array& aFields ); 
*
* Creates an array from oInput following
* the ordering and fields specified in aFields.
* 
* For example, 
*   objectToArray(
*		{"name": "Moe", "function": "Big Boss", "line": "Why, you...!"},
*       ["name", "function", "line"]
*   ) returns
*      ["Moe", "Big Boss", "Why, you...!"]
*
*   objectToArray(
*		{"name": "Moe", "function": "Big Boss", "line": "Why, you...!"},
*       ["line", "name"]
*   ) returns
*      ["Why, you...!", "Moe"]
*/
objectToArray: function( oInput, aFields ){

	var a_output = [];

	for( var i = 0; i < aFields.length; i++ ){
		var key = aFields[i];
		a_output.push( oInput[ key ] );
	}

	return a_output;
},/* objectToArray() */

/** (Array or null) arrayIfInputTrueyMaybeCreateeThenPushee(
*       (Array or null) oCurrentArray, (object) oObject, (string) sMessage )
*/
arrayIfInputTrueyMaybeCreateeThenPushee: function( oCurrentArray, oObject, sMessage ){
	if( ! oObject ){
		return oCurrent;
	}
	else {
		if( ! oCurrentArray || ! oCurrentArray instanceof Array ){
			oCurrentArray = [];
		}

		if( sMessage ){
			oCurrentArray.push( { "message": sMessage, "object": oObject } ); 
		}
		else {
			oCurrentArray.push( oObject );
		}

		return oCurrentArray;
	}
},/* arrayIfInputTrueyMaybeCreateeThenPushee() */

isNull: function( s_input, s_prefix, s_suffix, s_null ){
	if( s_input == null || typeof(s_input) == 'undefined' ){
		return s_null;
	}
	else {
		return s_prefix + s_input + s_suffix;
	}
},

arrayPruneIf: function( a_input, if_func ){

	// Important: traverse from end to beginning of array so you can splice() out
	// array elements without having to worry about re-traversing the same index...
	for( var i = a_input.length-1; i >= 0; i-- ){	
		if( if_false_func( a_input[i] ) ){
			a_input.splice(i, 1); // Remove it...
		}
	}
},

/**
* http://stackoverflow.com/questions/9229645/remove-duplicates-from-javascript-array
*/
uniqueArray: function(a) {

	var prims = {"boolean":{}, "number":{}, "string":{}}, objs = [];

	return a.filter(function(item) {
        var type = typeof item;
        if(type in prims){
            return prims[type].hasOwnProperty(item) ? false : (prims[type][item] = true);
		}
        else{
            return objs.indexOf(item) >= 0 ? false : objs.push(item);
		}
   	});

}, /* uniqueArray() */

debugPrint: function( s_input ){
	if( JUtils.debug ){
		console.log( s_input );
	}
},




}/* var JUtils = */

module.exports = JUtils;
