#ifndef _STOOGE_H_
#define _STOOGE_H_

#include <string>
#include <ostream>
using namespace std;

class Stooge
{
	public:
		int number;
		string name;
		string function;
		string line;

	public:
		Stooge( int number, string name, string function, string line ) : number(number), name(name), function(function), line(line) { }
};

ostream& operator<<(ostream& s, const Stooge& stooge ) {

    s << "Stooge:\n"
   	"\t" << "number=" << stooge.number << "'\n"
	"\t" << "name='" << stooge.name << "'\n" 
	"\t" << "function='" << stooge.function << "'\n"
	;

    return s;
}

#endif /* #ifndef _STOOGE_H_ */
