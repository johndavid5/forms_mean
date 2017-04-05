angular.module('waldoApp')
//.controller('FilingsDetailCtrl', function($scope, FilingsSvc, PeopleSvc, $routeParams){
// Try using inline injection annotation to avoid problemas with minification...
.controller('FormsDetailCtrl', ['$scope', 'FormsSvc', 'UtilsSvc', 'SharedUtilsSvc', '$routeParams', '$window', function($scope, FormsSvc, UtilsSvc, SharedUtilsSvc, $routeParams, $window){

	$scope.sWho = "FormsDetailCtrl";

	$scope.edgarFileNameToEdgarSecFilingPageUrl = SharedUtilsSvc.edgarFileNameToEdgarSecFilingPageUrl;

	//$scope.debug_html = false;
	$scope.debug_html = UtilsSvc.stringToBool( $routeParams.debug_html );
	//$scope.debug_html = true

	$scope.accession_number = $routeParams.accessionNumber;

	console.log($scope.sWho + "(): $scope.accession_number = \"" + $scope.accession_number + "\"...");

	$scope.fetch_form = function(){

		FormsSvc.fetch({accessionNumber: $routeParams.accessionNumber, all: true})
		.success(function(forms){

			console.log("FormsDetailCtrl: SHEMP: Hey, Moe, I'm setting $scope.form to forms[0] = ", forms[0], ", Moe...");

			$scope.form = forms[0];


		});
	}; /* $scope.fetch_form() */

	$scope.fetch_form();

}]);


