#include <iostream>

#include "gtest/gtest.h"

#include <string>
using namespace std;

#include "utils.h"
#include "logger.h"

#include "TestsCommon.h"

int main(int argc, char** argv)
{
	const char* sWho = "tests.cpp::main";

	string sDebugLevel = "INFO";	

	for( int i=0; i<argc; i++ ){
		if( strcmp( argv[i], "-v" ) == 0 && i + 1 < argc ){
			sDebugLevel = argv[++i];		
		}
	}

	JDA::Logger::DebugLevelType debugLevel = JDA::Logger::LevelNameToDebugLevel( sDebugLevel );

	cout << sWho << "(): sDebugLevel = '" << sDebugLevel << "'..." << endl;
	cout << sWho << "(): debugLevel = " << debugLevel << " = " << JDA::Logger::getLevelName( debugLevel ) << "..." << endl;

	if( debugLevel != JDA::Logger::NOTSET ){
		cout << sWho << "(): Setting TestsCommon::debug_level to " << JDA::Logger::getLevelName( debugLevel ) << "..." << endl;
		TestsCommon::debug_level = debugLevel;
	}

	int exitCode = 1;

	try {

		cout << sWho << "(): Running tests..." << endl;

		::testing::InitGoogleTest(&argc, argv);

		exitCode = RUN_ALL_TESTS();

	}catch( std::exception& e ){
		cout << sWho << "(): Caught std::exception: \"" << e.what() << "\"" << endl;
		exitCode = 2;
	}
	catch( ... ){
		cout << sWho << "(): Caught unknown exception!" << endl;
		exitCode = 3;
	}

	cout << sWho << "(): exitCode = " << exitCode << "..." << endl;

	cout << sWho << "(): Let off some steam, Bennett!" << endl;
	
	return exitCode;

}/* main() */


