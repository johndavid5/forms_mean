angular.module('waldoApp')
.controller('ApplicationCtrl', function($scope, $route, $routeParams, UtilsSvc ){

	// Expose $route for this controller...
	// will be used to determine active
	// nav tab...
	$scope.$route = $route;

	// $route is being changed dynamically, so listen
	// for the change to get the latest...
	// by the time it prints to the console
	// it's already been resolved...
	// See http://stackoverflow.com/questions/18368262/angular-js-getting-access-to-the-params-via-route-or-routeparams
	$scope.$on('$routeChangeSuccess',
		function(){
			//console.log("ApplicationCtrl: $routeChangeSuccess(): $scope.$route.current = ", $scope.$route.current, ", Moe...");
			//console.log("ApplicationCtrl: $routeChangeSuccess(): $scope.$route.current.params = ", $scope.$route.current.params, ", Moe...");
			//console.log("ApplicationCtrl: $routeChangeSuccess(): $scope.$route.current.params.hide_navbar = ", $scope.$route.current.params.hide_navbar, ", Moe...");
			if( $scope.$route.current.params.hide_navbar ){
				$scope.hide_navbar = UtilsSvc.stringToBool( $scope.$route.current.params.hide_navbar );
				console.log("Moe, I just set $scope.hide_navbar to ", $scope.hide_navbar, ", Moe, OK, Moe...?");
			}
		}
	);

	$scope.showLoginMenuItem = false;

	//$scope.hide_navbar = true;
	//console.log("ApplicationCtrl: $scope.hide_navbar = ", $scope.hide_navbar, ", Moe...");

	// Subscribe to the 'login' event,
	// then update $scope.CurrentUser
	// accordingly...what the hades
	// is that underscore doing there?
	$scope.$on('login', function(_, user){
		$scope.currentUser = user;
	});
});
