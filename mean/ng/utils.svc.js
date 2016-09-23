
    // angular.js
	
	angular.module('waldoApp')
	.service('UtilsSvc', function(){
	
		this.debug = false;

	
		/**
		* For Example: 
		*   propertiesToQueryString({"name": "Moe", "function": "Big Boss", "line": "Why, you...!"}) = 
	 	*/
		this.propertiesToQueryString = function( properties ){
	
			if( typeof properties == 'undefined' ){
				return "";
			}
	
			var iCount = 0;
			var sOutput = "";
			for( var propertyName in properties ){
				iCount++;
				if( iCount == 1 ){
					sOutput += "?"; 
				}
				else {
					sOutput += "&"; 
				}

				var sVal = "";
				if( properties[propertyName] ){
					sVal = properties[propertyName];
				}
				else{
					sVal = "";
				}

				sOutput += propertyName + "=" + encodeURIComponent( sVal );
			}
	
			return sOutput;
	
		};/* propertiesToQueryString() */
	
	
	
		/* Actually we have array.push( element ) built into the Array object... */
		this.arrayPush = function( leArray, lePushee ){
			leArray[ leArray.length ] = lePushee;
		}; /* arrayPush() */


		/* e.g., var leIndex = UtilsSvc.arrayFind( leArray, function(input){ return input.toUpperCase() == "JOE"; } ); */
		this.arrayFind = function( leArray, finderFunc ){

			if( ! leArray || ! leArray instanceof Array ){
				return -1;
			}
	
			for( var i = 0; i < leArray.length; i++ ){
				if( finderFunc( leArray[i] ) ){
					return i;
				}
			}

			return -1;

		}; /* arrayFind() */
	
		/* NOTE: Trims it if it's a string... */
		this.isNull = function( s_input, s_default ){

			if( typeof s_default == "undefined" ){
				s_default = "";
			}

			if( s_input && typeof s_input != "undefined" ){

				// coerce to string...
				s_input = "" + s_input;

				if( typeof s_input == "string"){
					return s_input.trim();
				}
				else{
					return s_input;
				}
			}
			else {	
				return s_default;
			}
		};
	
		this.stringToBool = function( s_input ){
	
			var sWho = "jutils::stringToBool";
		
			this.debugPrint(sWho + "(): s_input = ", s_input, "...");
		
			var sTypeOf = typeof s_input;
			this.debugPrint(sWho + "(): typeof s_input  = \"" + sTypeOf + "\"...");
		
			var sTrueyFalsey = "";
			if( s_input ){
				sTrueyFalsey = "TRUEY";
			}
			else {	
				sTrueyFalsey = "FALSEY";
			}
		
			this.debugPrint(sWho + "(): s_input is " + sTrueyFalsey + "...");
		
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
	
		};/* stringToBool() */

		/* Returns the array used to display the paginator...
		*  e.g., "1", "2", "3", "4", "5"
		*  or    "6", "7", "8", "9", "10"
		*/
		this.get_pagination_pages_array = function(max_pagination_pages, pending_page_num, total_num_pages){
	
			var sWho = "get_pagination_pages_array";
	
			console.log(sWho + "(): max_pagination_pages = ", max_pagination_pages, "...");
			console.log(sWho + "(): pending_page_num = ", pending_page_num, "...");
			console.log(sWho + "(): total_num_pages = ", total_num_pages, "...");
	
			var a_pagination_pages_array;
	
			if( total_num_pages > max_pagination_pages ){
				// Put the max_pagination_pages pages "central" to pending_page_num in there, Scotty...
				a_pagination_pages_array = new Array( max_pagination_pages );
	
				// e.g., if max_pagination_pages = 10, delta = Math.floor( 10 / 2 ) - 1 = Math.floor( 5 ) - 1 = 5 -1 = 4;
				var delta = Math.floor( (max_pagination_pages - 1) / 2 );
	
				console.log(sWho + "(): delta = ", delta );
	
				// Note: keep co-ercing xxx to integers via "1 * x" to prevent wacky stuff from
				// happening such as 27 + 455 = 27455...
				var page_start = (1 * pending_page_num) - delta;
	
				console.log(sWho + "(): page_start = ", page_start );
	
				if( page_start < 1 ){
					console.log(sWho + "(): Adjusting page_start too low...");
					page_start = 1;
					console.log(sWho + "(): after adjusting, page_start = ", page_start );
				}
	
				var page_end = (1 * pending_page_num) + delta;
	
				console.log(sWho + "(): page_end = ", page_end );
	
				if( page_end > total_num_pages ){
					console.log(sWho + "(): Adjusting page_end too high...");
					page_end = 1 * total_num_pages;
					page_start =  page_end - (1 * max_pagination_pages) + 1;
					console.log(sWho + "(): After adjusting, page_start = ", page_start, ", page_end = ", page_end, "..." );
				}
	
				console.log(sWho + "(): finally, page_start = ", page_start, ", page_end = ", page_end, "..." );
	
				var i;	
	
				for( i = 0; i < max_pagination_pages; i++ ){
					var patti_page = 0 + page_start + i;
					console.log(sWho + "(): i = " + i + ": Setting a_pagination_pages_array[" + i + "] equal to (page_start + i) = " + patti_page + "...");
					a_pagination_pages_array[i] = patti_page;
				}
	
			}
			else {
				// Put all the pages in there, Scotty...
				a_pagination_pages_array = new Array( total_num_pages );
	
				var i;
				for( i = 0; i < total_num_pages; i++ ){
					a_pagination_pages_array[i] = i+1; 
				}
			}
	
			return a_pagination_pages_array;
	
		}; /* get_pagination_pages_array() */

		/** 
		* String[] csvSplit( String s_input )
		*
		* Splits on "," then trims whitespace, and
		* returns an array.
		* 
		* e.g., JUtils.csvSplit("moe@stooges.com, larry@stooges.com, curly@stooges.com, shemp@stooges.com")
		*  returns the array ["moe@stooges.com", "larry@stooges.com", "curly@stooges.com", "shemp@stooges.com"].
		*/
		this.csvSplit = function( s_input ){

			// Armenian-style "cast": Make sure it's a String...
			s_input = "" + s_input;
		
			var a_input = s_input.split(",");
		
			for( var i = 0; i < a_input.length; i++ ){
				a_input[i] = a_input[i].trim();
			}
		
			return a_input;
		
		};/* csvSplit() */

		/* e.g., objectJoinIf( { "name" : "Moe", "number" : 1, "favorite_color" : null }, "&" )
		* returns "name=Moe&number=1"
		* if you supply if_func(), and output of if_func(array_element)
		* is true, adds that a_input[] element to the join.  If you do not supply
		* if_func(), adds that a_input[] element to the join if the element is
		* TRUEY.
		*/
		this.objectJoinIf = function( o_input, joinChars, if_func ){

			var iCount = 0;
			var sOut = "";

			for( var key in o_input ){
				var value = o_input[ key ];
				if(
					( ! if_func && value )
							||
					( if_func && if_func( value ) )
				){ 
					if( iCount > 0 ){
						sOut += "" + joinChars;	
					}
					sOut += "" + key + "=" + value;
					iCount++;
				}
			}

			return sOut;

		}; /* objectJoinIf() */

		this.objectSplit = function( s_input, splitExpr, objectInOut ){

			s_input = "" + s_input; // Armenian cast to a String...

			var mr_fields = s_input.split( splitExpr );

			if( ! objectInOut ){
				objectInOut = {};
			}

			for( var i = 0; i < mr_fields.length; i++ ){
				var s_key_value = mr_fields[i];
				var i_where = s_key_value.indexOf("=");
				if( i_where >= 0 ){
					var s_key = s_key_value.substring( 0, i_where );
					var s_value = s_key_value.substring( i_where + 1 );
					objectInOut[ s_key ] = s_value;
				}
			}

			return objectInOut;

		}; /* objectSplit() */


		/** Use for cookies expiration date...
		* (default expiration is when you close the browser...
		* ...as is often the case, default ain't so great)
		* @param date : defaults to new Date() i.e. "now",
		* @param days_in_future : defaults to 365...
		*/
		this.getCookieUtcTimeString = function( date, days_in_future ){

			date = date || new Date();

			days_in_future = days_in_future || 365;	

			var milli_seconds_per_day = 864000000; /* 24 * 60 * 60 * 1000 */

			var le_time = date.getTime();

			date.setTime( le_time + days_in_future * milli_seconds_per_day );

			return date.toUTCString();

		};

		this.debugPrint = function( s_msg ){
			if( this.debug ){
				console.log( s_msg );
			}
		};
	
	});

