#ifndef _FORMS_H_
#define _FORMS_H_

#include "utils.h"
#include "logger.h"

namespace JDA { 

class Forms {

	protected:
		VF::Logger* m_p_logger;

		int m_i_iteration_count;
		int m_i_byte_count;
		int m_i_query_attempt_count;
		int m_i_query_succeed_count;


	public:
		void setPLogger( VF::Logger* p_logger ){ m_p_logger = p_logger; }
		void setDbConnection(){}

		/** NOTE: curl will read HTTP_PROXY, HTTPS_PROXY, FTP_PROXY environmental variables
		 * for proxy URL...the remainder you can set here...
		*/

		/* Set to 1 if you want extra logging output... */
		void setFtpDebug( int i_ftp_debug ){ m_i_ftp_debug = i_ftp_debug; } 

		/** Set to FALSE if you wish to not use a proxy even if HTTP_PROXY, HTTPS_PROXY, or FTP_PROXY environment variables are set... */
		void setFtpNoProxy( bool b_ftp_no_proxy ){ m_b_ftp_no_proxy = b_ftp_no_proxy; }

		/** Set to "username:password", or try using just ":" for magical "NTLM" determination of username/password on Windows... */
		void setFtpProxyUserPass( bool b_ftp_no_proxy ){ m_b_ftp_no_proxy = b_ftp_no_proxy; }

		/**
		* Load bare-bones filings into database from entries in EDGAR index file...
		*
		* NOTE: Immediately after runnning this, you may call getIterationCount(), getByteCount(), getQueryAttemptCount(), and getQuerySucceedCount()
		* for further information.
		*
		* @return m_i_query_succeed_count (number of successful upserts)
		*/
		int Forms::loadFromEdgarIndexUrl( const string& sEdgarIndexUrl );

		int getIterationCount(){ return m_i_iteration_count; }
		int getByteCount(){ return m_i_byte_count; }
		int getQueryAttemptCount(){ return m_i_query_attempt_count; }
		int getQuerySucceedCount(){ return m_i_query_succeed_count; }

}; /* class Forms */

} /* namespace JDA */


#endif /* #ifndef _FORMS_H_ */
