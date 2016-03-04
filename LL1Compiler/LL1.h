#ifndef  LL1_H
#define  LL1_H

#include<vector>
#include<string>
#include<map>

using namespace std;

class LL1
{
public:

	//constructor
	LL1();

	//destructor
	~LL1();

	//Loads the file and each grammar is described in the format of ex S = AB | BC
	//Each grammar rule exists on one line.
	//James
	void LoadFile(string filename);
	void ReadLine(string line);

	//left factors the grammar and creates additional rules as needed.
	void LeftFactor();

	//Put it in a data structure (FirstSet map) runs the checkFor for each grammar rule
	void FirstSet();

	//inserts the $ for the parent and uses the information from the first set and running checkFor.
	void FollowSet();

	//Prints to an output file
	void PrintTable();

	//Prints all Terms
	void PrintTerms() const;
		
	//handles lambdas
	//Uses terms and iterators findif
	bool CheckFor();

private:

	struct Term
	{
		string name;

		//Need to be initialized to null
		//sorted ideally
		//coud be references
		bool isStart;

		//since a term can be a string, each child group consists of terms and each term
		vector<vector<string>> childGroups;
		//removing this line perhaps for vector<ChildGroup> childGroups;
		vector<std::map<string, Term>::iterator> children;
	};


	//Recurses to leaves from start
	void FirstRecurse(Term current);

	//takes a vector and returns the longest matching string. Used in Left 
	string FindLongestMatchingString(vector<string> vec);

	//Inserts terms into the term group while performing the needed checking for existing terms
	void InsertTerm(Term term);
	
	//Returns true if a given term is a TerminalValue
	bool IsTerminal(Term t) const;

	//Inserts items into the LL1 table
	void InsertInTable(int x, int y, string line);
	
	//Contains all terminals and non terminals after left factoring
	map<string, Term> mTermGroup;

	//Contains the first set for each production line after running the FirstSet Function
	map<string, Term> mFirstSet;

	//Contains the follow set for each production line after running the FirstSet, and then FollowSet Function.
	map<string, Term> mFollowSet;
	
	//This needs to be dynamic due to grammars changing in size.
	map<int, map<int,string>> mTable;
};


#endif