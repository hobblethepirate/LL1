#include "LL1.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

LL1::LL1() :mTable()
{

}

LL1::~LL1()
{

}

void LL1::LoadFile(string filename)
{
	ifstream file;	//file variable to open
	string line;	// a temporary variable for hold each line read from the file
	file.open(filename);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			ReadLine(line);
		}
		file.close();
	}
	else
	{
		cout << "Unable to locate file: " << filename << endl;
	}
}

void LL1::ReadLine(string line)
{
	//Removing comments and empty lines
	
	if (line.substr(0, 2).compare("//") != 0 && line.compare("") != 0)
	{	
		
		string lhs, rhs, tempLine;
		Term temp1, temp2;
		
		int rightIndex, leftIndex = 0, a;

		int pos = line.find('=');
		
		if (pos == -1)
		{
			cout << "Throwing out line: " << line << " because it is unreadable." << endl;
			return;
		}
		//lhs may have spaces
		lhs = line.substr(0, pos);
		
		for (a = 0; a < lhs.length(); a++)
		{
			if (lhs[a] == ' ')
			{
				lhs.erase(lhs.find_last_not_of(" \n\r\t") + 1);
			}
		}
		temp1.name = lhs;
		if (temp1.name[0] == '*')
		{
			temp1.name =temp1.name.substr(1, temp1.name.length() - 1);
			temp1.isStart = true;
		}

		rhs = line.substr((pos + 1), (line.length() - pos));
		
		//push back the final child group
		//rhs may have spaces and pipes
		vector<string> tempVector;
		temp1.childGroups.push_back(tempVector);
		int groupNum = 0, posNum = 0;

		for (rightIndex = 0; rightIndex < rhs.length(); rightIndex++)
		{
			if (rhs[rightIndex] == '|' || rhs[rightIndex] == ' ')
			{
				//everything to the left is a terminal or non terminal
				if (rightIndex == 0 && rhs[rightIndex] == ' ')
				{
					//skip this
					leftIndex++;
				}
				else
				{
					
					temp2.name = rhs.substr(leftIndex, rightIndex-leftIndex);	
					
					temp1.childGroups[groupNum].push_back(temp2.name);
					InsertTerm(temp2);
					temp1.children.push_back(mTermGroup.find(temp2.name));
					if (rhs[rightIndex] == '|')
					{
						temp1.childGroups.push_back(tempVector);
						groupNum++;
					}
					leftIndex = rightIndex + 1;
					temp2.name = "";
				}
			}
		}
		//everything left over needs to be inserted
		temp2.name = rhs.substr(leftIndex, (rightIndex - leftIndex));
		temp1.childGroups[groupNum].push_back(temp2.name);
		InsertTerm(temp2);
		temp1.children.push_back(mTermGroup.find(temp2.name));
		InsertTerm(temp1);

	}
}


//used for LeftFactor method, will find the Longest matching string so it can LeftFactor that string
//Once we get longest matching string in left factoring, we can replace the original string with the new term name, and create the new term
//with the LongestMatchingString and lambda (?)
//this was a btch to figure out btw, the c_str() function is needed otherwise it throws an error for "cannot cast string to const * char"
//remember, you must load all the strings to compare into a vec before you run this
string LL1::FindLongestMatchingString(vector<string> vec)
{
	for (int i = 0; i < vec.size(); ++i) //the vec.size() - 1 could probably be changed to vec.size(), changed to vec.size
	{
		for (int k = 0; k < vec.size(); ++k) //was .size() - 1
		{
			for (int j = vec[i].length(); j > 0; --j)
			{
				if (i == k) ;
				else if (strcmp(vec[i].substr(0, j).c_str(), vec[i + 1].substr(0, j).c_str()) == 0)
				{
					return vec[i].substr(0, j);
				}
			}
		}
	}
	return ""; //if no match found, returns empty string;
}


void LL1::InsertTerm(Term term)
{
	if (mTermGroup.find(term.name) != mTermGroup.end())
	{
		//a redefinition should occur here
		if (mTermGroup.at(term.name).children.empty() == true)
		{
			if (term.children.empty() == true)
			{
				return;
			}
			else
			{
				//the current version of the term has no children
				mTermGroup.at(term.name).children = term.children;
				mTermGroup.at(term.name).childGroups = term.childGroups;
			}
		}
		else
		{
			//the current version of the term has children
			for (auto child : term.children)
			{		
				mTermGroup.at(term.name).children.push_back(child);	
			}
			for (auto childGroup : term.childGroups)
			{
				mTermGroup.at(term.name).childGroups.push_back(childGroup);
			}
		}
		return;
	}

	mTermGroup[term.name] = term;
}

void LL1::PrintTerms() const
{
	for (auto term : mTermGroup)
	{
		cout <<"["<<term.second.name<<"]";
		if (!term.second.children.empty())
		{
			cout << " has the children: ";
			for (int count = 0; count < term.second.children.size(); count++)
			{
				cout << term.second.children[count]->second.name << ", ";
			}
			cout << endl;
			cout << " and has the childgroups: " << endl;
			//if at term has children it has a childgroup
			for (auto childGroup : term.second.childGroups)
			{
				if (childGroup.empty() != true)
				{
					for (int count = 0; count < childGroup.size(); count++)
					{
						cout << childGroup[count];
					}
					cout << " | " ;
				}
			}
			cout << endl;
		}
		else
		{
			cout << endl;
			
		}
	}

}


//left factors the grammar and creates additional rules as needed.
//happens only on the same parent
void LL1::LeftFactor()
{
	//any new unit productions will be given a prime character '

	//loop through every term in mTermGroup and perform the following:
	for (auto term : mTermGroup)
	{
		string termName = term.second.name;
		string oldTermName = termName;
		termName += '\''; // this will be the left factored term name if we create one
		vector<string> termVec;

		//if not leaf, do your bidding... and left factor the term
		if (!term.second.children.empty())
		{
			//place all children for given term into a temporary vector, which should make the calculations for comparison much simpler
			for (int i = 0; i < term.second.children.size(); ++i)
			{
				termVec.push_back(term.second.children[i]->second.name);
				//we now have a full vector in termVec to do the left factorizations
				//steps as follows:
				//1. use strncmp to figure out matches
				//2. create a new term with lambda and the matching 'magic string'
					//you do a strncmp comparison on length of a given string to all others, if it ever hits 0, it looks for all matching 0, and declares that section the 'magic string'
				//3. replace all instances of magic string with the termName

			}
			string longest_string = FindLongestMatchingString(termVec);

			if (longest_string != "")
			{
				//create new term with the longest_string and lambda
				Term temp_term;
				temp_term.name = termName;
				
				Term new_term;
				Term lambda;
				new_term.name = longest_string;
				lambda.name = "?";

				InsertTerm(new_term);
				InsertTerm(lambda);
				temp_term.children.push_back(mTermGroup.find(new_term.name));
				temp_term.children.push_back(mTermGroup.find(lambda.name));

				InsertTerm(temp_term);

				//replace instances of longest string with the new term's name

				//***TODO, need to modify here with replacement code for original string with the name
				//need to modify the mTermGroup with the oldTermName.name field and modify all strings that look like longest_string with termName

			}

		}

	}
}

//Put it in a data structure (FirstSet map) runs the checkFor for each grammar rule
void LL1::FirstSet()
{
	//Teacher's notes on the implementation
	//1. Generate a set of non - terminals that can generate a lambda, called it the lambda set.
	//	2. If B’s production begins with a terminal(eg.B->aX), then add a to First(B).
	//	3. If B is in the lambda set, then add lambda to First(B)
	//	4. If B begins with a non - terminal X(eg.B->XY)
		//	a.Compute First(X) and add First(X) – lambda to First(B)
		//	b.If X is in the lambda set, then add First(remainder) – lambda to First(B)
	


	// 1. Generate lambda set using GenerateLambdaSet
	vector<string> lambdaSet = GenerateLambdaSet();
	char c;

	// 2. if B begins with a terminal x, add terminal x to First(B)
		//map<string, Term> mFirstSet;
		//temp1.children.push_back(mTermGroup.find(temp2.name));
	for (auto term : mTermGroup)
	{
		//term.first is set name
		for (auto child : term.second.childGroups)
		{
			//checks each character, childGroup[i], of each childGroup string 
			// 2.if child[0] is lowercase, add it to First(X), where X is term.first
			string s = child[0];
			char c = s[0];
			
			if (c >= 'a' && c <= 'z')
			{ //true if char is lowercase
				//add to First(X), where X = term.first
				//	map<string, Term> mFirstSet; this is difficult to wrap my head around, I'm going to attempt a simpler data structure
				
				cout << "FirstSet() call -- char: " << c << " has been added to grantFirstSet for LHS " << term.first << endl;
				//warning as this will create duplicates sometimes

				mFirstSet[term.first].push_back(string(1, c)); //this pushes the char to the grantFirstSet

				//remove any duplicates that were created, i tried alternative methods but they didn't work
				std::sort(mFirstSet[term.first].begin(), mFirstSet[term.first].end());
				mFirstSet[term.first].erase(unique(mFirstSet[term.first].begin(), mFirstSet[term.first].end()), mFirstSet[term.first].end());
			}

		}

	}
	
	/*//Go through the list of terms, find start
	for (auto term : mTermGroup)
	{
		if (term.second.isStart == true)
		{
			//Recurse to leaves and calculate the first set of that leaf's parent
			FirstRecurse(term.second);
			//Replace non Terminals first sets with actual first sets
			break;
		}
	}*/

}

void LL1::FirstRecurse(Term current)
{
	if (current.children.empty() == true)
	{		
		return;
	}
	else
	{
		//This child term has kids we should calculate it's first set first
		for (auto child : current.children)
		{

			FirstRecurse(child->second);			
		}
		//next we need to calculate the current term's first set
		for (auto childGroup : current.childGroups)
		{
			//start with the first childGroup
			//Insert it into the first set even if it's non terminal
			
		}
	}

}

//inserts the $ for the parent and uses the information from the first set and running checkFor.

void LL1::FollowSet()
{
	//-Look for non - terminals on the right - hand side of the productions.
	//	- Compute what could come after those non - terminals
	//	- Use First set information
	//	- $ - denotes end of input

	//
		//1. Put $ in Follow(S) if S is the start symbol
		//2. If there is a production of the form : A->By where y is a terminal, add y to Follow(B)
		//3. If there is a production of the form : A->BC where C is a non - terminal, then add First(C) – lambda
		//to Follow(B)
		//4. If there is a production of the form A->BC, then add Follow(A) to Follow(C)
		//5. If there is a production of the form A->BC and C is in the lambda set, then add Follow(A) to
		//Follow(B)
		//6. If there is a production of the form A->BCD and C is in the lambda set, then add First(D) – lambda to
		//the Follow(B).
	vector<string> lambdaSet = GenerateLambdaSet();
	
	for (auto term : mTermGroup)
	{
		if (term.second.isStart == true)
		{
			//	1. Put $ in Follow(S) if S is the start symbol	
			mFollowSet[term.second.name].push_back("$");
				
		}
		if (IsTerminal(term.second) == false)
		{
			//no follow set exists for terminal terms.
			for (auto childGroup : term.second.childGroups)
			{
				
				if (childGroup.size >=1 && IsUpper(childGroup[0]) == true)
				{
					//	2. If there is a production of the form : A->By where y is a terminal, add y to Follow(B)
					if (IsUpper(childGroup[1]) == false)
					{
						mFollowSet[term.second.name].push_back(childGroup[1]);
					} 
					else
					{
						//non terminal second term

						//	3. If there is a production of the form : A->BC where C is a non - terminal, then add First(C) – lambda
						//	to Follow(B)
						for (auto firstSetItem : mFirstSet[childGroup[1]])
						{
							if (firstSetItem != "?")
							{
								mFollowSet[childGroup[0]].push_back(firstSetItem);
							}
						}
						//	4. If there is a production of the form A->BC, then add Follow(A) to Follow(C)						
						//This step may not work properly depending on the order of terms follow set A is in the proccess of changing
						for (auto followSetItem : mFollowSet[term.second.name])
						{
							mFollowSet[childGroup[1]].push_back(followSetItem);
						}



						for (auto lambdaSetItem : lambdaSet)
						{
							
							if (lambdaSetItem.compare(childGroup[1]) == 0)
							{
								//current second

								//	5. If there is a production of the form A->BC and C is in the lambda set, then add Follow(A) to
								//	Follow(B)
								for (auto followSetItem : mFollowSet[term.second.name])
								{
									mFollowSet[childGroup[0]].push_back(followSetItem);
								}
								//	6. If there is a production of the form A->BCD and C is in the lambda set, then add First(D) – lambda to
								//	the Follow(B).
								if (childGroup.size >= 2 && IsUpper(childGroup[2])==true)
								{
									for (auto firstSetItem : mFirstSet[childGroup[2]])
									{
										if (firstSetItem != "?")
										{
											mFollowSet[childGroup[1]].push_back(firstSetItem);
										}
									}
								}
								break;
							}
	
						}
					}			
				}
			}
		}
	}
}

//Prints to the console
void LL1::PrintTable()
{
	if (mTable.empty() == true)
	{
		cout << "There is nothing to print." << endl;
		return;
	}
	
	for (auto col : mTable)
	{
		for (auto row : col.second)
		{

			cout <<"|"<< row.second;		
			
		}
		cout << "|" << endl;
		cout << endl;
	}
}

bool LL1::IsUpper(string line)
{
	for (auto ch : line)
	{
		if (isupper(ch) == false)
		{
			return false;
		}
	}
}

//returns true if the given term is Terminal Value (empty children)
bool LL1::IsTerminal(Term t) const
{
	return t.children.empty();
}


//handles lambdas
//Uses terms and iterators findif
//pretty much only useful for finding lambdas in a LHS production
bool LL1::CheckFor(string s, char c)
{
	//example usage:
		//string s = "C";
		//char c = '?';
		//cout << "Checking " << s << " For " << c << " RESULT:" << CheckFor(s, c);
			//returns true if ? is found in C, checks all characters in all productions

	std::size_t found;

	//for each item in mTermGroup
	for (auto item : mTermGroup)
	{
		if (item.first == s) //this item's name matches the string (lhs S, A, B, C match)
		{
			for (auto child : item.second.childGroups)
			{
				//for each childgroup
				//look for matching character
				for (int i = 0; i < child.size(); ++i)
				{
					found = child[i].find(c);
					if (found != std::string::npos)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}


vector<string> LL1::GenerateLambdaSet()
{
	//returns an empty vector if fails to find any lambdas
	//otherwise, will return a vector of strings, each string containing a production that has lambda
	vector<string> vec;
	std::size_t found;

	//for each item in mTermGroup
	for (auto item : mTermGroup)
	{
		for (auto child : item.second.childGroups)
		{
			//for each childgroup
			//look for matching character
			for (int i = 0; i < child.size(); ++i)
			{
				found = child[i].find('?');
				if (found != std::string::npos)
				{
					vec.push_back(item.first);
				}
			}
		}
	}
	return vec;
}


//
// eg LL1A.InsertInTable(1, 0, "hi");
void LL1::InsertInTable(int col, int row, string line)
{
	//A1  col, row
	mTable[row][col] = line;
	
}