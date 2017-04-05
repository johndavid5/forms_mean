exports.config = {

	framework: 'mocha',

	specs: [
		'test/e2e/**/*.spec.js'
	],

	mochaOpts: {
		// Enable timeout bugs with Mocha
		// that I've found using Protractor.
		// May be unnecessary now...
		enableTimeouts: false,

		// So that console.log() goes to the terminal...
		captureConsole: true
	}
};
