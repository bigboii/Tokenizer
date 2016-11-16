	..,,<....
#include<iostream>
#include<string>
using namespace std;
int main
{
	char c = '\n';

	std::string fileSpec = "";
	fileSpec = "../Tokenizer/Test.cpp";
	str1 = " hue \"special \"double\" quote\"";
	string str1 = "Testing, \", special chars";

	str1 = "Testing, \', special chars";
	string str = "Testing Quoted Strings";
	
	//Testing C++ Comment
	/*
		Testing C style comment 2 
	*/
	/* Testing C style comment 1 */
	/** Testing C style comment 3 */
	..,,<....
	<>[](){}=:+-*/
	<<>>::++--==+= -= *= /=
	
	cout << "hello" << endl;

	c = '\"';
	
	for(int i=0; i<10;i++) {               //semi expression
		if(true)                           //semi expression
			if(true) {                     //semi expression
				if(true) cout<< "I'm a nested semi expression" <<endl;   //semi expression
			}
		}
	} 

	return 0;
}
