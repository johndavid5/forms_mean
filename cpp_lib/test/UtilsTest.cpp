#include <vector>
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <memory> // shared_ptr
using namespace std;

#include "gtest/gtest.h" /* googletest */

#include "utils.h"
#include "logger.h"

#include "TestsCommon.h"

namespace { 

//std::ostream& operator<<(std::ostream& os, const OrdinalSuffixExpected& ordSuffExp );

class UtilsTest: public ::testing::Test {

	public:

	class OrdinalSuffixExpected {
		public:
			int input;
			string expected_suffix;

			OrdinalSuffixExpected(int le_input, string le_suffix): input(le_input), expected_suffix(le_suffix) {
				const char* sWho = "OrdinalSuffixExpected::OrdinalSuffixExpected";
				//cout << sWho << "() <constructor>..." << (*this) << endl;
			}

			virtual ~OrdinalSuffixExpected(){
				const char* sWho = "OrdinalSuffixExpected::~OrdinalSuffixExpected";
				//cout << sWho << "() <destructor>..." << (*this) << endl;
			}

			virtual string toString(){
				ostringstream oss_out;
				oss_out << (*this);	
				return oss_out.str();
			}

			friend ostream& operator<<(ostream& os, const OrdinalSuffixExpected& ordSuffExp );

	}; /* class OrdinalSuffixExpected */

	void ordinal_suffix_expecteds_populate(){

		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 0, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1, "st" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 2, "nd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 3, "rd" ) );  
		// For deliberate error...
		//ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 3, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 4, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 5, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 6, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 7, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 8, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 9, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 10, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 11, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 12, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 13, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 14, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 15, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 16, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 17, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 18, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 19, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 20, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 21, "st" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 22, "nd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 23, "rd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 24, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 25, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 26, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 27, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 28, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 29, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 30, "th" ) );  

		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 100, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 101, "st" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 102, "nd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 103, "rd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 104, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 105, "th" ) );  

		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1000, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1001, "st" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1002, "nd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1003, "rd" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1004, "th" ) );  
		ordinal_suffix_expecteds.push_back( make_shared<OrdinalSuffixExpected>( 1005, "th" ) );  
	}/* void ordinal_suffix_expecteds_populate() */


	vector<shared_ptr<OrdinalSuffixExpected>> ordinal_suffix_expecteds;

	JDA::Logger log;

	UtilsTest(){
		const char* sWho = "UtilsTest::UtilsTest<constructor>";

		cout << sWho << "(): Calling log.setDebugLevel(TestsCommon::debug_level = " << TestsCommon::debug_level << " = " << JDA::Logger::getLevelName( TestsCommon::debug_level ) << ")..." << endl;
		log.setDebugLevel(TestsCommon::debug_level);

		log(JDA::Logger::DEBUG) << sWho << "()..." << endl;
		
	}
	
	virtual ~UtilsTest(){
		const char* sWho = "UtilsTest::~UtilsTest<destructor>";
		log(JDA::Logger::DEBUG) << sWho << "()..." << endl;
	}

	/** Before every test... */
	virtual void SetUp(){
		const char* sWho = "UtilsTest::SetUp";
		log(JDA::Logger::DEBUG) << sWho << "()..." << endl;
	}

	/** After every test... */
	virtual void TearDown(){
		const char* sWho = "UtilsTest::TearDown";
		log(JDA::Logger::DEBUG) << sWho << "()..." << endl;
	}

};/* class UtilsTest */

ostream& operator<<(ostream& os, const UtilsTest::OrdinalSuffixExpected& ordSuffExp ){
	os << "input = \"" << ordSuffExp.input << "\", expected_suffix = \"" << ordSuffExp.expected_suffix << "\"";
	return os;
}

TEST_F(	UtilsTest, TestAssert){ 

	const char* sWho = "UtilsTest::TestAssert";

	log(JDA::Logger::DEBUG) << sWho << "()..." << endl;

	EXPECT_NE( "expected", "actual" );

}/* TEST_F(	UtilsTest, TestAssert) */

TEST_F(	UtilsTest, TestHasWhitespace){ 

	const char* sWho = "UtilsTest::TestHasWhitespace";

	EXPECT_EQ( true, JDA::Utils::has_whitespace("Hello, World!") );
	EXPECT_EQ( false, JDA::Utils::has_whitespace("Hello!") );
	EXPECT_EQ( false, JDA::Utils::has_whitespace("") );
	EXPECT_EQ( true, JDA::Utils::has_whitespace(" ") );
	EXPECT_EQ( true, JDA::Utils::has_whitespace("  ") );
	EXPECT_EQ( true, JDA::Utils::has_whitespace("Hello,\tTab!") );

}/* TEST_F(	UtilsTest, TestHasWhitespace) */

TEST_F(	UtilsTest, TestTrim){ 

	const char* sWho = "UtilsTest::TestHasWhitespace";

	string s;
	s = "01";
	EXPECT_EQ( "1", JDA::Utils::trim_leading_zeroes(s) );
	s = "x1";
	EXPECT_EQ( "1", JDA::Utils::ltrim(s, 'x') );

}/* TEST_F(	UtilsTest, TestTrim) */


TEST_F(	UtilsTest, TestOrdSuffix){ 

	const char* sWho = "UtilsTest::TestCompare";

	for( size_t i = 0 ; i < ordinal_suffix_expecteds.size(); i++ ){

		try {

			string s_ordinal_suffix = JDA::Utils::get_ordinal_suffix( ordinal_suffix_expecteds[i]->input );

			log(JDA::Logger::DEBUG) << sWho << "(): i = " << i <<
				": SPOCK: Captain, for JDA::Utils::get_ordinal_suffix( " << ordinal_suffix_expecteds[i]->input << " ):\n" <<  
			"\t" << "expected: \"" << ordinal_suffix_expecteds[i]->expected_suffix << "\",\n"
			"\t" << "actual: \"" << s_ordinal_suffix << "\"" << endl;

			EXPECT_EQ( ordinal_suffix_expecteds[i]->expected_suffix, s_ordinal_suffix ) << "test case " << i << ": get_ordinal_suffix( " << ordinal_suffix_expecteds[i]->input << " )";
		}
		catch(JDA::Utils::Exception & e){ 
			// NOTE: FAIL() causes fatal failure, while ADD_FAILURE() causes non-fatal failure like EXPECT()
			ADD_FAILURE() << "Caught JDA::Utils::Exception: \"" << e.what() << "\"";
		}
		catch( ... ){
			ADD_FAILURE() << "Caught unknown Exception.";
		}

	} /* for( size_t i = 0 ; i < test_pairs.size(); i++ ) */

}/* TEST_F(	UtilsTest, TestOrdSuffix) */


TEST_F(	UtilsTest, TestStringToInt){ 

	const char* sWho = "UtilsTest::TestStringToInt";

	EXPECT_EQ( 1, JDA::Utils::stringToInt("1", 0 ) );
	EXPECT_EQ( 1, JDA::Utils::stringToInt("01", 0 ) );
	EXPECT_EQ( 10, JDA::Utils::stringToInt("010", 0 ) );
	EXPECT_EQ( -1, JDA::Utils::stringToInt("", -1 ) );
	EXPECT_EQ( -1, JDA::Utils::stringToInt("x", -1 ) );
	EXPECT_EQ( -1, JDA::Utils::stringToInt("x5", -1 ) );

}/* TEST_F(	UtilsTest, TestStringToInt ) */


}/* namespace { */ 
