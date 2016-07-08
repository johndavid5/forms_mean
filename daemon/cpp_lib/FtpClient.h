#ifndef _FTP_CLIENT_H_
#define _FTP_CLIENT_H_

#include <string>
#include <utils.h>
#include <exception>

using namespace std;

#include <curl/curl.h>

#include "logger.h" // JDA::Logger

namespace JDA {

	class FtpClient {

		public:				

			class FtpException : std::exception {
				public:
					string errString;

				FtpException(const string& errString){
					this->errString = errString;
				}

				virtual ~FtpException() throw() {}

				virtual const char* what() const throw()
				{
					return this->errString.c_str();
				}

			}; /* class FtpException : std::exception */

			/* User provides an implementation of this interface for user-defined dataReceived() callback... 
			*
			* Note that if dataReceived() callback does not return numBytes = size * nmemb, it will
			* cause an abort.  Sorry, not my rules...
			*/ 
			class IFtpClientCallback {
				public:
					virtual size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata ) = 0;
			};


			/**
			* A "decorator" IFtpClientCallback to deliver data line-by-line to its client...
			*
			* For example of use see ./utils/ftp/ftpget.cpp
			*/
			class LineratorFtpClientCallback : public JDA::FtpClient::IFtpClientCallback 
			{
				public:
			
					JDA::FtpClient::IFtpClientCallback* m_pClientCallback;
			
					size_t m_i_iteration_count; 
					size_t m_i_byte_count; 
					ostringstream m_oss_line_buff;

					JDA::Logger* m_p_logger;
			
			
					LineratorFtpClientCallback(JDA::FtpClient::IFtpClientCallback* pClientCallback): m_pClientCallback(pClientCallback), m_i_iteration_count(0), m_i_byte_count(0), m_p_logger(NULL) {
						const char* sWho = "JDA::FtpClient::LineratorFtpClientCallback::LineratorFtpClientCallback";
						(void)sWho; /* who says sWho is unused...? */
						//JDA::Logger::log(JDA::Logger::TRACE) << sWho << "()..." << endl;
						m_oss_line_buff.str(""); // Clear the ostringstream buffer...
					}

					virtual ~LineratorFtpClientCallback(){
						const char* sWho = "JDA::FtpClient::LineratorFtpClientCallback::~LineratorFtpClientCallback";
						(void)sWho; /* who says sWho is unused...? */
						//JDA::Logger::log(JDA::Logger::TRACE) << sWho << "()..." << endl;
					}
					
					size_t dataReceived( void* buffer, size_t size, size_t nmemb, void *userdata );
			
			
			}; /* class LineratorFtpClientCallback : public JDA::FtpClient::IFtpClientCallback */


			std::string filePath; // For download to file...
			FILE *stream;

			ostream* pDownloadStream; // For download to an ostream (which could be an ostringstream)...

			IFtpClientCallback* pIFtpClientCallback; // For calling user-defined callback function object... 

			size_t iByteCount;
			int iDebug;
			JDA::Logger* pLogger;

			char error_buffer[CURL_ERROR_SIZE];

		public:


			static string curl_infotype_to_string( curl_infotype input );

			FtpClient();

			void reset();

			/**
			* You must supply EITHER...
			* 		 filePath that is not equal to ""
			*        ...OR...
			*        pDownloadStream that is not equal to NULL
			*        ...OR...
			*        pIFtpClientCallback that is not equal to NULL
			*
			* @throws JDA::FtpClient::FtpException
			*
			* @param iDebug : set to non-zero for extra debug output from libcurl.
			* @param pLogger: Supply a pointer to a JDA::Logger object, otherwise debug output due to iDebug being non-zero will go to STDOUT.
			*
			* @return number of bytes read
			*/
			size_t grabIt(	const string& url, const string& filePath, ostream* pDownloadStream,
							int iDebug = 0, bool bNoProxy = false,
							const string& sFtpProxyUserPass="", JDA::Logger* pLogger = NULL,
						   	IFtpClientCallback* pIFtpClientCallback = NULL		
			);

	}; /* class FtpClient */


}/* namespace JDA */

#endif /* #ifndef _FTP_CLIENT_H_ */
