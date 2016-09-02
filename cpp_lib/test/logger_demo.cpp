#include <iostream>
#include <ios>
#include <vector>

#include <stdio.h>

#include "logger.h"

#include "stooge.h"
//#include "../../formd.h"

class MyWriter : JDA::Logger::ILogWriter
{
	protected:
		std::string m_sName;

	public:

	MyWriter( const std::string& sName ){
		this->m_sName = sName;
	}
	
	void logIt( const std::string& message ) const {
		cout << "Log it yourself, " << m_sName <<  ": '" << message << "'" << endl;
	}
};


int main(int argc, char* argv[])
{
    ios::sync_with_stdio(); // So you can gracefully inter-leave C++ and C style I/O... 

	cout << "Hello, C++ cout..." << endl;

	fprintf(stdout, "Hello, fprintf(stdout)...\n");

	MyWriter myWriter1 = MyWriter( "Joe" );
	MyWriter myWriter2 = MyWriter( "Charles" );

	string sLogFilePath = "logger_demo.log";
	cout << "sLogFilePath = '" << sLogFilePath << "'" << endl;

	JDA::Logger log;

	vector<Stooge> stooges;
	stooges.push_back( Stooge(1, "Moe", "Big Boss", "Why, you!") );
	stooges.push_back( Stooge(2, "Larry", "Meaningless Goof", "No, Moe!") );
	stooges.push_back( Stooge(3, "Curly", "Wild Man", "Woop! Woop! Woop!") );
	stooges.push_back( Stooge(4, "Shemp", "Possum Puss", "Meep! Meep! Meep!") );

	//vector<JDA::Edgar::FormD> formds;
	//formds.push_back( JDA::Edgar::FormD() );
	//formds[0].setSchemaVersion("S1");
	//formds.push_back( JDA::Edgar::FormD() );
	//formds[1].setSchemaVersion("S2");
	//formds.push_back( JDA::Edgar::FormD() );
	//formds[1].setSchemaVersion("S3");


	log.setLogFilePath( sLogFilePath );
	log.setLogFileOn( true );
	log.setLogFileAppend( true );

	log.setStdoutOn( true );

	log.addLogWriter( (JDA::Logger::ILogWriter*) &myWriter1 );
	log.addLogWriter( (JDA::Logger::ILogWriter*) &myWriter2 );

	for( size_t i = 0; i<stooges.size(); i++ ){
		log(JDA::Logger::NOTICE) << "stooges[" << i << "] = " << stooges[i] << "..." << endl;
	}

	//for( size_t i = 0; i<formds.size(); i++ ){
	//	log(JDA::Logger::NOTICE) << "formds[" << i << "] = " << formds[i] << "..." << endl;
	//}

	std::string msg1 = "Now is the time for all good men to come to the aid of the party.";

	cout << "log.getDebugLevel() = " << log.getDebugLevel() << " = " << JDA::Logger::getLevelName( log.getDebugLevel() ) << endl;

	cout << "Calling log.printf(JDA::Logger::DEBUG, \"" << msg1 << "\" )..." << endl;
	//log.printf(JDA::Logger::DEBUG, "Now is the time for all good men to come to the aid of the party.\n");

    log(JDA::Logger::ERROR) << "Level " << 7 << " hydraulic pressure seen" << std::endl;
    log(JDA::Logger::WARN) << "Level " << 5 << " hydraulic pressure seen" << std::endl;
	double angle = 3.1415927;
	cout << "angle = " << angle << endl;
	cout << "angle = " << setprecision(2) << angle << endl;
	//cout << "angle = " << precision(2) << angle << endl;
	cout << "angle = " << angle << endl;

	log(JDA::Logger::INFO) << "angle = " << angle << endl;
	log(JDA::Logger::ERROR) << "angle = " << setprecision(2) << angle << endl;
	//log << "angle = " << precision(2) << angle << endl;
	log(JDA::Logger::DEBUG) << "angle = " << angle << endl;
	log << "angle = " << angle << endl;

	log.setDebugLevel( JDA::Logger::DEBUG );
	log(JDA::Logger::DEBUG) << "angle = " << angle << endl;

	log.debug = 1;

	log(JDA::Logger::INFO) << "What if I'm too lazy to instantiate my own JDA::Logger object?" << endl;
	int numTimes = 7;
	log(JDA::Logger::INFO) << "Will I get away with it up to " << numTimes << " times?" << endl;
	numTimes *= 70;
	log(JDA::Logger::INFO) << "Nay, up to " << numTimes << " times." << endl;

	log(JDA::Logger::INFO) << "Flush demo..." << endl;

	log(JDA::Logger::INFO) << "1 ";
	log(JDA::Logger::INFO) << "2 ";
	log(JDA::Logger::INFO) << "3\n";
	log.flush();

	char* log_files[] = {"moe.log", "larry.log", "curly.log"};

	log.setLogFileOn( true );
	log.setLogFileAppend( true );
	for( size_t i = 0; i < sizeof(log_files)/sizeof(char*); i++ ){
		for( size_t j = 1; j <= 3; j++ ){
			log.setLogFilePath( log_files[i] );
			log(JDA::Logger::INFO) << log_files[i] << " #" << j << "..." << endl;
		}
	}

	fprintf(stdout, "\n**** Let off some steam, Bennett!\n\n");

	return 0;	
}
