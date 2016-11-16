/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.3                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Young Kyu Kim, ykim127@syr.edu                     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
Modified Helper Tokenizer code which finally tokenizes various tokens

Maintenance History:
====================
ver 3.3 : 08 Feb 2016
- Added project 1 requirements such as comments and quoted strings
- Added isComment flag in ConsumeState to aid SemiExpression to toggle comments on and off
- Tokenizes Quoted Strings
- Tokenizes C++ and C style comments
- Tokenizes Special single and pairs of chars
- Added feature to setSpecialSingleChars and setSpecialCharPairs(string scp)
ver 3.2 : Original Code
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <map>

//using namespace Scanner;
namespace Scanner
{

	class ConsumeState
	{
	public:
		ConsumeState();
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();

		//Q's functions
		ConsumeState* checkIsPunct(int char1, int char2);
		ConsumeState* checkQuotedString(int char1, int char2);
		bool isSpecialOneChar(int char1);
		ConsumeState* isSpecialTwoChar(int char1, int char2);
		
		//bool isComment = false;
		void setIsComment(bool flag) { isComment = flag; }
		bool getIsComment() { return isComment; }

		void setSpecialSingleChars(char ssc);
		void setSpecialCharPairs(std::string sdc);

		std::map<char, char> ssc_map = { { '<', '<' },{ '>','>' },{ '[', '[' },{ ']', ']' }
			,{ ']', ']' },{ '(', '(' },{ ')', ')' },{ '{', '{' }
			,{ '}', '}' },{ ':', ':' },{ '=', '=' },{ '+', '+' }
			,{ '-', '-' },{ '*', '*' },{ '\n', '\n' } };            //special single character map <key, value> ;

		/*		ssc_map = { { '<', '<' },{ '>','>' },{ '[', '[' },{ ']', ']' }
			,{ ']', ']' },{ '(', '(' },{ ')', ')' },{ '{', '{' }
			,{ '}', '}' },{ ':', ':' },{ '=', '=' },{ '+', '+' }
		,{ '-', '-' },{ '*', '*' },{ '\n', '\n' } };            //special single character map <key, value>*/

		std::map<std::string, std::string> sdc_map = { { "<<", "<<" },{ ">>",">>" },{ "::", "::" },{ "++", "++" }
												,{ "--", "--" },{ "==", "==" },{ "+=", "+=" },{ "-=", "-=" }
												,{ "*=", "*=" },{ "/=", "/=" } }; //special double character map

	protected:
		static std::string token;
		static std::istream* _pIn;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatSpecialOneChar;      //Qs: req #4, special 1 characters
		static ConsumeState* _pEatSpecialTwoChar;      //Qs: req #4, special 2 characters
		static ConsumeState* _pEatCComment;            //Qs: req #4, C style comments
		static ConsumeState* _pEatCppComment;          //Qs: req #4, C++ style comments
		static ConsumeState* _pEatSingleQuoteString;   //Qs; req #4, single quoted strings
		static ConsumeState* _pEatDoubleQuoteString;   //Qs: req #4, double quoted strings

	private:
		bool isComment = false;
	};
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
//int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;

ConsumeState* ConsumeState::_pEatSingleQuoteString = nullptr;
ConsumeState* ConsumeState::_pEatDoubleQuoteString = nullptr;     //Qs: added to incorporate quoted strings
ConsumeState* ConsumeState::_pEatSpecialOneChar = nullptr;  //Qs: added to incorporate single special char
ConsumeState* ConsumeState::_pEatSpecialTwoChar = nullptr;  //Qs: added to incorporate double special char

void testLog(const std::string& msg);

ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();
	if (std::isspace(currChar) && currChar != '\n')
	{
		isComment = false;
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		isComment = true;
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		isComment = true;
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		//isComment = false;
		testLog("state: eatNewLine");
		return _pEatSpecialOneChar;
	}
	if (std::isalnum(currChar))
	{
		isComment = false;
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (ispunct(currChar))
	{
		isComment = false;
		return checkIsPunct(currChar, chNext);
	}
	if (!_pIn->good())
		return _pEatWhitespace;
	throw(std::logic_error("invalid type"));
}

/* Q's functions*/

//Determine next state from a punctuator character
ConsumeState* ConsumeState::checkIsPunct(int char1, int char2)
{
	if (currChar == '\"' || currChar == '\'')
		return checkQuotedString(char1, char2);
	if (isSpecialOneChar(currChar))
		return isSpecialTwoChar(char1, char2);   //check for special double Char
	else
		return _pEatPunctuator;
}

//Check quoted strings
ConsumeState* ConsumeState::checkQuotedString(int char1, int char2)
{
	if (char1 == '\"')             	  //Case: Quoted Strings
	{
		if (char2 == '\"')
		{
			currChar = _pIn->get();       //skip over empty quoted strings
			currChar = _pIn->get();
		}
		else
		{
			_pState = _pEatDoubleQuoteString;
			return _pEatDoubleQuoteString;
		}
	}
	if (char1 == '\'')
	{
		if (char2 == '\'')
		{
			currChar = _pIn->get();     //Skip over empty quoted strings
			currChar = _pIn->get();
		}
		else
		{
			_pState = _pEatSingleQuoteString;
			return _pEatSingleQuoteString;
		}
	}

	//If its not a quoted string, check for Special double character
	return _pEatPunctuator;
}
/*
//Check if character is a special single character 
ConsumeState* ConsumeState::isSpecialOneChar(int char1)
{
	return
}
*/

//sets a special character that is hardcoded
void ConsumeState::setSpecialSingleChars(char ssc)
{
	ssc_map.insert(std::pair<char, char>(ssc, ssc));
}

//sets a special character pair that is hardcoded
void ConsumeState::setSpecialCharPairs(std::string sdc)
{
	sdc_map.insert(std::pair<std::string, std::string>(sdc, sdc));
}

//Check if pair of chracters are special double character <<, >>, ::, ++, --, ==, +=, -=, *=, /=, \n
ConsumeState* ConsumeState::isSpecialTwoChar(int char1, int char2)
{
	std::string exp = "";
	exp += char1;
	exp += char2;

	std::map<std::string, std::string>::iterator it = sdc_map.find(exp);     //check if char is in map
	if (it != sdc_map.end())
	{
		//if (exp == )
			return _pEatSpecialTwoChar;
	}

	_pState = _pEatSpecialOneChar;
	return _pEatSpecialOneChar;
}

class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

//Qs: req #4, C++ style comments
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		//isComment = true;
		//std::cout << "Eating CPP Comment! ";
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			token += currChar; //Q's Line: add comments to token
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
		_pState = nextState();
	}
};

//Qs: req #4, C style comments
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		//std::cout << "Eating C Comment! ";
		//isComment = true;
		token.clear();
		int chNext;
		//std::cout << "\n  eating C comment";
		token += currChar;
		currChar = _pIn->get();

		do {
			token += currChar; //Q's Line: add comments to token
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			chNext = _pIn->peek();
			if (currChar == '*' && chNext == '/')   //  */
			{
				token += currChar;
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				break;
			}
		} while (1/*currChar != '*' && chNext != '/'*/);

		_pState = nextState();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		bool isSSC = false;
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			/*
			std::map<char, char>::iterator it = ConsumeState::ssc_map.find(currChar);     //check if char is in map
			if (it != ssc_map.end())
				isSSC = true;
			else
				isSSC = false;
			*/

			if (isSpecialOneChar(currChar) == true)
			if(isSSC == true)
			{
				_pIn->get();
				break;
			}

				token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();

		} while (ispunct(currChar) && !isSpecialOneChar(currChar));  //while(currChar != Special Single Char)
	}
};

//Check if character is a special single char
bool ConsumeState::isSpecialOneChar(int char1)
{
	std::map<char, char>::iterator it = ssc_map.find(char1);     //check if char is in map
	if (it != ssc_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

/* 
	Q's new ConsumeState classes
*/
class EatSingleQuoteString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		currChar = _pIn->get();       //skip first single quote
		int chNext = _pIn->peek();

		if (currChar != '\\')
		{
			token += currChar;
			currChar = _pIn->get();       //skip second single quote		
		}
		else
		{
			if (chNext == '\'')
			{
				currChar = _pIn->get();    //skip escape character	
			}
			else
			{
				token += currChar;
				currChar = _pIn->get();
			}

			token += currChar;
			currChar = _pIn->get();
		}

		_pState = nextState();
	}
};

class EatDoubleQuoteString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		bool isTerminate = false;
		int chNext;                    //char after current
		int chPrev;                    //char before current
		currChar = _pIn->get();                    //skip first double quote
		chNext = _pIn->peek();

		do 
		{
			//1. Check if escape character exists
			if (currChar == '\\')
			{
				if (chNext == '\"')
				{
					currChar = _pIn->get();   //skip escape character					
				}
			}

			//2. Add internal string
			token += currChar;
			chPrev = currChar;
			currChar = _pIn->get();

			//3. Detect Terminating Character
			if (currChar == '\"' && chPrev != '\\')   //terminating quote?
			{
				currChar = _pIn->get();               //skip terminating quote
				break;
			}

		} while (1/*(currChar != '\"' && std::isspace((chNext))) || (currChar != '\"' && chNext != ';')*/);  // " ; or  ";
		_pState = nextState();
	}
};

class EatSpecialOneChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special one character";

		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

class EatSpecialTwoChar : public ConsumeState
{
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special two character";
		int count = 0;
		do
		{
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			count++;
		} while (count != 2);
		_pState = nextState();
	}

};


ConsumeState::ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pEatSingleQuoteString = new EatSingleQuoteString();
		_pEatDoubleQuoteString = new EatDoubleQuoteString();        //Qs: Initialize Quoted Strings
		_pEatSpecialOneChar = new EatSpecialOneChar();  //Qs: Initialize Special one chars
		_pEatSpecialTwoChar = new EatSpecialTwoChar();  //Qs: Initialie Special two chars
		_pState = _pEatWhitespace;                //Q's notes, pState is initially pEatWhiteSpace
		isComment = false;

		//ssc_map.insert(std::pair<char, char>('<', '<'));
		
		/*
		{ { '<', '<' },{ '>','>' },{ '[', '[' },{ ']', ']' }
			,{ ']', ']' },{ '(', '(' },{ ')', ')' },{ '{', '{' }
			,{ '}', '}' },{ ':', ':' },{ '=', '=' },{ '+', '+' }
		,{ '-', '-' },{ '*', '*' },{ '\n', '\n' } };            //special single character map <key, value>
		*/
	}
}

ConsumeState::~ConsumeState()
{
	static bool first = true;
	if (first)
	{
		first = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatSingleQuoteString;
		delete _pEatDoubleQuoteString;
		delete _pEatSpecialOneChar;
		delete _pEatSpecialTwoChar;
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

//returns isComment
bool Toker::getIsComment()
{
	//return pConsumer->isComment;
	return pConsumer->getIsComment();
}

//relays Special Single character from tokenizer to ConsumeState
void Toker::setSSC(char c)
{
	pConsumer->setSpecialSingleChars(c);
}

//relays Special character pair from tokenizer to ConsumeState
void Toker::setSCP(std::string pair)
{
	pConsumer->setSpecialCharPairs(pair);
}

//returns pConsumer
ConsumeState* Toker::getConsumeState()
{
	return pConsumer;
}

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}


//----< test stub >--------------------------------------------------

//#define TEST_TOKENIZER
#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/TokenizerTest.cpp";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}

	Toker toker;
	toker.attach(&in);

	//SetSpecialChar test
	toker.getConsumeState()->setSpecialSingleChars('.');  //Semicolons become special char
	toker.getConsumeState()->setSpecialCharPairs("..");   //{} becomes special char pairs

	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "\\n";
		std::cout << "\n -- " << tok;
	} 	while (in.good())
	std::cout << "\n\n";
	return 0;
}
#endif
