/***********************************************************
* Author:					James Shaver & Grant Kropf
* Date Created:				3/1/2016
* Last Modification Date:	3/14/2016
* Lab Number:				CST 320 Lab 3
* Filename:					LL1Tester.cpp
*
***********************************************************/
#include <iostream> 
#include <map>
#include "LL1.h"
using namespace std;

int main()
{
	LL1 LL1A ,LL1B, LL1C;

	cout << "LL1 Compiler Tester" << endl;
	
	LL1A.LoadFile("grammar1.txt");	
	//cout << "Printing out the terms found for grammar 1" << endl;
	//LL1A.PrintTerms();

	cout << "First set:" << endl;
	LL1A.FirstSet();
	LL1A.PrintFirstSet();

	cout << "Follow set:" << endl;
	LL1A.FollowSet();
	LL1A.PrintFollowSet();
	cout << "LL1 Table:" << endl;
	LL1A.GenerateTable();
	LL1A.PrintTable();

	system("pause");

	LL1B.LoadFile("grammar2.txt");
	//cout << "Printing out the terms found for grammar 2" << endl;
	//LL1B.PrintTerms();

	cout << "First set:" << endl;
	LL1B.FirstSet();
	LL1B.PrintFirstSet();

	cout << "Follow set:" << endl;	
	LL1B.FollowSet();
	LL1B.PrintFollowSet();
	LL1B.GenerateTable();
	LL1B.PrintTable();
	system("pause");

	//
	LL1C.LoadFile("grammar3.txt");
	LL1C.LeftFactor();
	//cout << "Printing out the terms found for grammar 3" << endl;
	//LL1C.PrintTerms();
	cout << "First set:" << endl;
	LL1C.FirstSet();
	LL1C.PrintFirstSet();

	cout << "Follow set:" << endl;
	LL1C.FollowSet();
	LL1C.PrintFollowSet();
	LL1C.GenerateTable();
	LL1C.PrintTable();
	system("pause");

	return 0;
}