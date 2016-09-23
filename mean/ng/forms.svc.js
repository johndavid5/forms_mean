angular.module('waldoApp')
.service('FormsSvc', function($http, UtilsSvc){

	// Later try using the $resource service in ng-resource.js if you don't
	// want to be macho-wacho and use the "raw" $http service...
	this.fetch = function(options){
		var sWho = "FormsSvc::fetch";

		console.log( sWho + "(): options = \"" + JSON.stringify( options ) + "\"...");

		// If we're gonna muck with the options, we'd better muck 
		// with a cloned copy of 'em, or else we'll muck up the caller's
		// options Object...
		var ourOptions = angular.copy( options ); 
		ourOptions.getFilingAgent = true;
		//ourOptions.withFilingAttributesOnly = true;

		console.log( sWho + "(): ourOptions = \"" + JSON.stringify( ourOptions ) + "\"...");

		var url =  '/api/forms' + UtilsSvc.propertiesToQueryString(ourOptions);

		console.log( sWho + "(): Returning $http.get( \"" + url + "\" )...");

		return $http.get( url );
	}; /* this.fetch() */

	this.fetch_count = function(options){

		var sWho = "FormsSvc::fetch_count";

		console.log( sWho + "(): options = \"" + JSON.stringify( options ) + "\"...");

		// If we're gonna muck with the options, we'd better muck 
		// with a cloned copy of 'em, or else we'll muck up the caller's
		// options Object...
		var ourOptions = angular.copy( options ); 
		ourOptions.countOnly = true;
		ourOptions.getFilingAgent = true;

		console.log( sWho + "(): ourOptions = \"" + JSON.stringify( ourOptions ) + "\"...");

		var url =  '/api/forms' + UtilsSvc.propertiesToQueryString(ourOptions);

		console.log( sWho + "(): Returning $http.get( \"" + url + "\" )...");

		return $http.get( url );

	}; /* this.fetch_count() */


	this.associate_people_with_filing = function(options){

		var sWho = "FormsSvc::associate_people_with_filing";

		console.log( sWho + "(): options = \"" + JSON.stringify( options ) + "\"...");

		var ourOptions = angular.copy( options ); 
		ourOptions.action = "associatePeopleWithFiling";

		console.log( sWho + "(): ourOptions = \"" + JSON.stringify( ourOptions ) + "\"...");

		var url =  '/api/forms';

		console.log( sWho + "(): Returning $http.post( url = \"" + url + "\", body = ", ourOptions, " )...");

		return $http.post( url, ourOptions );
	};

	this.dis_associate_people_with_filing = function(options){

		var sWho = "FormsSvc::dis_associate_people_with_filing";

		var ourOptions = angular.copy( options ); 
		ourOptions.action = "disAssociatePeopleWithFiling";

		console.log( sWho + "(): ourOptions = \"" + JSON.stringify( ourOptions ) + "\"...");

		var url =  '/api/forms';

		console.log( sWho + "(): Returning $http.post( url = \"" + url + "\", body = ", ourOptions, " )...");

		return $http.post( url, ourOptions );
	};

	var self_filed_options = [
			{ "le_value" : "", "le_window_dressing" : "---" },	
			{ "le_value" : "yes", "le_window_dressing" : "Yes"},	
			{ "le_value" : "no", "le_window_dressing" : "No"},	
	];

	SELF_FILED_OPTIONS_WHATEVER_INDEX=0;
	SELF_FILED_OPTIONS_YES_INDEX=1;
	SELF_FILED_OPTIONS_NO_INDEX=2;

	this.get_self_filed_options = function(){
		return self_filed_options;
	};

	this.SELF_FILED_OPTIONS_WHATEVER_INDEX = function(){
		return SELF_FILED_OPTIONS_WHATEVER_INDEX;
	}

	this.SELF_FILED_OPTIONS_YES_INDEX = function(){
		return SELF_FILED_OPTIONS_YES_INDEX;
	}

	this.SELF_FILED_OPTIONS_NO_INDEX = function(){
		return SELF_FILED_OPTIONS_NO_INDEX;
	}

	//this.create = function(filing){
	//	return $http.post('/api/forms', filing);
	//}
});
