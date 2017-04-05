var expect = require('chai').expect;
var jutils = require('../../../../jutils.js');

// Does controller GET /api/forms return status code 200...?
describe('', function()
{				
	describe('jUtils.endsWith', function(){
		it('exists', function() {	
			console.log("\n" +
					"*** TEST:\n" + 
					"\t" + __filename + ":\n" +
				   	"\t" + "jutils.endsWith() exists...?");
			console.log("\n" +
					"*** TEST:\n" + 
					"\t" + __filename + ":\n" +
				   	"\t" + "jutils.endsWith() = ", jutils.endsWith, "...");
			expect(jutils.endsWith).to.exist;
		});
		it('works', function() {	
			console.log("\n" +
					"*** TEST:\n" + 
					"\t" + __filename + ":\n" +
				   	"\t" + "jutils.endsWith() works...?");
			expect(jutils.endsWith("joey", "ey")).to.be.true;
			expect(jutils.endsWith("joey", "ee")).to.be.false;
			expect(jutils.endsWith("", "ee")).to.be.false;
			expect(jutils.endsWith("", "")).to.be.true;
		});
	});
});


