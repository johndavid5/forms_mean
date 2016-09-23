angular.module('waldoApp')
.directive('vfListPicker', function($window, $document) {

	console.log("Let off some vfListPickers, Bennett!");

	var ourController = function($scope,$element,$attrs,$window,$document){

		var sWho = "vfListPicker::controller";

		console.log(sWho + "(): this =", this );	
		console.log(sWho + "(): \$scope =", $scope );	
		console.log(sWho + "(): \$element =", $element );	
		console.log(sWho + "(): \$attrs =", $attrs);
		console.log(sWho + "(): \$window = ", $window);
		console.log(sWho + "(): \$document = ", $document);
		
		// Make "this" available to nested anonymous functions...
		var ctrl = this;	

		ctrl.select = function(){

			var sMyWho = sWho + "::select";

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );
			console.log(sMyWho + "(): ctrl.selectedListSelections = ", ctrl.selectedListSelections );
			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );
			console.log(sMyWho + "(): ctrl.unSelectedListSelections = ", ctrl.unSelectedListSelections );

			/*
			if( ! ctrl.leftSelectWidth ){
				ctrl.leftSelectWidth = "7em"; 
			}
			if( ! ctrl.rightSelectWidth ){
				ctrl.rightSelectWidth = "7em"; 
			}
			if( ! ctrl.pickerWidth){
				ctrl.pickerWidth = "325px"; 
			}
			if( ! ctrl.pickerHeight){
				ctrl.pickerHeight = "350px"; 
			}
			*/

			console.log(sMyWho + "(): Calling ctrl.moveEm( ctrl.unSelectedListSelections, ctrl.unSelectedList, ctrl.selectedList )...");
			ctrl.moveEm( ctrl.unSelectedListSelections, ctrl.unSelectedList, ctrl.selectedList );

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );

			console.log(sMyWho + "(): Calling ctrl.selectedList.sort()...");
			// sort case insensitive...
			ctrl.selectedList = ctrl.selectedList.sort(function(a,b){ return a.toUpperCase().localeCompare(b.toUpperCase())} );

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );

			console.log(sMyWho + "(): Calling ctrl.selectedList.gefilter()...");
			// eliminate dupes...
			ctrl.selectedList = ctrl.selectedList.filter( function( item, pos, ary ){ 
				return !pos || item.toUpperCase() != ary[pos-1].toUpperCase();
			});

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );

			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );

			console.log(sMyWho + "(): Calling ctrl.unSelectedList.sort()...");
			ctrl.unSelectedList = ctrl.unSelectedList.sort(function(a,b){ return a.toUpperCase().localeCompare(b.toUpperCase())} );

			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );

			console.log(sMyWho + "(): Calling ctrl.unSelectedList.gefilter()...");
			// eliminate dupes...
			ctrl.unSelectedList = ctrl.unSelectedList.filter( function( item, pos, ary ){ 
				return !pos || item.toUpperCase() != ary[pos-1].toUpperCase();
			});

			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );


			ctrl.unSelectedListSelections.length = 0;

		    ctrl.formFieldModel = ctrl.selectedList.join(", ");	

		}; /* ctrl.select() */

		ctrl.selectAll = function(){

			var sMyWho = sWho + "::selectAll";

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );
			console.log(sMyWho + "(): ctrl.selectedListSelections = ", ctrl.selectedListSelections );
			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );
			console.log(sMyWho + "(): ctrl.unSelectedListSelections = ", ctrl.unSelectedListSelections );

			console.log(sMyWho + "(): Calling ctrl.moveEm( 'all', ctrl.unSelectedList, ctrl.selectedList )...");
			ctrl.moveEm( 'all', ctrl.unSelectedList, ctrl.selectedList );

			console.log(sMyWho + "(): Calling ctrl.selectedList.sort()...");
			ctrl.selectedList.sort();

		    ctrl.formFieldModel = ctrl.selectedList.join(", ");	

		}; /* ctrl.selectAll() */

		ctrl.unSelect = function(){

			var sMyWho = sWho + "::unSelect";

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );
			console.log(sMyWho + "(): ctrl.selectedListSelections = ", ctrl.selectedListSelections );
			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );
			console.log(sMyWho + "(): ctrl.unSelectedListSelections = ", ctrl.unSelectedListSelections );

			console.log(sMyWho + "(): Calling ctrl.moveEm( ctrl.selectedListSelections, ctrl.selectedList, ctrl.unSelectedList )...");
			ctrl.moveEm( ctrl.selectedListSelections, ctrl.selectedList, ctrl.unSelectedList );

			console.log(sMyWho + "(): Calling ctrl.unSelectedList.sort()...");
			ctrl.unSelectedList.sort();

			ctrl.selectedListSelections.length = 0;

		    ctrl.formFieldModel = ctrl.selectedList.join(", ");	

		}; /* ctrl.unSelect() */

		ctrl.unSelectAll = function(){

			var sMyWho = sWho + "::unSelectAll";

			console.log(sMyWho + "(): ctrl.selectedList = ", ctrl.selectedList );
			console.log(sMyWho + "(): ctrl.selectedListSelections = ", ctrl.selectedListSelections );
			console.log(sMyWho + "(): ctrl.unSelectedList = ", ctrl.unSelectedList );
			console.log(sMyWho + "(): ctrl.unSelectedListSelections = ", ctrl.unSelectedListSelections );

			console.log(sMyWho + "(): Calling ctrl.moveEm( 'all', ctrl.selectedList, ctrl.unSelectedList )...");
			ctrl.moveEm( 'all', ctrl.selectedList, ctrl.unSelectedList );

			console.log(sMyWho + "(): Calling ctrl.unSelectedList.sort()...");
			ctrl.unSelectedList.sort();

		    ctrl.formFieldModel = ctrl.selectedList.join(", ");	

		}; /* ctrl.unSelectAll() */

		ctrl.moveEm = function( which, from, to ){

			var sMyWho = sWho + "::moveEm";

			console.log(sMyWho + "(): which = ", which, ", from = ", from, " to = ", to, "...");

			if( ! which ){
				console.log(sWho + "(): WARNING: which is falsey, so not doing anything...");
				return;
			}

			var i;
			var j;
			var el;
			var key;

			if( which == 'all' ){

				// In one fell swoop...
				//var from_clone = angular.copy( from ); // CLONE "from"...
				//console.log(sWho + "(): from_clone = ", from_clone, "...");

				//to.length = 0; // Clear "to" array...
				//console.log(sWho + "(): After clearing 'to', to = ", to, "...");

				//to = from_clone; // Set "to" equal to CLONE of "from"...
				//console.log(sWho + "(): After setting 'to' equal to from_clone, to = ", to, "...");

				// Or, splice member by member...
				for( j = from.length-1; j >= 0; j-- ){
					el = from[j];
					console.log(sWho + "(): j = " + j + ", el = from[j] = from[" + j + "] = ", el );

					console.log(sWho + "(): j = " + j + ": Splice element j from 'from'...BEFORE: from=", from);
					from.splice(j,1); // Remove from 'from'...
					console.log(sWho + "(): j = " + j + ": Splice element j from 'from'...AFTER: from=", from);

					console.log(sWho + "(): j = " + j + ": Pushing el = ", el, " onto 'to'...");
					to.push( el );	 // Push onto 'to'...
				}

			}else{
				if( ! which instanceof Array ){
					console.log(sWho + "(): WARNING: which is not 'all' and which is not an Array, so not doing anything...");
					return;
				}
				for( i = 0; i < which.length; i++ ){ 	
					key = which[i];
					console.log(sWho + "(): i = " + i + ", key = which[i] = ", key );
					j = from.indexOf( key );		
					console.log(sWho + "(): j = from.indexOf( key ) = ", j );
					if( j >= 0 ){
						console.log(sWho + "(): el = from[j] = from[" + j + "] = ", el );
						el = from[j];

						console.log(sWho + "(): from.splice( j, 1 )...");
						console.log(sWho + "(): BEFORE splice(): from = ", from );
						from.splice(j, 1); // Remove from 'from'...
						console.log(sWho + "(): BEFORE splice(): from = ", from );

						console.log(sWho + "(): BEFORE to.push(el), to = ", to );
						to.push( el ); // Push onto 'to'...
						console.log(sWho + "(): AFTER to.push(el), to = ", to );
					}
				}
			}
		}; /* ctrl.moveEm() */

		ctrl.whichIsSelected = function( selectObject ){
			var i;
			var output = [];
		    for (i = 0; i < selectObject.options.length; i++ ){
				if( selectObject[i].selected ){
					output.push( { 'index': i, 'value': selectObject[i].value, 'text': selectObject[i].text } );
				}
			}
			return output;
		};/* ctrl.whichIsSelected() */

		/** 
		* Moves all selected rows from SS1 to SS2, where
		* SS1 and SS2 are JavaScript DOM Select objects.
		*
		* Many thanks to 
		* http://www.johnwbartlett.com/cf_tipsntricks/index.cfm?TopicID=86
		*
		* This is impossible to do with jQuery...gotta use old-fashioned
		* JavaScript and DOM.
		*/
		ctrl.SelectMoveRows = function(SS1,SS2,bMoveAll)
		{
		    var SelID='';
		    var SelText='';
			var i;
		
		    // Move rows from SS1 to SS2 from bottom to top
		    for (i=SS1.options.length - 1; i>=0; i--)
		    {
		        if (bMoveAll || SS1.options[i].selected == true)
		        {
		            SelID=SS1.options[i].value;
		            SelText=SS1.options[i].text;
		            var newRow = new Option(SelText,SelID);
		            SS2.options[SS2.length]=newRow;
		            SS1.options[i]=null;
		        }
		    }
		
		}/* SelectMoveRows() */

	}; /* ourController() */

	return {
		restrict: 'E',

		// Create "isolate scope"...
		scope: {
			'pickerTitle': '=',
			'selectedList': '=',
			'unSelectedList': '=',
			'formFieldModel': '=',
			'offsetX': '=',
			'offsetY': '=',
			'leftSelectWidth': '=',
			'rightSelectWidth': '=',
			'pickerWidth': '=',
			'pickerHeight': '=',
			'isOpen': '=',
			'debugHtml': '=',
		},
		
		//template: '<div style="border: 1px solid green;"><h3>List Picker</h3></div>'
		//templateUrl: 'myListPickerDirective.html',
		templateUrl: 'list-picker-directive.html',

		controllerAs: 'listPickerCtrl',
		bindToController: true,
		controller: ourController
	};
});

