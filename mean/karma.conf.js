module.exports = function( config) {
	config.set( {
		frameworks: [' mocha', 'chai'],
		files: [ 'assets/ angular/ angular.js',
		'assets/ angular-route/ angular-route.js',
		'assets/ angular-mocks/ angular-mocks.js',
		'ng/**/ module.js',
		'ng/**/*. js',
		'test/ ng/**/*. spec.js' ],
		reporters: [' progress'],
		port: 9876,
		colors: true,
		logLevel: config.LOG_INFO,
		autoWatch: true,
		//browsers: ['PhantomJS'],
		browsers: ['chrome'],
		singleRun: false })
}

// Dickey, Jeff. Write Modern Web Apps with the MEAN Stack:
// Mongo, Express, AngularJS, and Node.js (Develop and Design)
// (pp. 185-186). Pearson Education. Kindle Edition. 

