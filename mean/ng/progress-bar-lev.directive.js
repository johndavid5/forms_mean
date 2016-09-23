angular.module('waldoApp')
.directive('vfProgressBarLev', ['$window', '$interval', 'UtilsSvc', 'SharedUtilsSvc', 'AddressesSvc', function($window, $interval, UtilsSvc, SharedUtilsSvc, AddressesSvc) {

	var sOuterWho = "vfProgressBarLev";	

	var ourController = function($scope,$element,$attrs){

		var sInnerWho = sOuterWho + "::ctrl";

		var sWho = sInnerWho;

		console.log(sWho + "(): this =", this );	
		console.log(sWho + "(): $scope =", $scope );	
		console.log(sWho + "(): $element =", $element );	
		console.log(sWho + "(): $attrs =", $attrs);
		
		// Make "this" available to nested anonymous functions...as private "closure" variable...
		var ctrl = this;	

		console.log(sWho + "(): ctrl.debugHtml = ", ctrl.debugHtml );

		// Private variable for "Jane, stop this crazy thing...!"
		var progress_interval_handle;
		var progress_reversing = false;

		var progress_start = 50;
		var progress_increment = 25;
		var progress_max = 100;

		ctrl.left_progress_percent = "0%";
		ctrl.right_progress_percent = "100%";

		ctrl.progress_bar_active = false;

		//ctrl.progress_bar_style = "circular";
		ctrl.progress_bar_style = "back-and-forth";

		ctrl.progress = 0;

		ctrl.start_progress_bar = function( bManualAdvance ){

			// Don't start a new progress bar if we are already progressing...
			if ( angular.isDefined(progress_interval_handle) ){
					return;
			}

			ctrl.progress_bar_active = true;
			ctrl.progress = progress_start;
			ctrl.updateProgressPercents();

			if( ! bManualAdvance ){
				progress_interval_handle = $interval( function() {
					ctrl.advance_progress_bar();
   			      }, 100 
				);
			}
		};

		ctrl.advance_progress_bar = function(){

			var sWho = sWho + "::advance_progress_bar";

			if( ctrl.progress_bar_style == "circular" ){
	
				//console.log(sWho + "(): BEFORE: ctrl.progress = ", ctrl.progress );
	
				var nextel = ctrl.progress + progress_increment;
	
				if( nextel > progress_max ){
					nextel = 0;
				}
	
				ctrl.progress = nextel;
		
				//console.log(sWho + "(): AFTER: ctrl.progress = ", ctrl.progress );
	
			}
			else {
				//console.log(sWho + "(): BEFORE: progress_reversing = ", progress_reversing, ", ctrl.progress = ", ctrl.progress );
		
				if( progress_reversing ){	
					ctrl.progress -= progress_increment;
				}
				else {
					ctrl.progress += progress_increment;
				}
		
				if(ctrl.progress >= progress_max) {
					progress_reversing = true;
				} else if (ctrl.progress <= 0) {
					progress_reversing = false;
				}

				//console.log(sWho + "(): AFTER: progress_reversing = ", progress_reversing, ", ctrl.progress = ", ctrl.progress );
			}

			ctrl.updateProgressPercents();

		};
	
		ctrl.stop_progress_bar = function(){
			if (angular.isDefined(progress_interval_handle)) {
				$interval.cancel(progress_interval_handle);
	            progress_interval_handle = undefined;
	
				ctrl.progress = 0;
				ctrl.updateProgressPercents();
				ctrl.progress_bar_active = false;
			}
		};

		ctrl.updateProgressPercents = function(){

			var sWho = "updateProgressPercents";

			ctrl.left_progress_percent = "" + ctrl.progress + "%";
			ctrl.right_progress_percent = "" + (100-ctrl.progress) + "%";

			//console.log(sWho + "(): GILLIGAN: Skipper, ctrl.left_progress_percent = '" + ctrl.left_progress_percent + "'...");
			//console.log(sWho + "(): GILLIGAN: Skipper, ctrl.right_progress_percent = '" + ctrl.right_progress_percent + "'...");

		};/* ctrl.updateProgressPercents() */
	
		$scope.$on('$destroy', function() {
			// Make sure that the interval is destroyed too
			ctrl.stop_progress_bar();
		});

		$scope.$watch(  'ctrl.active', function(value) {
			var sWho = sInnerWho + "::$scope.$watch('ctrl.active')";
			console.log(sWho + "(): GILLIGAN: Skipper, ctrl.active has changed, Skipper...");
			console.log(sWho + "(): GILLIGAN: Skipper, ctrl.active = " + ctrl.active + "..."); 

			if( ctrl.active ){
				console.log(sWho + "(): GILLIGAN: Skipper, calling ctrl.start_progress_bar()...");
				ctrl.start_progress_bar();
			}
			else {
				console.log(sWho + "(): GILLIGAN: Skipper, calling ctrl.stop_progress_bar()...");
				ctrl.stop_progress_bar();
			}
		});

	}; /* ourController() */

	return {
		restrict: 'AEC',

		// Create an isolate scope, Monsieur...
		scope: {
			'debug': '=debug',
			'debug_html': '=debugHtml',
			'active': '=',
		},

	    templateUrl: 'progress-bar-lev-directive.html',
		controllerAs: 'ctrl',
		bindToController: true,
		controller: ourController
	};


}]);

