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
	
	//Prints the contents of FirstSet
	void PrintFirstSet() const;

	//Prints the contents of FollowSet
	void PrintFollowSet() const;
	
	//adds items from the first and follow set into table.
	void GenerateTable();

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
		
		vector<std::map<string, Term>::iterator> children;
	};


	//handles lambdas
	//Uses terms and iterators findif
	//had to move below Term definition or throws compiler error :/
	bool CheckFor(string, char);
	string CrossReference(string non_terminal, string terminal);
	bool IsUpper(string line);
	bool IsInLambdaSet(string term);
	void FirstSetRecurse(string term);
	bool replace(std::string& str, const std::string& from, const std::string& to); //find replace of strings

	//Generate Lambda set, returns vector of strings, each string a LHS identifier
	vector<string> GenerateLambdaSet();
		
	//takes a vector and returns the longest matching string. Used in Left 
	string FindLongestMatchingString(vector<string> vec);

	//A helper method to avoid duplicate in the firstSet
	void InsertFirstSet(string term, string firstSetItem);

	//A helper method to avoid duplicate in the followSet
	void InsertFollowSet(string term, string followSetItem);

	//Inserts terms into the term group while performing the needed checking for existing terms
	void InsertTerm(Term term);
	
	//Returns true if a given term is a TerminalValue
	bool IsTerminal(Term t) const;

	//Inserts items into the LL1 table
	void InsertInTable(int x, int y, string line);
	
	//Contains all terminals and non terminals after left factoring
	map<string, Term> mTermGroup;

	//Contains the first set for each production line after running the FirstSet Function
	//mFirstSet was getting tricky to think about, I just put it into something simpler
	map<string, vector<string>> mFirstSet;

	//Contains the follow set for each production line after running the FirstSet, and then FollowSet Function.
	map<string, vector<string>> mFollowSet;

	//This needs to be dynamic due to grammars changing in size.
	map<int, map<int,string>> mTable;

	vector<string> mLambdaSet;
};


#endif