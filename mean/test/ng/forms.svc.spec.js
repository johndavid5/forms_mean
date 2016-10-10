// Note: using chai -- included via karma.conf.js...
describe('forms.svc', function(){

	beforeEach(module('waldoApp'));

	var FormsSvc;	

	beforeEach(inject(function(_FormsSvc_) {
		FormsSvc = _FormsSvc_
	}));

	console.log("\n" + 
			"*** TEST:\n" +
			"\t" + "forms.svc.spec.js" + ":\n" +
			"FormsSvc.fetch().to.exist...");

	describe('#fetch', function(){	
		it('exists', function(){
			expect(FormsSvc.fetch).to.exist
		});
	});

});
