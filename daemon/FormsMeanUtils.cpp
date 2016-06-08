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

} /* namespace JDA */

