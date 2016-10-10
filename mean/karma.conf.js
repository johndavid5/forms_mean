module.exports = function( config) {
	config.set( {
		frameworks: ['mocha', 'chai', 'jasmine'],
		files: [
		'assets/bower/angular/angular.js',
		'assets/bower/angular-route/angular-route.js',
		'assets/bower/angular-mocks/angular-mocks.js',
		'assets/bower/angular-cookies/angular-cookies.js',
		'assets/bower/angular-animate/angular-animate.js',
		'assets/bower/angular-bootstrap/ui-bootstrap-tpls.js',
		'ng/**/module.js',
		'ng/**/*.js',
		'test/ng/**/*.spec.js'
		],
		reporters: ['progress'],
		port: 9876,
		colors: true,
		logLevel: config.LOG_INFO,
		autoWatch: true,
		//browsers: ['PhantomJS'],
		browsers: ['Chrome'],
		singleRun: false })
}

// Dickey, Jeff. Write Modern Web Apps with the MEAN Stack:
// Mongo, Express, AngularJS, and Node.js (Develop and Design)
// (pp. 185-186). Pearson Education. Kindle Edition. 

