#include <iostream>
#include <map>
#include "LL1.h"
using namespace std;

int main()
{
	LL1 LL1A ,LL1B, LL1C, LL1D;

	cout << "LL1 Compiler Tester" << endl;
	//
	//LL1A.LoadFile("grammar1.txt");
	//cout << "Printing out the terms found for grammar 1" << endl;
	//LL1A.PrintTerms();
	//LL1A.FirstSet();
	//system("pause");

	//LL1B.LoadFile("grammar2.txt");
	//cout << "Printing out the terms found for grammar 2" << endl;
	//LL1B.PrintTerms();
	//system("pause");

	//LL1C.LoadFile("grammar3.txt");
	//cout << "Printing out the terms found for grammar 3" << endl;
	//LL1C.PrintTerms();
	//LL1C.FirstSet();
	//system("pause");

	LL1D.LoadFile("grammar4.txt");
	cout << "Printing out the terms found for grammar 4" << endl;
	LL1D.PrintTerms();
	LL1D.FirstSet();
	system("pause");
	return 0;
}