/**
* Many thanks to Anthony Estebe: 
*
* Many thanks to Anthony Estebe at...
*
*   http://microblog.anthonyestebe.com/2013-11-30/window-resize-event-with-angular/
*
* Sample usage:
*   <div resizable ng-style="{ width: windowWidth, height: windowHeight }" ></div>
*/
angular.module('waldoApp')
.directive('vfResizable', function($window) {
  return function($scope) {
    $scope.initializeWindowSize = function() {
      $scope.windowHeight = $window.innerHeight;
      //return $scope.windowWidth = $window.innerWidth;
      $scope.windowWidth = $window.innerWidth;
    };
    $scope.initializeWindowSize();
    //return angular.element($window).bind('resize', function() {
    angular.element($window).bind('resize', function() {
	  console.log("window resized, Moe...");
      $scope.initializeWindowSize();
      return $scope.$apply();
    });
  };
});

