angular.module('waldoApp').directive('vfProgressBar', function() {

    return {

		restrict: 'A',

		link: function(scope, element, attrs) {

			var sWho = "vfProgressBar";

			var watchFor = attrs.progressBarWatch;

			var left_right = attrs.progressBarLeftRight;

			// update now
			var val = scope[watchFor];

			//console.log(sWho + "(): initially, val = ", val, "...");
			//console.log(sWho + "(): initially, left_right = ", left_right, "...");
			//console.log(sWho + "(): initially, scope = ", scope , "...");
			//console.log(sWho + "(): initially, element = ", element, "...");
			//console.log(sWho + "(): initially, attrs = ", attrs , "...");

			//element.attr('aria-valuenow', val)
			//	.css('width', val+"%");

			//var left_progress_percent = "" + val + "%";
			//var right_progress_percent = "" + (100-val) + "%";

			//element.filter('#left-progress').css('width', val+"%");
			//element.filter('#right-progress').css('width', (100-val)+"%");

			updateProgress( element, val );

			function updateProgress(leElement, leVal) {

				var sWho = "updateProgress";

           		////element.html('<div class="bar" style="width: ' + percentage + '%"></div>');
				
				//console.log(sWho + "(): left_right = ", left_right );
				//console.log(sWho + "(): leVal = ", leVal );
				
				var left_progress_percent = "" + leVal + "%";
				var right_progress_percent = "" + (100-leVal) + "%";

				//console.log(sWho + "(): left_progress_percent = \"" + left_progress_percent + "\"...");
				//console.log(sWho + "(): right_progress_percent = \"" + right_progress_percent + "\"...");

				if( left_right == "left" ){
					leElement.css('width', left_progress_percent);
				}
				else if( left_right == "right" ){
					leElement.css('width', right_progress_percent);
				}

   	     	}/* updateProgress() */

			scope.$watch(watchFor, function(val){

				//console.log(sWho + "(): $watch(), val = ", val, "...");
				//console.log(sWho + "(): $watch(), scope = ", scope , "...");
				//console.log(sWho + "(): $watch(), element = ", element, "...");
				//console.log(sWho + "(): $watch(), attrs = ", attrs , "...");

				//element.attr('aria-valuenow', val)
				//	.css('width', val+"%");
				updateProgress( element, val );
        	});
		}
	}
});
