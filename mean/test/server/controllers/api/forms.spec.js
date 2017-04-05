var expect = require('chai').expect;
var api = require('../../support/api');

// Does controller GET /api/forms return status code 200...?
describe('controllers.api.forms', function()
{				
	describe('GET /api/forms', function(){
		it('exists', function(done) {	
			//console.log("TEST: forms.spec.js: GET /api/forms return status code 200...?");
			console.log("\n" +
					"*** TEST:\n" + 
					"\t" + __filename + ":\n" +
				   	"\t" + "GET /api/forms return status code 200 and length > 0 and length == 100...?");
			api.get('/api/forms')	
			.expect(200)
			.expect(function(response){
				expect( response.body ).to.have.length.above( 1 );
			})
			.expect(function(response){
				expect( response.body ).to.have.length( 100 );
			})
			.end(done)
		});
	});
});


