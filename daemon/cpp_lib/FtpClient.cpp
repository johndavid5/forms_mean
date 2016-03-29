#include "FtpClient.h"

#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

#include "logger.h"

#include <curl/curl.h>


//struct FtpFile {
//  std::string filename;
//  FILE *stream;
//  size_t iByteCount;
//  int iDebug;
//  JDA::Logger* pLogger;
//
//  FtpFile(): filename(""), stream(NULL), iByteCount(0), iDebug(0), pLogger(NULL) {}
//};


#include <curl/curl.h>
 

 


/* Used for 
*    CURLcode curl_easy_setopt(CURL *handle, CURLOPT_WRITEFUNCTION, write_callback);
* size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
*
* Note we should return the number of bytes handled or we'll get an abort/error...
*/
size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *userdata) {

  const char* sWho = "my_fwrite";

  //struct FtpFile* pFtpFile =(struct FtpFile *)userdata;
  JDA::FtpClient* pFtpClient = (JDA::FtpClient*)userdata;

  //cout << sWho << "(): size = " << size << ", nmemb = " << nmemb << "..." << endl;

  size_t numBytes = size * nmemb;

  pFtpClient->iByteCount += numBytes;

  if(pFtpClient && pFtpClient->pIFtpClientCallback ){
		return pFtpClient->pIFtpClientCallback->dataReceived( buffer, size, nmemb, userdata);
  }

  if(pFtpClient && pFtpClient->filePath.length() != 0 && !pFtpClient->stream) {

    /* "lazy-initialization of pFtpClient->stream": open file for writing */

    //pFtpClient->stream = fopen(pFtpClient->filename.c_str(), "wb");
	errno_t lastError = fopen_s( &(pFtpClient->stream), pFtpClient->filePath.c_str(), "wb" );

    //if(!pFtpClient->stream){
    //  return -1; /* failure, can't open file to write */
	//}

	if( lastError != 0 ){
		pFtpClient->stream = NULL;

		ostringstream oss_msg;
		oss_msg  << "Trouble opening '" << pFtpClient->filePath << "' for writing: " 
				<< lastError << ": \"" << JDA::Utils::strerror( lastError ) << "\"";
		throw JDA::FtpClient::FtpException( oss_msg.str().c_str() );
	}

  }/* if(pFtpClient && !pFtpClient->stream)  */

  if( pFtpClient && pFtpClient->filePath.length() != 0 && pFtpClient->stream) {
    return fwrite(buffer, size, nmemb, pFtpClient->stream);
  }
  else if( pFtpClient && pFtpClient->pDownloadStream != NULL ){

	// Buffer is NOT zero-char-terminated, so feed in characters one-by-one...
	char* cbuf = (char*) buffer;

	for( size_t i = 0 ; i < numBytes; i++ ){
		(*(pFtpClient->pDownloadStream)) << cbuf[i];
	}

    return numBytes;
  }

}/* my_fwrite(void *buffer, size_t size, size_t nmemb, void *userdata) */


size_t my_fheader( void *buffer, size_t size, size_t nmemb, void *userdata){

	const char* sWho = "my_fheader";

	//struct FtpFile* pFtpFile =(struct FtpFile *)userdata;
	JDA::FtpClient* pFtpClient =(JDA::FtpClient*)userdata;

	size_t numBytes = size * nmemb;

	//if( pFtpClient->iDebug ){
	//	cout << sWho << "(): Captain, we've got header! size = " << size << ", nmemb = " << nmemb << ", numBytes = " << numBytes << "..." << endl;
	//}

	// Buffer is not zero-char-terminated, so feed in characters one-by-one...
	char* cbuf = (char*) buffer;

	ostringstream ossHeader;
	for( size_t i = 0 ; i < numBytes; i++ ){
		ossHeader << cbuf[i];
	}

	//char* cbuf = new char[numBytes+1];
	//memcpy( cbuf, buffer, numBytes );
	//cbuf[ numBytes ] = '\0';

	//if( pFtpClient->iDebug ){
		//std::string sBuffer = std::string( (const char*)buffer, nmemb);
		//cout << sWho << "(): '" << sBuffer << "'" << endl;
		//cout << sWho << "(): '" << cbuf << "'" << endl;
		cout << sWho << "(): '" << ossHeader.str() << "'" << endl;
	//}

	//delete[] cbuf;

	return size * nmemb;

}/* my_fheader() */

int my_debug_callback(CURL *curly, curl_infotype infoType, char* buf, size_t numChars, void *userdata){ 

	const char* sWho = "my_debug_callback";

  	//struct FtpFile* pFtpFile =(struct FtpFile *)userdata;
	JDA::FtpClient* pFtpClient =(JDA::FtpClient*)userdata;

	string sInfoType = JDA::FtpClient::curl_infotype_to_string(infoType);

	//cout << sWho << "(): infoType = " << infoType << " = " << JDA::FtpClient::curl_infotype_to_string(infoType) << ", numChars = " << numChars << "..." << endl;
	//cout << sWho << "(): infoType = " << infoType << "..." << endl;
	
	ostringstream oss_out;
	for( size_t i = 0; i < numChars; i++ ){ 
		oss_out << buf[i];
	}

	// No, Archie!  You'll corrupt their buffer!
	//buf[numChars] = '\0';

	//cout << sWho << "(): buf = \"" << oss_out.str() << "\"" << endl;
	//cout << sInfoType << ": " << oss_out.str() << endl;
	//cout << sInfoType << ": " << buf << endl;

	const char* sWhovee="FtpClient";

	switch( infoType ){
		case CURLINFO_TEXT:
			if( pFtpClient->pLogger ){
				(*(pFtpClient->pLogger))(JDA::Logger::DEBUG) << sWhovee << " ** " << oss_out.str();
				pFtpClient->pLogger->flush();
			}
			else {
				cout << sWhovee << ": ** " << oss_out.str();
				cout.flush();
			}
			break;

		case CURLINFO_HEADER_OUT:
			if( pFtpClient->pLogger ){
				(*(pFtpClient->pLogger))(JDA::Logger::DEBUG) << sWhovee << " >> " << oss_out.str();
				pFtpClient->pLogger->flush();
			}
			else {
				cout << ">> " << oss_out.str();
				cout.flush();
			}
			break;

		case CURLINFO_HEADER_IN:
			if( pFtpClient->pLogger ){
				(*(pFtpClient->pLogger))(JDA::Logger::DEBUG) << "<< " << oss_out.str();
				pFtpClient->pLogger->flush();
			}
			else {
				cout << "<< " << oss_out.str();
				cout.flush();
			}
			break;
	}


	return 0; /* Must return 0. */

}/* my_debug_callback() */


/* static */ string JDA::FtpClient::curl_infotype_to_string( curl_infotype input ){

	switch( input ){
		case CURLINFO_TEXT:
			return "CURLINFO_TEXT";

		case CURLINFO_HEADER_IN:
			return "CURLINFO_HEADER_IN";

		case CURLINFO_HEADER_OUT:
			return "CURLINFO_HEADER_OUT";

		case CURLINFO_DATA_IN:
			return "CURLINFO_DATA_IN";

		case CURLINFO_DATA_OUT:
			return "CURLINFO_DATA_OUT";

		case CURLINFO_SSL_DATA_IN:
			return "CURLINFO_SSL_DATA_IN";

		case CURLINFO_SSL_DATA_OUT:
			return "CURLINFO_SSL_DATA_OUT";

		default:
			return "???";
	}
}/* \* static *\ string JDA::FtpClient::curl_infotype_to_string( curl_infotype input ) */

JDA::FtpClient::FtpClient(){
	this->reset();

	this->iDebug = 0;
	this->pLogger = NULL;
}

void JDA::FtpClient::reset(){

	this->filePath = "";
	this->stream = NULL;

	this->pDownloadStream = NULL;

	this->iByteCount = 0;
}


size_t JDA::FtpClient::grabIt( const string& url, const string& filePath, ostream* pDownloadStream,
				int iDebug /* = 0 */, bool bNoProxy /* = false */,
				const string& sFtpProxyUserPass /* ="" */, JDA::Logger* pLogger /* = NULL */, 
				IFtpClientCallback* pIFtpClientCallback /* = NULL */		
)
{

	const char* sWho = "JDA::FtpClient::grabIt";

	//char error_buffer[CURL_ERROR_SIZE];
	//char* error_buffer = new char[CURL_ERROR_SIZE];

	if( pLogger ){
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): url = '" << url << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): filePath = '" << filePath << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): pDownloadStream = '" << pDownloadStream << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): iDebug = " << iDebug << "..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): bNoProxy = '" << bNoProxy << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): sFtpProxyUserPass = '" << sFtpProxyUserPass << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): pLogger = '" << pLogger << "'..." << endl;
		(*pLogger)(JDA::Logger::DEBUG) << sWho << "(): pIFtpClientCallback = '" << pIFtpClientCallback << "'..." << endl;
	}

	if( url.length() == 0 ){
		ostringstream oss_msg;
		oss_msg  << "url is a required parameter!";
		throw JDA::FtpClient::FtpException( oss_msg.str() );
	}

	if( filePath.length() == 0 && pDownloadStream == NULL && pIFtpClientCallback == NULL ){
		ostringstream oss_msg;
		oss_msg  << "You must supply EITHER non-"" filePath OR non-NULL pDownloadStream (but not both of these) OR pIFtpClientCallback.";
		throw JDA::FtpClient::FtpException( oss_msg.str() );
	}

	if( filePath.length() != 0 && pDownloadStream != NULL ){
		ostringstream oss_msg;
		oss_msg  << "You must supply EITHER non-"" filePath OR non-NULL pDownloadStream, not both.";
		throw JDA::FtpClient::FtpException( oss_msg.str() );
	}

	//struct FtpFile* pFtpFile = new (struct FtpFile);
	//pFtpFile->filename = filePath;
	//pFtpFile->stream = NULL;
	//pFtpFile->iDebug = iDebug;
	//pFtpFile->iByteCount = 0;
	//pFtpFile->pLogger = pLogger;

	this->filePath = filePath;
	this->stream = NULL;

	this->pDownloadStream = pDownloadStream;
	this->pIFtpClientCallback = pIFtpClientCallback;

	this->iDebug = iDebug;
	this->iByteCount = 0;
	this->pLogger = pLogger;

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();

	if(curl){

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);

		/* Set a pointer to our struct to pass to the callback */
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, pFtpFile);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		/* Define callback for header data... */
		//curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, my_fheader);

		/* Set a pointer to our struct to pass to the header callback */
		// curl_easy_setopt(curl, CURLOPT_WRITEHEADER, pFtpFile);
		// curl_easy_setopt(curl, CURLOPT_WRITEHEADER, this);

		if( iDebug ){
			/* Switch on full protocol/debug output */
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_debug_callback);
			//curl_easy_setopt(curl, CURLOPT_DEBUGDATA, pFtpFile); 
			curl_easy_setopt(curl, CURLOPT_DEBUGDATA, this); 
		}

		curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

		//curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, this->error_buffer);

		if( bNoProxy ){
			std::string empty_string = "";
			curl_easy_setopt(curl, CURLOPT_PROXY, empty_string.c_str());
		}

		if( sFtpProxyUserPass.length() > 0 ){
			curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, sFtpProxyUserPass.c_str());
			//curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
			curl_easy_setopt(curl, CURLOPT_PROXYAUTH, CURLAUTH_NTLM);

			/**
			* CURLAUTH_NTLM 
			* HTTP NTLM authentication. A proprietary protocol invented and used by Microsoft.
			* It uses a challenge-response and hash concept similar to Digest, to prevent the
			* password from being eavesdropped. 
			*
			* You need to build libcurl with either OpenSSL, GnuTLS or NSS support for this
			* option to work, or build libcurl on Windows with SSPI support. 
			*/
		}

		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		if(CURLE_OK != res) {
			ostringstream oss_msg;
			//oss_msg  << "error: curl result = " << res;
			//oss_msg  << "Error: curl result = " << res << ": \"" << error_buffer << "\"";
			oss_msg  << "Error: curl result = " << res << ": \"" << this->error_buffer << "\"";
			throw JDA::FtpClient::FtpException( oss_msg.str() );
		}

	} else {
		ostringstream oss_msg;
		oss_msg  << "Error: curl init failed.";
		throw JDA::FtpClient::FtpException( oss_msg.str() );
	}

	//if(pFtpFile->stream){
	//	fclose(pFtpFile->stream); /* close the local file */
	//}

	if(this->stream){
		fclose(this->stream); /* close the local file */
	}

	//size_t iByteCount = pFtpFile->iByteCount;
	size_t iByteCount = this->iByteCount;

	curl_global_cleanup();

	//delete pFtpFile;
	//delete[] error_buffer;

	return iByteCount;
		
}/* void JDA::FtpClient::grabIt() */


size_t JDA::FtpClient::LineratorFtpClientCallback::dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ){
			
	const char* sWho = "JDA::FtpClient::LineratorFtpClientCallback::dataReceived";
				
	m_i_iteration_count++;
				
	size_t numBytes = size * nmemb;
				
	m_i_byte_count += numBytes;
				
	JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): m_i_iteration_count = " << m_i_iteration_count << "\n" 
			<< "\t" << "size = " << size << "\n" 
			<< "\t" << "nmemb = " << nmemb << "\n" 
			<< "\t" << "numBytes = " << numBytes << "\n" 
			<< "\t" << "m_i_byte_count = " << m_i_byte_count << "\n" 
			<< "\t" << "m_oss_line_buff = \"" << m_oss_line_buff.str() << "\"" << endl;
			
	char* cbuf = (char*)buffer;
			
	for( size_t i = 0; i < numBytes; i++ ){
			
		m_oss_line_buff << cbuf[i]; // Append char-by-char to line buffer...
			
		JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": cbuf[i] = '" << cbuf[i] << "', m_oss_line_buff = \"" << m_oss_line_buff.str() << "\"..." << endl;
			
		if( cbuf[i] == '\n' ){
			
			JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Looks like end-of-line..." << endl;
			
			if( m_pClientCallback ){	
			
				JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Calling m_pClientCallback->dataReceived( buffer = '" << m_oss_line_buff.str() << "', size = " << size << ", nmemb = " << m_oss_line_buff.str().length() << ", userdata = " << userdata  << " )..." << endl;
			
				size_t i_return = m_pClientCallback->dataReceived( (void*) m_oss_line_buff.str().c_str(), size, m_oss_line_buff.str().length(), userdata );  
			
				JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Got back i_return = " << i_return << "..." << endl;
			
				JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Is i_return same as size * nmemb  = " << size << " * " << m_oss_line_buff.str().length() << "...?" << endl;
			
				if( i_return != size * m_oss_line_buff.str().length() ){
					// Client returned a value not equal to size * nmemb, so we will return zero now
					// and thus abort the download...
					JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Sizes are not the same, returning 0 to caller to abort..." << endl;
					return 0;	
				}
				else{
					//JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): i = " << i << ": Sizes are the same, we continue..." << endl;
				}
			}
			m_oss_line_buff.str(""); // Clear the line buffer...
		}
			
	}/* for( size_t i = 0; i < numBytes; i++ ) */
			
	JDA::Logger::log(JDA::Logger::TRACE) << sWho << "(): end of data, returning numBytes = " << numBytes << " to caller..." << endl;

	// Return numBytes so that download is not aborted...
	return numBytes;
			
}/* JDA::FtpClient::LineratorFtpClientCallback::dataReceived() */
