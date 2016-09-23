angular.module('waldoApp')
.filter('VfLeftPadFilter', function(){

  return function(input, le_width, le_char, le_default) {
		le_width = le_width || 10; // Standard CIK width...
		le_char = le_char || '0'; // default left pad with zeroes...

		input = input || '';

	    var output = "" + input; // coerce to string

		output = output.trim();

		if( output.length == 0 ){
			// Don't bother to zero-pad if it's an empty string...
			return le_default;
		}

	    while( output.length < le_width ){
			output = le_char + output;
		}
		return output;
	};

});
