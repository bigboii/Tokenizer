/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
Helper code that does not attempt to handle all tokenizing
special cases like escaped characters.
*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>


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
protected:
	static std::string token;
	static std::istream* _pIn;
	static int currChar;
	static ConsumeState* _pState;
	static ConsumeState* _pEatWhitespace;
	static ConsumeState* _pEatPunctuator;
	static ConsumeState* _pEatAlphanum;
	static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatSpecialOneChar;  //Qs: req #4, special 1 characters
	static ConsumeState* _pEatSpecialTwoChar;  //Qs: req #4, special 2 characters
	static ConsumeState* _pEatCComment;        //Qs: req #4, C style comments
	static ConsumeState* _pEatCppComment;      //Qs: req #4, C++ style comments
	static ConsumeState* _pEatQuoteString;     //Qs: req #4, quoted strings
};

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

ConsumeState* ConsumeState::_pEatQuoteString = nullptr;     //Qs: added to incorporate quoted strings
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
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (ispunct(currChar))
	{
		if (currChar == '\"')             	  //Case: Quoted Strings
		{
			_pState = _pEatQuoteString;
			return _pEatQuoteString;
		}
		/*
		if (currChar == '\"')               //Case: Special one characters
		{

		}
		if(currChar == '<')
		{
		if (chNext == '<')              //Double?
		{
		_pState = _pEatSpecialTwoChar;
		return _pEatSpecialTwoChar;
		}

		_pState = _pEatSpecialOneChar;
		return _pEatSpecialOneChar;
		}
		if (currChar == '>')
		{
		if (chNext == '>')              //Double?
		{
		_pState = _pEatSpecialTwoChar;
		return _pEatSpecialTwoChar;
		}

		_pState = _pEatSpecialOneChar;
		return _pEatSpecialOneChar;
		}
		if (currChar == '[')
		{

		}
		if (currChar == ']')
		{

		}
		if (currChar == ':')
		{
		if (chNext == ':')              //Double?
		{
		_pState = _pEatSpecialTwoChar;
		return _pEatSpecialTwoChar;
		}

		_pState = _pEatSpecialOneChar;
		return _pEatSpecialOneChar;
		}
		*/

		//std::cout << "\n  state: eatPunctuator";
		_pState = _pEatPunctuator;
		return _pEatPunctuator;
	}
	if (!_pIn->good())
		return _pEatWhitespace;
	throw(std::logic_error("invalid type"));
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
			/*
			if (currChar == '*' && chNext == '*')   //  /**
			{
			token += currChar;
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
			}
			*/
		} while (1/*currChar != '*' && chNext != '/'*/);
		//_pIn->get();
		//currChar = _pIn->get();
		_pState = nextState();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar));
	}
};

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

/* Q's new ConsumeState classes
static ConsumeState* _pEatSpecialOneChar;  //Qs: req #4, special 1 characters
static ConsumeState* _pEatSpecialTwoChar;  //Qs: req #4, special 2 characters
static ConsumeState* _pEatQuoteString;     //Qs: req #4, quoted strings
*/

class EatQuoteString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		int chNext;
		//std::cout << "\n  eating Quoted String";
		do {
			token += currChar; //Q's Line: add comments to token
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			chNext = _pIn->peek();

			if (currChar == '\n')
			{
				//Already done
			}
			if (currChar == '\\' && (chNext == '\"') || chNext == '\'') //found special char " '
			{
				token += currChar;
				token += chNext;
				currChar = _pIn->get();
				currChar = _pIn->get();
				chNext = _pIn->peek();
			}
			if (currChar == '\"')                                        //found terminating "
			{
				token += currChar;
				currChar = _pIn->get();
				break;
			}
		} while (1/*(currChar != '\"' && std::isspace((chNext))) || (currChar != '\"' && chNext != ';')*/);  // " ; or  ";
		_pState = nextState();
	}
};

class EatSpecialOneChar : public ConsumeState
{
	virtual void eatChars()
	{

	}
};

class EatSpecialTwoChar : public ConsumeState
{
	virtual void eatChars()
	{
		token.clear();
		do
		{
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar));
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
		_pEatQuoteString = new EatQuoteString();        //Qs: Initialize Quoted Strings
		_pEatSpecialOneChar = new EatSpecialOneChar();  //Qs: Initialize Special one chars
		_pEatSpecialTwoChar = new EatSpecialTwoChar();  //Qs: Initialie Special two chars
		_pState = _pEatWhitespace;                //Q's notes, pState is initially pEatWhiteSpace
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
		delete _pEatQuoteString;
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

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	//std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	return 0;
}
#endif
