#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "../Tokenizer/Test.cpp";

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
			tok = "\\n";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	return 0;
}
#endif