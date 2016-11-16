#include<iostream>
#include<string>
using namespace std;

class test
{
public:
	void testFunc1();
	virtual int testFunc2() = 0;

protected:
	int testVar1;
	int testVar2;
	
private:
	int testVar3;
	int testVar4;
};

int test::testFunc2()
{
	return 0;
}

int main
{
	str1 = "\"special double quote\"";
	string str1 = "Testing, \", special chars";
	str1 = "Testing, \', special chars";
	string str = "Testing Quoted Strings";
	
	//Testing C++ Comment
	/*
		Testing C style comment 2 
	*/
	/* Testing C style comment 1 */
	/** Testing C style comment 3 */
	int i = 0;
	i++;
	i--;
	i += 10;
	i -= 20;
	i *= 30;
	i /= 40;
	cout << "hello" << endl;

	for(int i=0; i<10;i++) {               //semi expression
		if(true)                           //semi expression
			if(true) {                     //semi expression
				if(true) cout<< "I'm a nested semi expression" <<endl;   //semi expression
			}
		}
	} 

	return 0;
}