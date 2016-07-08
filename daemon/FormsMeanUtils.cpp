#include "FormsMeanUtils.h"

namespace JDA { 

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


/* static */ string FormsMeanUtils::cikFromUrl( const string& sUrl ){

	std::vector<std::string> elems;

	std::regex re_cik("(\\d+)");
	std::smatch sm;    // same as std::match_results<string::const_iterator> sm;

	elems.clear();
	JDA::Utils::split( sUrl, '/', elems ); 

	for( size_t i = 0; i < elems.size(); i++ ){
		if( std::regex_match (elems[i], sm, re_cik ) ){
			return elems[i];
		}
	}

	elems.clear();
	JDA::Utils::split( sUrl, "--", elems ); 

	for( size_t i = 0; i < elems.size(); i++ ){
		if( std::regex_match (elems[i], sm, re_cik ) ){
			return elems[i];
		}
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

} /* namespace JDA */

