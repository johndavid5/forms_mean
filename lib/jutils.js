//String.prototype.lpad = function(padString, length) {
//    var str = this;
//    while (str.length < length){
//        str = padString + str;
//	}
//    return str;
//};

//global.JUtils = {
var JUtils = {

	objectToString: function (o, args) {

		var withTypes = false;
		if( args && args.withTypes )
		{
			withTypes = true;	
		}

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
				val=JUtils.objectToString(o[i], args);
	
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
				  var rhs = JUtils.objectToString(o[property], args);
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
					//output += "" + "\n";
				}
		
				output += "" + s_own_property + property + ": " + rhs;
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
},

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
}

}/* var JUtils  */

module.exports = JUtils
