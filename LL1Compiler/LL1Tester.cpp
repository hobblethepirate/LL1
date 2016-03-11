#include <iostream>
#include <map>
#include "LL1.h"
using namespace std;

int main()
{
	LL1 LL1A ,LL1B, LL1C, LL1D;

	cout << "LL1 Compiler Tester" << endl;
	//
	LL1A.LoadFile("grammar1.txt");	
	//cout << "Printing out the terms found for grammar 1" << endl;
	//LL1A.PrintTerms();

	cout << "First set:" << endl;
	LL1A.FirstSet();
	LL1A.PrintFirstSet();

	cout << "Follow set:" << endl;
	LL1A.FollowSet();
	LL1A.PrintFollowSet();
	system("pause");

	LL1B.LoadFile("grammar2.txt");
	cout << "Printing out the terms found for grammar 2" << endl;
	LL1B.PrintTerms();

	cout << "First set:" << endl;
	LL1B.FirstSet();
	LL1B.PrintFirstSet();

	cout << "Follow set:" << endl;	
	LL1B.FollowSet();
	LL1B.PrintFollowSet();
	system("pause");

	LL1C.LoadFile("grammar3.txt");
	cout << "Printing out the terms found for grammar 3" << endl;
	LL1C.PrintTerms();
	cout << "First set:" << endl;
	LL1C.FirstSet();
	LL1C.PrintFirstSet();

	cout << "Follow set:" << endl;
	LL1C.FollowSet();
	LL1C.PrintFollowSet();
	system("pause");

	LL1D.LoadFile("grammar4.txt");
	cout << "Printing out the terms found for grammar 4" << endl;
	LL1D.PrintTerms();
	cout << "First set:" << endl;
	LL1D.FirstSet();
	LL1D.PrintFirstSet();

	cout << "Follow set:" << endl;
	LL1D.FollowSet();
	LL1D.PrintFollowSet();

	system("pause");
	return 0;
}