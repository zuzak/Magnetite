#ifndef _UTIL_H_
#define _UTIL_H_
#include "prerequisites.h"

class Util
{
public:
	// String split function
	static stringvector &split(const std::string &s, char delim, stringvector &elems) {
		std::stringstream ss(s);
		std::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
	static stringvector split(const std::string &s, char delim) {
		stringvector elems;
		return Util::split(s, delim, elems);
	}

	static void log(const std::string text)
	{
		std::cout << "| " << text << std::endl;
	}

	static std::string toString(int number)
	{
		return itoa(number, 10);
	}

	/**
	 * C++ version std::string style "itoa":
	*/
	static std::string itoa(int value, int base) {
		enum { kMaxDigits = 35 };
		std::string buf;
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	
		// check that the base if valid
	
		if (base < 2 || base > 16) return buf;
		int quotient = value;
		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
			quotient /= base;
		
		} while ( quotient );

		// Append the negative sign for base 10
		if ( value < 0 && base == 10) buf += '-';
		std::reverse( buf.begin(), buf.end() );
		return buf;
	}

	// Causes Ambiguity errors.
	/*static std::string toString(bool t)
	{
		if( t )
			return "true";
		return "false";
	}*/
};

#endif