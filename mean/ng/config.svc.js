angular.module('waldoApp')
.service('ConfigSvc', function(UtilsSvc, $cookies){
	
	this.MAX_ENTRIES_PER_PAGE = 1000;

	this.MOUSE_OUT_CSV_BTN_IMG_SRC = "/images/csv.button.30x30.gif";
	this.MOUSE_OVER_CSV_BTN_IMG_SRC = "/images/csv.button.gray.30x30.gif";

	this.CSV_IMG_SRC = "/images/csv.gif";

	this.ARROW_UP_GRAY_BTN_IMG_SRC = "/images/arrow_up.40x34.gray.gif";
	this.ARROW_UP_WHITE_BTN_IMG_SRC = "/images/arrow_up.40x34.white.gif";

	this.ARROW_DOWN_GRAY_BTN_IMG_SRC = "/images/arrow_down.40x34.gray.gif";
	this.ARROW_DOWN_WHITE_BTN_IMG_SRC = "/images/arrow_down.40x34.white.gif";

	this.ARROW_TRANSPARENT_BTN_IMG_SRC = "/images/transparent.40x34.gif";

	// Automatically do a search when you go to a summary screen, before you hit the "Search" button...
	this.AUTO_SEARCH = true;


	this.setSearchScreenSearchUrl = function( searchScreenSearchUrl ){
		var sWho = "ConfigSvc::setSearchScreenSearchUrl";
		var options = {};
		options.expires = UtilsSvc.getCookieUtcTimeString(); 
		//console.log(sWho + "(): GILLIGAN: Skipper, calling $cookies.put( \"searchScreenSearchUrl\", \"" + searchScreenSearchUrl + "\", options=", options , " )...");
		$cookies.put("searchScreenSearchUrl", searchScreenSearchUrl, options );
	};

	this.getSearchScreenSearchUrl = function(){
		return $cookies.get("searchScreenSearchUrl");
	};

	this.setFilingsScreenSearchUrl = function( FilingsScreenSearchUrl ){
		var sWho = "ConfigSvc::setFilingsScreenSearchUrl";
		var options = {};
		options.expires = UtilsSvc.getCookieUtcTimeString(); 
		//console.log(sWho + "(): GILLIGAN: Skipper, calling $cookies.put( \"FilingsScreenSearchUrl\", \"" + FilingsScreenSearchUrl + "\", options=", options , " )...");
		$cookies.put("FilingsScreenSearchUrl", FilingsScreenSearchUrl, options );
	};

	this.getFilingsScreenSearchUrl = function(){
		return $cookies.get("FilingsScreenSearchUrl");
	};

	this.setEntitiesScreenSearchUrl = function( EntitiesScreenSearchUrl ){
		var sWho = "ConfigSvc::setEntitiesScreenSearchUrl";
		var options = {};
		options.expires = UtilsSvc.getCookieUtcTimeString(); 
		//console.log(sWho + "(): GILLIGAN: Skipper, calling $cookies.put( \"EntitiesScreenSearchUrl\", \"" + EntitiesScreenSearchUrl + "\", options=", options , " )...");
		$cookies.put("EntitiesScreenSearchUrl", EntitiesScreenSearchUrl, options );
	};

	this.getEntitiesScreenSearchUrl = function(){
		return $cookies.get("EntitiesScreenSearchUrl");
	};

	this.setPeopleScreenSearchUrl = function( PeopleScreenSearchUrl ){
		var sWho = "ConfigSvc::setPeopleScreenSearchUrl";
		var options = {};
		options.expires = UtilsSvc.getCookieUtcTimeString(); 
		//console.log(sWho + "(): GILLIGAN: Skipper, calling $cookies.put( \"PeopleScreenSearchUrl\", \"" + PeopleScreenSearchUrl + "\", options=", options , " )...");
		$cookies.put("PeopleScreenSearchUrl", PeopleScreenSearchUrl, options );
	};

	this.getPeopleScreenSearchUrl = function(){
		return $cookies.get("PeopleScreenSearchUrl");
	};

	this.setCompaniesScreenSearchUrl = function( CompaniesScreenSearchUrl ){
		var sWho = "ConfigSvc::setCompaniesScreenSearchUrl";
		var options = {};
		options.expires = UtilsSvc.getCookieUtcTimeString(); 
		//console.log(sWho + "(): GILLIGAN: Skipper, calling $cookies.put( \"CompaniesScreenSearchUrl\", \"" + CompaniesScreenSearchUrl + "\", options=", options , " )...");
		$cookies.put("CompaniesScreenSearchUrl", CompaniesScreenSearchUrl, options );
	};

	this.getCompaniesScreenSearchUrl = function(){
		return $cookies.get("CompaniesScreenSearchUrl");
	};

});
