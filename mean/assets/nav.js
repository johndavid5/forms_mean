	/* Get the stickyTableHeader jQuery plugin and
	* and the bootstrap fixed navbar to play
	* together nicely
	*/
	
	/** Invoke jQuery based stickyTableHeaders plug-in
	* so that the table header remains fixed at the top
	* even if you scroll the table horizontally...
	*
	* NOTE: Set offset for stickyTableHeader to the height of
	* the bootstrap navbar-fixed-top...
	*
	* Also set the body's padding-top CSS property
	* to this very same offset.
	*/
	function navStickyTableHeaderInit(){

		var sWho = "nav.js::navStickyTableHeaderInit";

		//console.log(sWho + "()...");

		var offset = $('.navbar-fixed-top').height();

		// In case it's a scrollable DIV, set the scrollableArea to
		// the DIV.  scrollableArea defaults to the Window. 
		if( $(".sticky-table-header-scrollable-area").length > 0 ){
			$('#stickyHeader').stickyTableHeaders({ scrollableArea: $(".sticky-table-header-scrollable-area")[0] });
		}
		else{
			$('#stickyHeader').stickyTableHeaders({fixedOffset: offset});
		}

		$('body').css( 'padding-top', offset );
	}

$( document ).ready( function(){ navStickyTableHeaderInit(); } );

navStickyTableHeaderInit();

$( window ).resize(function() {
  navStickyTableHeaderInit();
});

$(".sticky-table-header-scrollable-area").resize(function(){
  navStickyTableHeaderInit();
});


/*
* 
* http://stackoverflow.com/questions/10641646/twitter-bootstrap-2-on-collapse-event
* In bootstrap 3.x, the four events you're looking for are:
* 
* show.bs.collapse
* shown.bs.collapse
* hide.bs.collapse
* hidden.bs.collapse
*/

/* Also need to reset the stickyTableHeader offset if user collapses
* or expands the responsive navbar. IMPORTANT: We use "shown" and
* "hidden" rather than "show" and "hide" to re-do the offset
* AFTER the navbar is resized...
*/
$('.navbar-collapse').on('shown.bs.collapse', navStickyTableHeaderInit );
$('.navbar-collapse').on('hidden.bs.collapse', navStickyTableHeaderInit );
