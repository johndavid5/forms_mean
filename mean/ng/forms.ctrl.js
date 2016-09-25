angular.module('waldoApp')
.controller('FormsCtrl', function($scope, $interval, $window, $routeParams, ConfigSvc, FormsSvc, UtilsSvc, FormTypesSvc, SharedUtilsSvc ){

	var sWho = "FormsCtrl";

	$scope.debug_html = UtilsSvc.stringToBool( $routeParams.debug_html );

	$scope.entities_id_filter = $routeParams.entities_id;

	$scope.BASE_CSV_URL = "/api/forms?format=csv";

	$scope.csv_url = $scope.BASE_CSV_URL;

	$scope.refresh_csv_url = function(){

		var sWho = "$scope.refresh_csv_url";

		//$scope.csv_url = $scope.BASE_CSV_URL + "&entities_id_filter=" + $scope.entities_id_filter + "&accession_number_filter=" + $scope.accession_number_filter + "&form_type_filter=" + $scope.form_type_filter + "&date_filed_from_filter=" + $scope.date_filed_from_filter + "&date_filed_to_filter=" + $scope.date_filed_to_filter + "&orderBy=" + $scope.current_sort_by + "&ascDesc=" + $scope.current_asc_desc;
		$scope.csv_url = $scope.BASE_CSV_URL 
		+ "&accession_number_filter=" + $scope.accession_number_filter
		+ "&form_type_filter=" + $scope.form_type_filter
		+ "&date_filed_from_filter=" + $scope.date_filed_from_filter
		+ "&date_filed_to_filter=" + $scope.date_filed_to_filter
		+ "&filer_name_filter=" + $scope.filer_name_filter
		+ "&filer_cik_filter=" + $scope.filer_cik_filter
		+ "&orderBy=" + $scope.current_sort_by 
		+ "&ascDesc=" + $scope.current_asc_desc;

		console.log(sWho + "(): $scope.csv_url = \"" + $scope.csv_url + "\"...");	
	}; /* $scope.refresh_csv_url() */

	// Later use stylesheets for this mouseover...better yet, make a fake button that displays the basic gif...
	$scope.MOUSE_OUT_CSV_BTN_IMG_SRC = ConfigSvc.MOUSE_OUT_CSV_BTN_IMG_SRC;
	$scope.MOUSE_OVER_CSV_BTN_IMG_SRC = ConfigSvc.MOUSE_OVER_CSV_BTN_IMG_SRC;

	$scope.csv_btn_img_src = $scope.MOUSE_OUT_CSV_BTN_IMG_SRC;

	$scope.ARROW_UP_GRAY_BTN_IMG_SRC = ConfigSvc.ARROW_UP_GRAY_BTN_IMG_SRC;
	$scope.ARROW_UP_WHITE_BTN_IMG_SRC = ConfigSvc.ARROW_UP_WHITE_BTN_IMG_SRC;

	$scope.ARROW_DOWN_GRAY_BTN_IMG_SRC = ConfigSvc.ARROW_DOWN_GRAY_BTN_IMG_SRC;
	$scope.ARROW_DOWN_WHITE_BTN_IMG_SRC = ConfigSvc.ARROW_DOWN_WHITE_BTN_IMG_SRC;

	$scope.ARROW_TRANSPARENT_BTN_IMG_SRC = ConfigSvc.ARROW_TRANSPARENT_BTN_IMG_SRC;
			
	$scope.date_filed_from_filter = "";
	//$scope.date_filed_from_filter = SharedUtilsSvc.firstDayOfThisMonthAsString();
	//$scope.date_filed_from_filter = "2015-07-01"; // Hard code to July 1st, 2015 for now...perhaps use a more sophisticated algorithm later...
	$scope.edgarFileNameToEdgarSecFilingPageUrl = SharedUtilsSvc.edgarFileNameToEdgarSecFilingPageUrl;

	$scope.on_mouse_enter_csv_btn = function(){
		var sWho = "$scope.on_mouse_enter_csv_btn";
		//console.log(sWho + "()...");
		//console.log(sWho + "(): BEFORE: $scope.csv_btn_img_src = \"" + $scope.csv_btn_img_src + "\"...");
		$scope.csv_btn_img_src = $scope.MOUSE_OVER_CSV_BTN_IMG_SRC;
		//console.log(sWho + "(): AFTER: $scope.csv_btn_img_src = \"" + $scope.csv_btn_img_src + "\"...");
	};

	$scope.on_mouse_leave_csv_btn = function(){
		var sWho = "$scope.on_mouse_leave_csv_btn";
		//console.log(sWho + "()...");
		//console.log(sWho + "(): BEFORE: $scope.csv_btn_img_src = \"" + $scope.csv_btn_img_src + "\"...");
		$scope.csv_btn_img_src = $scope.MOUSE_OUT_CSV_BTN_IMG_SRC;
		//console.log(sWho + "(): BEFORE: $scope.csv_btn_img_src = \"" + $scope.csv_btn_img_src + "\"...");
	};

	$scope.columns = [
			{"heading": "Accession Number", "name": "accession_number", "width": "20%"},
			{"heading": "Form Type", "name": "form_type", "width": "15%"},
			{"heading": "Date Filed", "name": "date_filed", "width": "15%" },
			{"heading": "Company Name", "name": "dn_company_conformed_name", "width": "30%"},
			{"heading": "Company CIK", "name": "dn_company_central_index_key", "width": "20%"}
	];

	$scope.total_entries = 0;

	$scope.DEFAULT_ENTRIES_PER_PAGE = 100;

	$scope.entries_per_page = $scope.DEFAULT_ENTRIES_PER_PAGE;
	$scope.requested_entries_per_page = 100;

	$scope.current_page_num = 1;
	$scope.requested_page_num = 1;
	$scope.pending_page_num = 1;

	$scope.total_num_pages = 0;
	//$scope.max_pagination_pages = 7;
	//$scope.max_pagination_pages = 8;
	//$scope.max_pagination_pages = 9;
	$scope.max_pagination_pages = 10;
	$scope.pagination_pages_array = [];

	$scope.filter_text_array = [];

	$scope.pending_sort_by = "date_filed";
	$scope.pending_asc_desc = "desc";

	$scope.current_sort_by = "";
	$scope.current_asc_desc = "";

	$scope.current_mouse_on = "";

	$scope.accession_number_filter = "";

	$scope.form_type_filter = "";
	$scope.form_type_filter_count = 0;

	$scope.filer_name_filter = "";
	$scope.filer_cik_filter = "";
	

	// Private variable for "Jane, stop this crazy thing...!"
	var progress_interval_handle;
	var progress_reversing = false;

	var progress_start = 50;
	var progress_increment = 25;
	var progress_max = 100;

	$scope.progress_bar_active = false;

	//$scope.progress_bar_style = "circular";
	$scope.progress_bar_style = "back-and-forth";

	$scope.progress = 0;

	$scope.start_progress_bar = function( bManualAdvance ){

		// Don't start a new progress bar if we are already progressing...
		if ( angular.isDefined(progress_interval_handle) ){
				return;
		}

		$scope.progress_bar_active = true;
		$scope.progress = progress_start;

		if( ! bManualAdvance ){
			progress_interval_handle = $interval( function() {
				$scope.advance_progress_bar();
   		      }, 100 
			);
		}
	};



	$scope.advance_progress_bar = function(){

		var sWho = sWho + "::advance_progress_bar";

		if( $scope.progress_bar_style == "circular" ){

			//console.log(sWho + "(): BEFORE: $scope.progress = ", $scope.progress );

			var nextel = $scope.progress + progress_increment;

			if( nextel > progress_max ){
				nextel = 0;
			}

			$scope.progress = nextel;
	
			//console.log(sWho + "(): AFTER: $scope.progress = ", $scope.progress );

		}
		else {
			//console.log(sWho + "(): BEFORE: progress_reversing = ", progress_reversing, ", $scope.progress = ", $scope.progress );
	
			if( progress_reversing ){	
				$scope.progress -= progress_increment;
			}
			else {
				$scope.progress += progress_increment;
			}
	
			if($scope.progress >= progress_max) {
				progress_reversing = true;
			} else if ($scope.progress <= 0) {
				progress_reversing = false;
			}
	
			//console.log(sWho + "(): AFTER: progress_reversing = ", progress_reversing, ", $scope.progress = ", $scope.progress );
		}
	};

	$scope.stop_progress_bar = function(){
		if (angular.isDefined(progress_interval_handle)) {
			$interval.cancel(progress_interval_handle);
            progress_interval_handle = undefined;

			$scope.progress = 0;
			$scope.progress_bar_active = false;
		}
	};

	$scope.$on('$destroy', function() {
		// Make sure that the interval is destroyed too
		$scope.stop_progress_bar();
	});


	// Used for automatic initial fetcheroo...
	// ...maybe we can later re-factor the "filter", "sort", and "page"
	// fetch logic into this one method...
//	$scope.fetcheroo = function(){
//
//		var options = {"maxRows": 100, "withFilingsAttributesOnly": true };
//
//		// Don't forget your filtering options...add them to our options Object...
//		if( ! $scope.massage_and_validate_search_filters( options ) ){
//			console.log("WARNING: crummy filters on initial load.");
//			//return;
//		}
//
//		options.orderBy = $scope.current_sort_by;
//		options.ascDesc = $scope.current_asc_desc;
//
//		$scope.start_progress_bar();
//
//	    FormsSvc.fetch( options )
//		.success(function(filings){
//
//			console.log( sWho + "(): Got back filings...");
//
//			$scope.filings = filings;
//
//			$scope.stop_progress_bar();
//
//		});
//
//	}; /* fetcheroo() */

//	$scope.synch_date_filed_filter_to_dt = function(){
//
//		if( $scope.date_filed_from_filter_dt && $scope.date_filed_from_filter_dt instanceof Date ){
//			$scope.date_filed_from_filter = SharedUtilsSvc.formatDateObjectAsString($scope.date_filed_from_filter_dt);
//		}
//		else{
//			$scope.date_filed_from_filter = "";
//		}
//
//		if( $scope.date_filed_to_filter_dt && $scope.date_filed_to_filter_dt instanceof Date ){
//			$scope.date_filed_to_filter = SharedUtilsSvc.formatDateObjectAsString($scope.date_filed_to_filter_dt);
//		}
//		else{
//			$scope.date_filed_to_filter = "";
//		}
//	}

	$scope.massage_and_validate_search_filters = function( filterOptions ){

		$scope.accession_number_filter = UtilsSvc.isNull(  $scope.accession_number_filter, "" );

		$scope.form_type_filter = UtilsSvc.isNull(  $scope.form_type_filter, "" );
		$scope.form_type_filter = $scope.form_type_filter.toUpperCase();

		filterOptions.accession_number_filter = $scope.accession_number_filter;
		filterOptions.form_type_filter = $scope.form_type_filter;

		var outOptions = {};

		if( $scope.foolishly_attempt_to_use_angular_date_picker ){
			$scope.date_filed_from_filter = SharedUtilsSvc.formatDateObjectAsString( $scope.date_filed_from_filter_dt );
		}
		else {
			//$scope.date_filed_from_filter = UtilsSvc.isNull(  $scope.date_filed_from_filter, "" );

			if( ! $scope.date_filed_from_filter ){
				$scope.date_filed_from_filter = "";
			}
			$scope.date_filed_from_filter = $scope.date_filed_from_filter.trim();

			if( $scope.date_filed_from_filter.length != 0 ){
		
				if( ! SharedUtilsSvc.isDateStringValid( $scope.date_filed_from_filter, outOptions ) ){
					$window.alert("Date Filed From = '" + $scope.date_filed_from_filter + "' is not a valid date.");
					return false;
				}
				$scope.date_filed_from_filter = outOptions.formattedDateStringOut;
				$scope.date_filed_from_filter_dt = outOptions.dateObjectOut;
			}
		}
		

		/***********************************************************************/

		if( $scope.foolishly_attempt_to_use_angular_date_picker ){
			$scope.date_filed_to_filter = SharedUtilsSvc.formatDateObjectAsString( $scope.date_filed_to_filter_dt );
		}
		else {
			//$scope.date_filed_to_filter = UtilsSvc.isNull(  $scope.date_filed_to_filter, "" );
		
			if( ! $scope.date_filed_to_filter ){
				$scope.date_filed_to_filter = "";
			}
			$scope.date_filed_to_filter = $scope.date_filed_to_filter.trim();
	
			if( $scope.date_filed_to_filter.length != 0 ){
				if( ! SharedUtilsSvc.isDateStringValid( $scope.date_filed_to_filter, outOptions ) ){
					$window.alert("Date Filed To = '" + $scope.date_filed_to_filter + "' is not a valid date.");
					return false;
				}
				$scope.date_filed_to_filter = outOptions.formattedDateStringOut;
				$scope.date_filed_to_filter_dt = outOptions.dateObjectOut;
			}
		}

		console.log( sWho + "(): SHEMP: Before, Moe: $scope.date_filed_from_filter_dt = ", $scope.date_filed_from_filter_dt );
		console.log( sWho + "(): SHEMP: Before, Moe: $scope.date_filed_to_filter_dt = ", $scope.date_filed_to_filter_dt );

		console.log( sWho + "(): SHEMP: Before, Moe: $scope.date_filed_from_filter = ", $scope.date_filed_from_filter );
		console.log( sWho + "(): SHEMP: Before, Moe: $scope.date_filed_to_filter = ", $scope.date_filed_to_filter );

		if( $scope.date_filed_from_filter_dt instanceof Date 
			&& $scope.date_filed_to_filter_dt instanceof Date 
			&& $scope.date_filed_from_filter_dt.getTime() >  $scope.date_filed_to_filter_dt.getTime() )
		{
			console.log( sWho + "(): SHEMP: Sorry, Moe, looks like date_filed_from_filter_dt is greater than date_filed_to_filter_dt, I'm gonna have to swap 'em, Moe...");

			var le_swapperou;

			le_swapperou = $scope.date_filed_to_filter_dt;
			$scope.date_filed_to_filter_dt = $scope.date_filed_from_filter_dt;
			$scope.date_filed_from_filter_dt = le_swapperou;

			le_swapperou = $scope.date_filed_to_filter;
			$scope.date_filed_to_filter = $scope.date_filed_from_filter;
			$scope.date_filed_from_filter = le_swapperou;
		}

		console.log( sWho + "(): SHEMP: After, Moe: $scope.date_filed_from_filter_dt = ", $scope.date_filed_from_filter_dt );
		console.log( sWho + "(): SHEMP: After, Moe: $scope.date_filed_to_filter_dt = ", $scope.date_filed_to_filter_dt );

		console.log( sWho + "(): SHEMP: After, Moe: $scope.date_filed_from_filter = ", $scope.date_filed_from_filter );
		console.log( sWho + "(): SHEMP: After, Moe: $scope.date_filed_to_filter = ", $scope.date_filed_to_filter );

		filterOptions.date_filed_from_filter = $scope.date_filed_from_filter;
		filterOptions.date_filed_to_filter = $scope.date_filed_to_filter;

		//filterOptions.filing_agent_entity_name_varchar_filter = $scope.filing_agent_entity_name_varchar_filter;
		//filterOptions.filing_agent_cik_bigint_filter = $scope.filing_agent_cik_bigint_filter;
		filterOptions.filer_name_filter = $scope.filer_name_filter;
		filterOptions.filer_cik_filter = $scope.filer_cik_filter;

		filterOptions.entities_id_filter = $scope.entities_id_filter;

		return true;

	};/* $scope.massage_and_validate_search_filters() */

	$scope.requested_page_num_press = function( which, $event ){
		var sWho = "requested_page_num_press";
		console.log( sWho + "(): which = \"" + which + "\", \$event = ", $event , "...");
		console.log( sWho + "(): \$event.char = \"" + $event.char + "\", \$event.charCode=\"" + $event.charCode + "\"...");
		if( event.charCode == 13 ){
			// Looks like they hit the enter key...
			//$scope.go_to_page( which );
			$scope.search();
		}
	};

	$scope.requested_entries_per_page_press = function( which, $event ){
		var sWho = "requested_entries_per_page_press";
		console.log( sWho + "(): which = \"" + which + "\", \$event = ", $event , "...");
		console.log( sWho + "(): \$event.char = \"" + $event.char + "\", \$event.charCode=\"" + $event.charCode + "\"...");
		if( event.charCode == 13 ){
			// Looks like they hit the enter key...
			//$scope.go_to_page( which );
			$scope.search();
		}
	};

	$scope.go_to_page = function( which, $event ){

		var sWho = "go_to_page";

		console.log( sWho + "(): which = \"" + which + "\", \$event = ", $event , "...");

		var pending_page_num;

		if( which == "first" ){
			pending_page_num = 1;
		}
		else if( which == "previous" ){
			pending_page_num = $scope.current_page_num-1;
		}
		else if( which == "next" ){
			pending_page_num = $scope.current_page_num+1;
		}
		else if( which == "last" ){
			pending_page_num = $scope.total_num_pages;
		}
		else {
			// Assume it's a page number...later, validate that it's numeric...
			pending_page_num = which;
		}

		if( pending_page_num <= 1 ){	
			pending_page_num = 1;
		}
		else if( pending_page_num >= $scope.total_num_pages ){	
			pending_page_num = $scope.total_num_pages;
		}

		$scope.requested_page_num = pending_page_num;

		$scope.search();

	}; /* $scope.go_to_page() */

	$scope.get_pagination_class_for = function(what){
		if( what == "previous" || what == "first" ){
			if( $scope.current_page_num <= 1 ){
				return "disabled";
			}
			else {
				return "";
			}
		}
		else if( what == "next" || what == "last" ){
			if( $scope.current_page_num >= $scope.total_num_pages ){
				return "disabled";
			}
			else {
				return "";
			}
		}
		else {
			// Assume it's a page number...
			if( what == $scope.current_page_num ){
				return "active";
			}
			else if( what == $scope.pending_page_num ){
				return "pending";
			}
			else {
				return "";
			}
		}
	}; /* $scope.get_pagination_class_for */

	/* Returns the array used to display the paginator...
	*  e.g., "1", "2", "3", "4", "5"
	*  or    "6", "7", "8", "9", "10"
	*/
	$scope.get_pagination_pages_array = function(){

		var sWho = "$scope.get_pagination_pages_array";


		console.log(sWho + "(): $scope.max_pagination_pages = ", $scope.max_pagination_pages, "...");
		console.log(sWho + "(): $scope.pending_page_num = ", $scope.pending_page_num, "...");
		console.log(sWho + "(): $scope.total_num_pages = ", $scope.total_num_pages, "...");

		return UtilsSvc.get_pagination_pages_array( $scope.max_pagination_pages, $scope.pending_page_num, $scope.total_num_pages );

	}; /* $scope.get_pagination_pages_array() */

	$scope.insanity_filter_entries_per_page = function( entries_per_page ){
		if( ! SharedUtilsSvc.isIntegerValid( entries_per_page ) ){
			return $scope.DEFAULT_ENTRIES_PER_PAGE
		}
		else if( entries_per_page <= 0 ){
			return $scope.DEFAULT_ENTRIES_PER_PAGE
		}
		else if( entries_per_page > ConfigSvc.MAX_ENTRIES_PER_PAGE ){
			return ConfigSvc.MAX_ENTRIES_PER_PAGE;
		}
		else{
			return entries_per_page;
		}
	}

	$scope.insanity_filter_requested_page_num = function( page_num ){
		if( ! SharedUtilsSvc.isIntegerValid( page_num ) ){
			return 1;
		}
		else if( page_num <= 0 ){
			return 1;
		}
		else{
			return page_num;
		}
	}

	/* Use this as the single "load" point 
    * for "Search", "Sort" and "Paginate"...
	*/
	$scope.search = function($event){

		var sWho = "search";

		console.log( sWho + "()...");

		if( $event ){
			$event.preventDefault();
			$event.stopPropagation();
		}

		$scope.form_type_picker_opened = false;

		//var options = {"maxRows": 100, "withFilingsAttributesOnly": true };
		var options = {};

		if( ! $scope.massage_and_validate_search_filters( options ) ){
			return;
		}

		$scope.requested_entries_per_page = $scope.insanity_filter_entries_per_page( $scope.requested_entries_per_page );
		$scope.pending_entries_per_page = $scope.requested_entries_per_page;

		$scope.requested_page_num = $scope.insanity_filter_requested_page_num( $scope.requested_page_num );
		$scope.pending_page_num = $scope.requested_page_num;

		// Don't forget your sort options...
		options.orderBy = $scope.pending_sort_by;
		options.ascDesc = $scope.pending_asc_desc;

   		$scope.start_progress_bar();

		// Later on, adjust logic so we only re-fetch the count if the search filters have changed...
		FormsSvc.fetch_count( options )
		.success(function(arrayOut){

			console.log( sWho + "(): Got back arrayOut = ", arrayOut, "..." );

			// Expecting something like arrayOut = [{"count":970}]...array of one element... 
			if( arrayOut instanceof Array ){
				if( arrayOut.length != 0 ){
					$scope.total_entries = arrayOut[0].count;
					console.log( sWho + "(): Just set \$scope.total_entries = ", $scope.total_entries, ", Escamillo...");

					//$scope.total_num_pages = SharedUtilsSvc.getNumPages( $scope.total_entries, $scope.entries_per_page );
					$scope.total_num_pages = SharedUtilsSvc.getNumPages( $scope.total_entries, $scope.pending_entries_per_page );
					console.log( sWho + "(): Just set \$scope.total_num_pages = ", $scope.total_num_pages, ", Escamillo...");

					// Total number of pages may have changed, adjust $scope.pending_page_num if it's > $scope.total_num_pages...
					if( $scope.pending_page_num > $scope.total_num_pages ){
						console.log( sWho + "(): Sorry, Moe, we need to adjust too-high \$scope.pending_page_num: BEFORE: ", $scope.pending_page_num, "...");
						$scope.pending_page_num = $scope.total_num_pages;
						console.log( sWho + "(): Sorry, Moe, we need to adjust too-high \$scope.pending_page_num: AFTER: ", $scope.pending_page_num, "...");
					}

					$scope.pagination_pages_array = $scope.get_pagination_pages_array();
				}
				else {
					console.log( sWho + "(): WARNING: arrayOut is an Array, but its length is zero...");
				}
			}
			else {
				console.log( sWho + "(): WARNING: arrayOut is not an Array, expected an Array...");
			}

			
			var rangeOutOptions = {};
			//SharedUtilsSvc.getRangesForPage( $scope.total_entries, $scope.entries_per_page, $scope.pending_page_num, rangeOutOptions ); 
			SharedUtilsSvc.getRangesForPage( $scope.total_entries, $scope.pending_entries_per_page, $scope.pending_page_num, rangeOutOptions ); 
			console.log( sWho + "(): SharedUtilSvc.getRangesForPage( totalEntries = ", $scope.total_entries , ", entriesPerPage = ", $scope.pending_entries_per_page, ", pageNumber = ", $scope.pending_page_num, " ) yielded rangeOutOptions = ", rangeOutOptions, "...");

			options.lowRow = rangeOutOptions.lowEntryOut;
			options.highRow = rangeOutOptions.highEntryOut;
			//options.rowOffset = rangeOutOptions.offsetOut;
			//options.rowLimit = rangeOutOptions.limitOut;

   			FormsSvc.fetch( options )
			.success(function(filings){

				console.log( sWho + "(): Got back filings...");

				if( filings instanceof Array ){
					console.log( sWho + "(): filings is an Array with filings.length = " + filings.length );
				}
				else {
					console.log( sWho + "(): WARN: filings is NOT an Array, we expected an Array...");
				}

				$scope.filter_text_array = $scope.create_filter_text_array( options ); 

				$scope.entries_per_page = $scope.pending_entries_per_page;
				$scope.requested_entries_per_page = $scope.entries_per_page;

				$scope.current_page_num = $scope.pending_page_num;
				$scope.requested_page_num = $scope.current_page_num;

				$scope.current_sort_by = $scope.pending_sort_by;
				$scope.current_asc_desc = $scope.pending_asc_desc;

				$scope.filings = filings;

				navStickyTableHeaderInit(); // in nav.js	

	   			$scope.stop_progress_bar();

			});
		});

	}; /* search() */

	$scope.create_filter_text_array = function( options ){

		var sWho = "create_filter_text_array";

		var aOutput = [];

		console.log( sWho + "(): options = ", options );

		if( options.accession_number_filter ){
			aOutput.push("Accession Number:\"*" + options.accession_number_filter + "*\""); 	
		}

		if( options.form_type_filter ){
			aOutput.push("Form Types: \"" + options.form_type_filter + "\""); 	
		}

		if( options.date_filed_from_filter ){
			aOutput.push("Date Filed From: \"" + options.date_filed_from_filter + "\""); 	
		}

		if( options.date_filed_to_filter ){
			aOutput.push("Date Filed To: \"" + options.date_filed_to_filter + "\""); 	
		}

		//if( options.filing_agent_entity_name_varchar_filter ){
		//	aOutput.push("Filing Agent: \"*" + options.filing_agent_entity_name_varchar_filter + "*\""); 	
		//}
		
		if( options.filer_name_filter ){
			aOutput.push("Filer Name: \"*" + options.filer_name_filter + "*\""); 	
		}

		if( options.filer_cik_filter ){
			aOutput.push("Filer CIK: \"*" + options.filer_cik_filter + "*\""); 	
		}

		if( options.entities_id_filter ){
			aOutput.push("Entities ID: \"" + options.entities_id_filter + "\""); 	
		}

		console.log( sWho + "(): aOutput = ", aOutput, "...");

		return aOutput;

		//var sOut = aOutput.join(", ");
		//
		//var sOut = "";
		//for( var i = 0; i < aOutput.length; i++ ){
		//	if( i > 0 ){
		//		sOut += ", ";
		//	}
		//
		//	sOut +="<span style=\"white-space: nowrap;\">" + aOutput[i] + "</span>"; 
		//}
		//console.log( sWho + "(): Returning sOut = \"" + sOut + "\"...");
		//return sOut;

	}/* create_filter_text_array() */


	$scope.sortBy = function( sColumnName ){
		var sWho = "sortBy";

		console.log( sWho + "(): sColumnName = '" + sColumnName + "'...");

		console.log( sWho + "(): Hey, Moe: \$scope.current_sort_by = '" + $scope.current_sort_by + "'...");
		console.log( sWho + "(): Hey, Moe: \$scope.current_asc_desc = '" + $scope.current_asc_desc + "'...");

		if( $scope.current_sort_by == sColumnName ){
			// Same column as last sort, so flip asc_desc...
			$scope.pending_asc_desc = $scope.flip_asc_desc( $scope.current_asc_desc );
			$scope.pending_sort_by = $scope.current_sort_by;
		}
		else {
			// Different column from last sort, so set to asc_desc to "asc"
			$scope.pending_asc_desc = "asc"
			$scope.pending_sort_by = sColumnName;
		}

		$scope.search();

	};/* $scope.sortBy() */


	$scope.flip_asc_desc = function( sAscDesc ){
		if( sAscDesc == "asc" ){
			return "desc";
		}
		else if( sAscDesc == "desc" ){
			return "asc";
		}
		else {
			return "???";
		}
	};



	$scope.on_mouse_enter_col = function( sColumnName ){
		$scope.current_mouse_on = sColumnName;
	};

	$scope.on_mouse_leave_col = function( sColumnName ){
		$scope.current_mouse_on = "";
	};

	$scope.get_arrow_img_src = function( sColumnName ){
		if( $scope.current_sort_by == sColumnName ){
			if( $scope.current_asc_desc == "asc" ){
				if( $scope.current_mouse_on == sColumnName ){
					return $scope.ARROW_UP_GRAY_BTN_IMG_SRC;
					//return "/images/arrow_up.40x34.gray.gif";
				}
				else {
					return $scope.ARROW_UP_WHITE_BTN_IMG_SRC;
					//return "/images/arrow_up.40x34.white.gif";
				}
			}
			else {
				if( $scope.current_mouse_on == sColumnName ){
					return $scope.ARROW_DOWN_GRAY_BTN_IMG_SRC;
					//return "/images/arrow_down.40x34.gray.gif";
				}
				else {
					return $scope.ARROW_DOWN_WHITE_BTN_IMG_SRC;
					//return "/images/arrow_down.40x34.white.gif";
				}
			}
		}
		else {
			//return "/images/transparent.40x34.gif";
			return $scope.ARROW_TRANSPARENT_BTN_IMG_SRC;
		}
	};

	$scope.get_asc_desc_class_for_column = function( sColumnName ){
		if( $scope.current_sort_by == sColumnName ){
			return current_asc_desc;
		}
		else {
			return "transparent";
		}
	};

	$scope.get_asc_for_column = function( sColumnName ){
		if( sColumnName == $scope.current_sort_by ){	
			return $scope.current_asc_desc;		
		}
		else {
			return "";
		}
	};

	// <DATE_PICKER_SHTUFF>

	// Disable weekend selection for datepicker...
	$scope.is_date_disabled = function(date, mode) {
		//return ( mode === 'day' && ( date.getDay() === 0 || date.getDay() === 6 ) );
		return false;
	};

	$scope.open_close_date_picker = function($event, which) {

		$event.preventDefault();
		$event.stopPropagation();

		$scope['date_filter_'+which+'_opened'] = ! $scope['date_filter_'+which+'_opened']; 

		//if( $("#" + which + ).datepicker( "widget" ).is(":visible") ){
		//	$("#" + which + ).datepicker('hide');
		//}
		//else {
		//	$("#" + which + ).datepicker('show');
		//}
	};

	$scope.dateOptions = {
		formatYear: 'yy',
		startingDay: 0, // Sunday
		showWeeks: false, // Show week number...
	};

	$scope.date_format = 'yyyy-MM-dd';

	// <Form_Types_Shtuff>
	$scope.form_type_picker_opened = false;

	$scope.form_types_un_selected =  []; // use for picker, fill-in later via FormTypes service...

//	$scope.form_types_un_selected =  [
//		"S-1/A",
//		"424B2",
//		"F-X",
//		"EFFECT",
//	];

//	$scope.form_types_un_selected =  [
//		"S-1/A",
//		"424B2",
//		"F-X",
//		"EFFECT",
//		"F-N/A"  ,
//		"10-Q"  ,
//		"X-17A-5"  ,
//		"F-N"  ,
//		"40-17G"  ,
//		"10-K"  ,
//		"20-F"  ,
//		"8-K"  ,
//		"10-Q/A"  ,
//		"DEF 14A"  ,
//		"424B5"  ,
//		"DEFA14A"  ,
//		"N-30B-2"  ,
//		"F-3/A"  ,
//		"S-4"  ,
//		"SC 13D/A"  ,
//		"FWP"  ,
//		"20-F/A"  ,
//		"D"  ,
//		"424B3"  ,
//		"F-4/A"  ,
//		"F-3"  ,
//		"497J"  ,
//		"6-K"  ,
//		"10-D"  ,
//		"485BPOS"  ,
//		"497"  ,
//		"10-K/A"  ,
//		"4"  
//	]; 

//	$scope.form_types_un_selected.sort();

	$scope.form_types_selected =  [];

	/**
	* POSTCONDITION:
	* Elements found in $scope.form_type_filter will also be found in $scope.form_types_selected.
	* Elements NOT found in $scope.form_type_filter will also NOT be found in $scope.
	*
	* options.toTheLast: by default, ignore the LAST entry, since it may be a partially typed
	* in form...
	*/
	$scope.synch_selected_lists_with_form_type_filter = function(options){

		var sWho = "synch_selected_lists_with_form_type_filter";

		console.log(sWho + "(): options = ", options, "...");

		var bToTheLast = false;
		if(options && options.toTheLast ){
			bToTheLast = true;
		}

		console.log(sWho + "(): bToTheLast = ", bToTheLast, "..." );
		

		$scope.form_type_filter = $scope.form_type_filter.toUpperCase();
		var a_form_type_filter = $scope.form_type_filter.split(",");	

		console.log(sWho + "(): GEORGE TAKEI: Oh, my...a_form_type_filter.length = " + a_form_type_filter.length + "...");

		var i_max = a_form_type_filter.length-1;

		if( bToTheLast ){
			i_max = a_form_type_filter.length;
		}

		for( var i=0; i < i_max; i++ ){

			a_form_type_filter[i] = a_form_type_filter[i].trim();

			console.log(sWho + "(): a_form_type_filter[" + i + "] = \"" + a_form_type_filter[i] + "\"...");

			if( a_form_type_filter[i].length == 0 ){
				// Ignore if it's a blankee...
				continue;
			}

			var i_where;

			if( ( i_where = $scope.form_types_un_selected.indexOf( a_form_type_filter[i] ) ) >= 0 ){
				console.log(sWho + "(): Found it at element " + i_where + " of $scope.form_types_un_selected, so removing from that location...");
				$scope.form_types_un_selected.splice(i_where, 1); // Remove from un-selected if found...
			}

			if( ( i_where = $scope.form_types_selected.indexOf( a_form_type_filter[i] ) ) < 0 ){
				console.log(sWho + "(): Did NOT find it in $scope.form_types_selected, so pushing it onto...");
				$scope.form_types_selected.push( a_form_type_filter[i] );
			}
		}

		$scope.form_types_un_selected.sort();
	}; /* $scope.synch_selected_lists_with_form_type_filter() */

	$scope.close_form_type_picker = function($event) {

		var sWho = "$scope.close_form_type_picker";

		console.log(sWho + "(): $scope.form_type_picker_opened = '" + $scope.form_type_picker_opened + "'...");
		console.log(sWho + "(): $event = ", event );

		//$event.preventDefault();
		//$event.stopPropagation();

		//$scope.form_type_picker_opened = false;

	}; /* $scope.close_form_type_picker() */

	$scope.open_close_form_type_picker = function($event) {

		var sWho = "$scope.open_close_form_type_picker";

		console.log(sWho + "(): $scope.form_type_picker_opened = '" + $scope.form_type_picker_opened + "'...");

		$event.preventDefault();
		$event.stopPropagation();

		if( ! $scope.form_type_picker_opened ){
			// If we're opening it now, let's synchronize the lists...
			console.log(sWho + "(): Oh, my....calling $scope.synch_selected_lists_with_form_type_filter()...");
			$scope.synch_selected_lists_with_form_type_filter({"toTheLast": true});
		}

		$scope.form_type_picker_opened = ! $scope.form_type_picker_opened;

	}; /* $scope.open_close_form_type_picker() */

	/** GEORGE TAKEI: Oh, my....we'd better keep an eye or two on the form_type_filter...
	*/
	$scope.$watch( 'form_type_filter',
		function(value){
			console.log("GEORGE TAKEI: Oh, my..., form_type_filter has changed, the NEW value is \"" + value + "\"...");
			$scope.update_form_type_filter_count();
			$scope.synch_selected_lists_with_form_type_filter();
		}
	);

//	$scope.$watch( 'date_filed_from_filter_dt',
//		function(value){
//			console.log("GEORGE TAKEI: Oh, my..., date_filed_from_filter_dt has changed...");
//			$scope.synch_date_filed_filter_to_dt();
//		}
//	);
//
//	$scope.$watch( 'date_filed_to_filter_dt',
//		function(value){
//			console.log("GEORGE TAKEI: Oh, my..., date_filed_to_filter_dt has changed...");
//			$scope.synch_date_filed_filter_to_dt();
//		}
//	);

	$scope.update_form_type_filter_count = function() {

		sWho = "update_form_type_filter_count";

		//console.log(sWho + "(): $scope.form_type_filter = \"" + $scope.form_type_filter + "\"...");
		$scope.form_type_filter = $scope.form_type_filter.toUpperCase();
		var a_splitted = $scope.form_type_filter.split(","); 

		//console.log(sWho + "(): BEFORE PROCESSING: a_splitted = ", a_splitted );
		//console.log(sWho + "(): BEFORE PROCESSING: a_splitted.length = ", a_splitted.length );

		for( var i = a_splitted.length-1; i >= 0; i-- ){
			if( a_splitted[i].trim().length == 0 ){
				a_splitted.splice(i, 1); // Remove blankee element...
			}
		}

		//console.log(sWho + "(): AFTER PROCESSING: a_splitted = ", a_splitted );
		//console.log(sWho + "(): AFTER PROCESSING: a_splitted.length = ", a_splitted.length );

		$scope.form_type_filter_count = a_splitted.length;
	};

	// </Form_Types_Shtuff>

	FormTypesSvc.fetch()
	.success(function(formTypes){
		console.log("Got back formTypes = ", formTypes );
		if( formTypes instanceof Array ){

			$scope.form_types_un_selected =  [];
			var i;
			for(i=0; i < formTypes.length; i++ ){ 	
				//$scope.form_types_un_selected.push( formTypes[i].form_type_varchar ); 
				$scope.form_types_un_selected.push( formTypes[i] );
			}
			$scope.form_types_un_selected.sort();
		}

		if( ConfigSvc.AUTO_SEARCH ){
			// Perform an initial automatic fetcheroo...or maybe 
			// don't do this and wait for the user to click on
			// something...?
			$scope.search();
		}

	});


});

