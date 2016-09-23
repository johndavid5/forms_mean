angular.module('waldoApp')
.service('FormTypesSvc', function($http, UtilsSvc){

	// Later try using the $resource service in ng-resource.js if you don't
	// want to be macho-wacho and use the "raw" $http service...
	this.fetch = function(options){
		var sWho = "FormTypesSvc::fetch";

		console.log( sWho + "(): options = \"" + JSON.stringify( options ) + "\"...");

		var url =  '/api/formTypes' + UtilsSvc.propertiesToQueryString(options);

		console.log( sWho + "(): Returning $http.get( \"" + url + "\" )...");

		return $http.get( url );
	}; /* this.fetch() */

});
