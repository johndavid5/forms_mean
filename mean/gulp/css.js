var gulp = require('gulp');
var stylus = require('gulp-stylus');

gulp.task('css', function(){
	console.log("./gulp/css.js: Running it...");
	//gulp.src('css/**/*.styl.css')
	gulp.src('css/**/*.styl')
	.pipe(stylus())
	.pipe(gulp.dest('assets'))
});

gulp.task('watch:css', ['css'], function(){
	gulp.watch('css/**/*.styl', ['css']);
});
