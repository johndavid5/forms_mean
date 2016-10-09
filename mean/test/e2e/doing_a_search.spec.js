// use chai's assertion library...
var expect = require('chai').expect;

describe('doing a search', function(){
	it('goes to homepage and does a search',
	function(){
		// go to homepage
		var s_homepage = 'http://localhost:3000';
		console.log("*** Going to homepage \"" + s_homepage + "\"...");
		browser.get(s_homepage);

		//console.log("Pausing...");
		//browser.pause();

		// fill in the filer-name-filter...
		var filter_text = 'john';
		console.log("*** Filling in filer_name_filter with filter_text = \"" + filter_text + "\"...");
		element(by.id('filer_name_filter')).sendKeys(filter_text);

		// hit the search button
		console.log("*** Hitting search button...");
		element(by.id('search_button')).click();

		//console.log("Pausing...");
		//browser.pause();

		// user should now see filings with 'john' in company name

		var test1 = false;
		if( test1 ){
			var i_count = 0;
			//element.all(by.tagName('tr'))
			element.all(by.binding('filing.dn_company_conformed_name'))
			.getText()
			.then(function(text)
			//.each(function(el)
			{
				console.log("*** all table rows...");
				i_count++;
				console.log("*** #" + i_count + ": \"" + text + "\"");
			});
		}


		var test2 = true;
		if( test2 ){
			element.all(by.binding('filing.dn_company_conformed_name'))
			.first()
			.getText()
			.then(function(text){
				console.log("*** one table row...");
				console.log(text);
	
				// Assert that text contains "john" using 'chai'...
				console.log("*** Asserting text contains \"john\" via chai.expect()...");
				expect(text.toUpperCase()).to.contain("JOHN");
	
				//expect(text).to.contain("johnny");
			});
		}

		var test3 = false 
		if(test3){
			// Just have to figure out how to do this in a case-insensitive fashion...
			console.log("*** Asserting text contains \"john\" via chai-as-promised.eventually...");
			expect(element.all(by.css('ul.list-group li')).first().getText().toUpperCase()).to.eventually.contain("JOHN");
		}
		
		//var count = element.all(by.tagName('tr')).count();
		//console.log("*** count = " + count + "..." );
		//var list = element.all(by.tagName('td'));
		//var list = element.all(by.binding('filing.dn_company_conformed_name'));
		//console.log("*** list.count() = " + list.count() + "...");
		//console.log("*** list = ", list );
		//for( var i = 0; i < 200; i++ ){
		//	console.log("*** list[" + i + "].getText() = " + list.get(i).getText() + "..." );
		//	console.log("*** list[" + i + "].getInnerHtml() = " + list.get(i).getInnerHtml() + "..." );
		//	console.log("*** list[" + i + "].getOuterHtml() = " + list.get(i).getOuterHtml() + "..." );
		//}

		//all(by.tagName('tr')).then(function(elems){
		//	console.log("*** length = " + elems.length + "..." );
		//});

		//console.log("Pausing...");
		//browser.pause();
	});
});
	
