angular.module('waldoApp')
.directive('vfSortButton', ['$window', function($window) {

	var sOuterWho = "vfSortButton";	

	var ourController = function($scope,$element,$attrs){

		var sInnerWho = sOuterWho + "::ctrl";

		var sWho = sInnerWho;

		console.log(sWho + "(): this =", this );	
		console.log(sWho + "(): $scope =", $scope );	
		console.log(sWho + "(): $element =", $element );	
		console.log(sWho + "(): $attrs =", $attrs);
		
		// Make "this" available to nested anonymous functions...
		var ctrl = this;	

		//ctrl.debug_html = true;

		//ctrl.buttonClick = function(){
		//	var sWho = sInnerWho + "::buttonClick";  
		//	console.log(sWho + "(): ctrl = ", ctrl );	
		//};

		// Watch ctrl.asc_desc...
		$scope.$watch(  'ctrl.asc_desc', function(value) {
			var sWho = sInnerWho + "::$scope.$watch";
			console.log(sWho + "(): SHEMP: ctrl.by_what = ", ctrl.by_what);
			console.log(sWho + "(): SHEMP: ctrl.asc_desc = ", ctrl.asc_desc);
		});

	}; /* ourController() */

	return {
		restrict: 'E',

		// Create an isolate scope...
		scope: {
			'asc_desc': '=',
			'by_what': '=',
			'debug_html': '='
		},

	    templateUrl: 'sort-button-directive.html',
		controllerAs: 'ctrl',
		bindToController: true,
		controller: ourController
	};

}]);

