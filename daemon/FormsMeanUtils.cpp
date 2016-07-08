#include "FormsMeanUtils.h"

namespace JDA { 

/* static */ int FormsMeanUtils::debug = 0;

/* static */ const char* FormsMeanUtils::downloadWindowTypeEnumStrings[] = { "DAY", "EVENING", "WEEKEND" };

/* static */ string FormsMeanUtils::accessionNumberFromFilePath( const string& sFilePath ){

	string output = sFilePath;

	size_t iWhere;

	/* First, chop away anything to the left of the right-most "/" or "\" or "--", if you find it... */

	iWhere = output.rfind( "--" ); // flattened out file-name, e.g., file://../edgar/data--1000275--0001214659-14-000507.txt") = "0001214659-14-000507"

	if( iWhere != std::string::npos ){
		output = output.substr( iWhere+2 );
	}
	else {

		iWhere = output.rfind( '/' ); // UNIX-style c:/path/to/file

		if( iWhere == std::string::npos ){
			iWhere = output.rfind( '\\' ); // DOS-style: c:\path\to\file
		}

		if( iWhere != std::string::npos ){
			output = output.substr( iWhere+1 );
		}

	}

	/* Next, whittle away anything to the right of and including "." */
	iWhere = output.rfind( '.' );

	if( iWhere != std::string::npos ){
		output = output.substr( 0, iWhere );  
	}

	/* Et voila...!  What remaineth should be your accession number. */
	return output;

}/* accessionNumberFromFilePath( const string& sFilePath ) */


/* static */ string FormsMeanUtils::isoDateFromYyyyMmDd( const string& sYyyyMmDd ){

	if( sYyyyMmDd.length() == 8 ){ 
		ostringstream oss_out;
		oss_out << sYyyyMmDd.substr(0, 4) << "-" << sYyyyMmDd.substr(4, 2) << "-" << sYyyyMmDd.substr(6, 2); 
		return oss_out.str();
	}
	else{
		return sYyyyMmDd;
	}
}


/* static */ bool FormsMeanUtils::nuttinButNumbers( const string& s_input ){
	for( size_t i = 0 ; i < s_input.size(); i++ ){
		if( ! isdigit( s_input[i] ) ){
			return false;
		}
	}

	return true;
}

/* static */ string FormsMeanUtils::cikFromUrl( const string& sUrl ){

	const char* sWho = "FormsMeanUtils::cikFromUrl";

	if( FormsMeanUtils::debug ){
		cout << sWho << "( sUrl = \"" << sUrl << "\" )..." << endl;
	}

	std::vector<std::string> elems;

	//if( FormsMeanUtils::debug ){
	//	cout << sWho << "(): std::regex re_cik()..." << endl;
	//}
	//std::regex re_cik;

	// http://stackoverflow.com/questions/12530406/is-gcc-4-8-or-earlier-buggy-about-regular-expressions/12665408#12665408
	//string s_le_cik_regex = "(\\d+)";
	// g++ (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
	// Caught regex_error:
	// e.what() = "regex_error"
	// e.code() = 2 = "error_escape:	the expression contains an invalid escaped character or a trailing escape"...
	// string s_le_cik_regex = "([0-9]+)";
	//	C++ Regexp annoyed me quite a far bit in a project. I ended up using boost's regexp: boost.org/doc/libs/1_55_0/libs/regex/doc/html/index.html
	//	But that may not be what you want. – Mohammad Apr 3 '14 at 14:42
	//	  	 	
	// Use boost's regex. gcc's is broken right now. – KitsuneYMG Apr 3 '14 at 14:50
	//  	 	
	// See stackoverflow.com/questions/12530406/… – R. Martinho Fernandes Apr 3 '14 at 15:30
	//   	 	
	// gcc supports regex from version 4.9.0.! Older versions 4.8.x have no working support for regex.
	// Try the last snapshot of gcc 4.9.0 – Klaus Apr 3 '14 at 15:43

	//try {
	//	cout << sWho << "(): re_cik.assign( \"" << s_le_cik_regex << "\", std::regex_constants::ECMAScript | std::regex_constants::icase)..." << endl;
	//	re_cik.assign( s_le_cik_regex, std::regex_constants::ECMAScript | std::regex_constants::icase);
	//}
	//catch( std::regex_error& e ){
	//	cout << "Caught regex_error:\n" 
	//	<< 	"\t" << "e.what() = \"" << e.what() << "\"\n"
	//	<< 	"\t" << "e.code() = " << e.code() << " = \"" << FormsMeanUtils::regex_error_code_to_string(e.code()) << "\"...\n"
	//	<< "...re-throwing..." << endl;
	//	throw e;
	//}

	//cout << sWho << "(): std::smatch sm..." << endl;
	//std::smatch sm;    // same as std::match_results<string::const_iterator> sm;

	if( FormsMeanUtils::debug ){
			cout << sWho << "(): Splitting sUrl on '/'..." << endl;
	}
	elems.clear();
	JDA::Utils::split( sUrl, '/', elems ); 

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): elems.size() = " << elems.size() << "..." << endl;
	}

	for( size_t i = 0; i < elems.size(); i++ ){
		if( FormsMeanUtils::debug ){
			cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"" << endl;
		}
		//if( std::regex_match (elems[i], sm, re_cik ) ){
		if( FormsMeanUtils::nuttinButNumbers( elems[i] ) ){

			if( FormsMeanUtils::debug ){
				cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"...is a match, returnin' it..." << endl;
			}
			return elems[i];
		}
	}

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): Splitting sUrl on '\\'..." << endl;
	}
	elems.clear();
	JDA::Utils::split( sUrl, '\\', elems ); 

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): elems.size() = " << elems.size() << "..." << endl;
	}
	for( size_t i = 0; i < elems.size(); i++ ){

		if( FormsMeanUtils::debug ){
			cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"" << endl;
		}
		//if( std::regex_match (elems[i], sm, re_cik ) ){
		if( FormsMeanUtils::nuttinButNumbers( elems[i] ) ){

			if( FormsMeanUtils::debug ){
				cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"...is a match, returnin' it..." << endl;
			}
			return elems[i];
		}
	}

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): Splitting sUrl on '--'..." << endl;
	}
	elems.clear();
	JDA::Utils::split( sUrl, "--", elems ); 

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): elems.size() = " << elems.size() << "..." << endl;
	}

	for( size_t i = 0; i < elems.size(); i++ ){

		if( FormsMeanUtils::debug ){
			cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"" << endl;
		}
		//if( std::regex_match (elems[i], sm, re_cik ) ){
		if( FormsMeanUtils::nuttinButNumbers( elems[i] ) ){

			if( FormsMeanUtils::debug ){
				cout << sWho << "(): elems[" << i << "] = \"" << elems[i] << "\"...is a match, returnin' it..." << endl;
			}
			return elems[i];
		}
	}

	if( FormsMeanUtils::debug ){
		cout << sWho << "(): Looks like we couldn't find a match, returning \"\"..." << endl;
	}

	// Didn't find it...
	return "";

}/* cikFromUrl() */

/* static */ string FormsMeanUtils::double_quote_escape(const string& input){

	ostringstream oss_out("");

	for( size_t i = 0; i < input.length(); i++ ){
		if( input[i] == '"' ){
			oss_out << "\\\"";
		}
		//else if( input[i] == '\\' ){
		//	oss_out << "\\\\";
		//}
		else {
			oss_out << input[i];
		}
	}

	return oss_out.str();

}/* double_quote_escape() */

/* static */ string FormsMeanUtils::regex_error_code_to_string( std::regex_constants::error_type le_error_code ){

	switch(le_error_code){
		case std::regex_constants::error_collate: 
			return "error_collate:	the expression contains an invalid collating element name";
		case std::regex_constants::error_ctype:
			return "error_ctype:	the expression contains an invalid character class name";
		case std::regex_constants::error_escape:
			return "error_escape:	the expression contains an invalid escaped character or a trailing escape";
		case std::regex_constants::error_backref:
			return "error_backref:	the expression contains an invalid back reference";
		case std::regex_constants::error_brack:
			return "error_brack:	the expression contains mismatched square brackets ('[' and ']')";
		case std::regex_constants::error_paren:
			return "error_paren:	the expression contains mismatched parentheses ('(' and ')')";
		case std::regex_constants::error_brace:
			return "error_brace:	the expression contains mismatched curly braces ('{' and '}')";
		case std::regex_constants::error_badbrace:
			return "error_badbrace:	the expression contains an invalid range in a {} expression";
		case std::regex_constants::error_range:
			return "error_range:	the expression contains an invalid character range (e.g. [b-a])";
		case std::regex_constants::error_space:
			return "error_space:	there was not enough memory to convert the expression into a finite state machine";
		case std::regex_constants::error_badrepeat:
			return "error_badrepeat:	one of *?+{ was not preceded by a valid regular expression";
		case std::regex_constants::error_complexity:
			return "error_complexity:	the complexity of an attempted match exceeded a predefined level";
		case std::regex_constants::error_stack:
			return "error_stack:	there was not enough memory to perform a match";
		default:
			return "???";
	}/* switch(le_error_code) */

}/* regex_error_code_to_string() */

} /* namespace JDA */

