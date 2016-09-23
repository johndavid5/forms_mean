var fs = require('fs');

var gulp = require('gulp');
var concat = require('gulp-concat');

var jutils = require('./jutils');

var sWho = "gulpfile.js";

// Call require() on all tasks in the ./gulp subdirectory...
fs.readdirSync(__dirname +
'/gulp').forEach( function(task){
	/* Make sure it ends with ".js" ... */
	if( jutils.endsWith( task, ".js") ){
		var sRequiree = './gulp/' + task;
		console.log( sWho + ": require( '" + sRequiree + "')...");
		require( sRequiree );
	}
});

gulp.task('dev', ['watch:css', 'watch:js', 'dev:server']);

