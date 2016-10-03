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

	$scope.mapee = [];

	$scope.map_form = function(){

		var sWho = "$scope.map_form";

		console.log("FormsDetailCtrl: " + sWho + "()...");

		$scope.mapee = [];

		console.log("FormsDetailCtrl: " + sWho + "(): issuer, Skipper...?");
		if( $scope.form.issuer && $scope.form.issuer.company_data && $scope.form.issuer.company_data.company_conformed_name.length > 0 ){
			console.log("FormsDetailCtrl: " + sWho + "(): issuer, Yes, Skipper...?");
			$scope.mapee.push( { "name": "Issuer", "where": $scope.form.issuer } );			
		}

		console.log("FormsDetailCtrl: " + sWho + "(): reporting owner, Skipper...?");
		if( $scope.form.reporting_owner && $scope.form.reporting_owner.company_data.company_conformed_name.length > 0 ){
			console.log("FormsDetailCtrl: " + sWho + "(): reporting owner, Yes, Skipper...?");
			$scope.mapee.push( { "name": "Reporting Owner", "where": $scope.form.reporting_owner } );			
		}

		console.log("FormsDetailCtrl: " + sWho + "(): filed_by, Skipper...?");
		if( $scope.form.filed_by && $scope.form.filed_by.company_data.company_conformed_name.length > 0 ){
			console.log("FormsDetailCtrl: " + sWho + "(): filed_by, Yes, Skipper...?");
			$scope.mapee.push( { "name": "Filed By", "where": $scope.form.filed_by } );			
		}
	}

	$scope.fetch_form = function(){

		FormsSvc.fetch({accessionNumber: $routeParams.accessionNumber})
		.success(function(forms){

			console.log("FormsDetailCtrl: SHEMP: Hey, Moe, I'm setting $scope.form to forms[0] = ", forms[0], ", Moe...");

			$scope.form = forms[0];

			//$scope.map_form();

			// <For debugging only...>
			//$scope.show_add_people = true;
			////$scope.email_addresses_entered = "tneher@accelvp.com, edward.ristaino@akerman.com";
			//$scope.email_addresses_entered = "dbuck@andrewskurth.com, info@appliedminerals.com, joe.kovacs@ptl.com";
			//$scope.email_addresses_entered = "joe.kovacs@ptl.com, mike.hammer@ptl.com, kim.chee@ptl.com";
			//$scope.submit_email_addresses_click();
			//$scope.ciks_entered = "0001118974, 0000812149, 0001504876";
			// </For debugging only...>
		});
	}; /* $scope.fetch_form() */

	$scope.fetch_form();

}]);


