var expect = require('chai').expect;
var ctrl = require('../../../../controllers/api/forms');

// Does controller exist...?
describe('controllers.api.forms', function()
{				
	it('exists', function() {	
		//console.log("*** TEST: forms-exists.spec.js: controllers.api.forms...exists...?");
		console.log("\n" +
				"\t" + "*** TEST:\n" +
				"\t" + __filename + ":\n" +
			   	"\t" + "controllers/api/forms exists...?");
		expect(ctrl).to.exist;
	});
});


