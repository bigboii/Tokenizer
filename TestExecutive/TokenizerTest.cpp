#include<iostream>
int main
{
	char c = '\n';
	string str = "Testing Quoted Strings";
	str = "\"special double quote\"";
	str = "Testing, \", special chars\t";
	str = "Testing, \', special chars\n";
	str = "";
	
	//Testing C++ Comment
	/* Testing C style comment 1 */
	/*
		Testing C style comment 2 
	*/
	/** Testing C style comment 3 */
	<>[](){}=:+-*/
	<<>>::++--==+= -= *= /=
	
	cout << "hello" << endl;           //random c++ comment
	c = '\"';                          /* Random C comment*/
	
	for(int i=0; i<10;i++) 
	{
		if(true)
		{	
			if(true) 
			{
				if(true) cout<< "I'm a nested semi expression" <<endl;
			}
		}
	} 

	return 0;
}