#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.4                                                           //
// Language:  C++, Visual Studio 2013                                //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Young Kyu Kim, Syracuse University, ykim127@syr.edu  //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.4 : Feb 8, 2016
 * - Added setSSC() and setSCP() to allow addition of Special Single and Double Characters
 * ver 3.0 : 11 Jun 2014
 * - first release of new design
 *
 * Planned Additions and Changes:
 * ------------------------------
 * - Return quoted strings as single token
 * - Return "[", "]", "{", "}", "(", ")"
 *   as single character tokens
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
	class ConsumeState;

	class Toker
	{
	public:
		Toker();
		~Toker();
		bool attach(std::istream* pIn);
		std::string getTok();
		bool canRead();
		bool getIsComment();
		ConsumeState* getConsumeState();
		void setSSC(char c);
		void setSCP(std::string pair);

		private:
		ConsumeState* pConsumer;

	};
}

#endif

