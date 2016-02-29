#include <iostream>
#include <map>
#include "LL1.h"
using namespace std;

int main()
{
	//map<int, string> test;
	//for (auto item : test)
	//{

	//}
	////here is a random comment

	LL1 LL1A;
	LL1 LL1B;
	LL1 LL1C;

	cout << "LL1 Compiler Tester" << endl;
	
	LL1A.LoadFile("grammar1.txt");
	cout << "Printing out the terms found for grammar 1" << endl;
	LL1A.PrintTerms();
	system("pause");

	LL1B.LoadFile("grammar2.txt");
	cout << "Printing out the terms found for grammar 2" << endl;
	LL1B.PrintTerms();
	system("pause");

	LL1C.LoadFile("grammar3.txt");
	cout << "Printing out the terms found for grammar 3" << endl;
	LL1C.PrintTerms();
	system("pause");

	return 0;
}