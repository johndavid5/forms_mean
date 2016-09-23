angular.module('waldoApp')
.filter('VfEllipsisFilter', function(){
  return function(input, le_width, le_ellipsis) {

	le_width = le_width || 20; 
	le_ellipsis = le_ellipsis || '...'; 
    input = input || '';

	input = input.trim();

	if( input.length <= le_width ){
		return input;
	}
	else{
		return input.substring(0,le_width-le_ellipsis.length) + le_ellipsis;
	}
  };
});
