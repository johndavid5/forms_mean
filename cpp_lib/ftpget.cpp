#include <string>

#include "FtpClient.h"

using namespace std;

class MyFtpClientCallback : public JDA::FtpClient::IFtpClientCallback 
{
	public:
		size_t m_i_iteration_count; 
		size_t m_i_byte_count; 

		MyFtpClientCallback(): m_i_iteration_count(0), m_i_byte_count(0) {}

		size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ){

			const char* sWho = "MyFtpClientCallback::dataReceived";
	
			m_i_iteration_count++;
	
			size_t numBytes = size * nmemb;
	
			m_i_byte_count += numBytes;
	
			cout << sWho << "(): m_i_iteration_count = " << m_i_iteration_count << "\n" 
				<< "\t" << "size = " << size << "\n" 
				<< "\t" << "nmemb = " << nmemb << "\n" 
				<< "\t" << "numBytes = " << numBytes << "\n" 
				<< "\t" << "m_i_byte_count = " << m_i_byte_count << endl;

			char* cbuf = (char*)buffer;

			// Three ways to write it out, Doctor Kovacs...

			//cout << "------------- <data-" << m_i_iteration_count << "> --------------\n"; 
			//for( size_t i = 0; i < numBytes; i++ ){
				// buffer is NOT zero-char-terminated, so feed in characters one-by-one...
			//	cout << cbuf[i];
			//}
			//cout << "------------- </data-" << m_i_iteration_count << "> --------------\n"; 

			cout << "------------- <write-data" << m_i_iteration_count << "> --------------\n"; 
			cout.write( cbuf, numBytes );
			cout << "------------- </write-data-" << m_i_iteration_count << "> --------------\n"; 

			//ostringstream oss_out;
			//oss_out.write( cbuf, numBytes );

			//cout << "------------- <ossout-data" << m_i_iteration_count << "> --------------\n"; 
			//cout << oss_out.str();
			//cout << "\n"; // What the heck, give 'em a line-break for free...
			//cout << "------------- </ossout-data-" << m_i_iteration_count << "> --------------\n"; 

			if( m_i_iteration_count > 3000 ){
				cout << sWho << "(): JOE KOVACS: returning 0 to attempt to cause an abort, Doc-tor Cy-a-nide..." << endl;
				return 0; // Cause an abort by not returning numBytes...
			}
			else {
				cout << sWho << "(): JOE KOVACS: returning numBytes = " << numBytes << ", Doc-tor Cy-a-nide..." << endl;
				return numBytes; // JOE KOVACS: Important to return numBytes, Doc-tor Cy-a-nide...
			}

		}/* dataReceived() */

}; /* class MyFtpClientCallback : public JDA::FtpClient::IFtpClientCallback */


int main(int argc, char** argv){

	cout.sync_with_stdio(true); // Get C and C++ I/O to play together nicely...

	const char* sWho = "ftpget";

	string filePath = "";
	string url = "";

	bool bNoProxy = false;
	string sFtpAuth = "";
	bool bCallback = false;
	int iDebug = 0;

	int i;

	for( i = 1; i < argc; i++ ){
		if( strcmp( argv[i], "-f" ) == 0 && i+1 < argc ){
			filePath = argv[++i];
		}
		else if( strcmp( argv[i], "--no-proxy" ) == 0 ){
			bNoProxy = true;
		}
		else if( strcmp( argv[i], "-dbg" ) == 0 ){
			iDebug = 1;
		}
		else if( strcmp( argv[i], "-auth" ) == 0 && i+1 < argc ){
			sFtpAuth = argv[++i];
		}
		else if( strcmp( argv[i], "-callback" ) == 0 && i+1 < argc ){
			bCallback = true;
		}
		else {
			url = argv[i];
		}
	}

	//ostream* pDownloadStream = NULL;
	ostringstream* pDownloadStream = NULL;
	ostringstream oss_download;

	if( filePath.length() == 0 ){
		//cerr << "Please supply filename!" << endl;
		//exit(1);
		pDownloadStream = &oss_download;
	}

	//if( iDebug ){
		cout << sWho << ": url = '" << url << "'..." << endl;
		cout << sWho << ": filePath = '" << filePath << "'..." << endl;
		cout << sWho << ": pDownloadStream = " << pDownloadStream << "..." << endl;
		cout << sWho << ": iDebug = " << iDebug << "..." << endl;
		cout << sWho << ": bNoProxy = '" << bNoProxy << "'..." << endl;
		cout << sWho << ": sFtpAuth = '" << sFtpAuth << "'..." << endl;
		cout << sWho << ": bCallback = '" << bCallback << "'..." << endl;
	//}

	if( url.length() == 0 ){
		cerr << "Please supply url!" << endl;
		exit(1);
	}

	JDA::FtpClient ftpClient = JDA::FtpClient();

	try {

		JDA::Logger* pLogger = NULL;

		JDA::FtpClient::IFtpClientCallback* pIFtpClientCallback = NULL;

		if( bCallback ){
			//pIFtpClientCallback = new MyFtpClientCallback();

			// Use LineratorFtpClientCallback as a "decorator" to return data line-by-line...
			pIFtpClientCallback = new JDA::FtpClient::LineratorFtpClientCallback( new MyFtpClientCallback() );
		}

		cout << sWho << ": Calling ftpClient.grabIt( url = \"" << url << "\", filePath = \"" << filePath << "\", pDownloadStream=" << pDownloadStream << "," << "\n"
			<< "iDebug = " << iDebug << ", bNoProxy = " << JDA::Utils::boolToString( bNoProxy) << ", sFtpAuth = \"" << sFtpAuth << "\"," << "\n" 
			<< "pLogger = " << pLogger << ", pIFtpClientCallback = " << pIFtpClientCallback << "\n"
			<< ")..." << endl;

		size_t iByteCount = ftpClient.grabIt( url, filePath, pDownloadStream, iDebug, bNoProxy, sFtpAuth, pLogger, pIFtpClientCallback );

		/* Flush out any debug output from FtpClient... */
		fflush(stdout); 
		fflush(stderr);

		cout << "\n" << "***> Received " << JDA::Utils::commify( iByteCount ) << " byte" << (iByteCount == 1 ? "" : "s" ) << "." << endl;

		if( pDownloadStream != NULL ){
			//cout << "pDownloadStream = <<EOF;\n" << pDownloadStream->str() << "\nEOF\n" << endl;
			cout << "pDownloadStream = <<EOF;\n" << oss_download.str() << "\nEOF\n" << endl;
		}

	}catch( JDA::FtpClient::FtpException& e ){ 

		cout << "\n" << "$#*! My Dad Says> Caught JDA::FtpClient::FtpException: \"" << e.what() << "\"." << endl;

	}

	return 0;

}/* int main(int argc, char** argv) */
