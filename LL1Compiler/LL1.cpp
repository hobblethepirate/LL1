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
			for (int j = vec[i].length()-1; j > 0; --j)
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
		string oldTermName = termName; //S
		termName += '\''; // this will be the left factored term name if we create one - S => S'
		vector<string> termVec;

		if (!term.second.children.empty())
		{
			for (auto childGroup : term.second.childGroups)
			{
				if (childGroup.empty() != true)
				{
					string s = "";
					for (int count = 0; count < childGroup.size();count++)
					{
						s.append(childGroup[count]);
					}
					termVec.push_back(s);
				}
			}
		}
		//we now have a full vector in termVec to do the left factorizations
		//steps as follows:
		//1. use strncmp to figure out matches
		//2. create a new term with lambda and the matching 'magic string'
			//you do a strncmp comparison on length of a given string to all others, if it ever hits 0, it looks for all matching 0, and declares that section the 'magic string'
		//3. replace all instances of magic string with the termName

		string longest_string = FindLongestMatchingString(termVec);
		//string longest_string = "";

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

			//grab termName's first production and print it out, termName is S', longest_string is aa, oldTermName is S
			//now you loop through all terms. If the term's first name matches oldTermName, then you modify with replace function
			//cout << "termName:" << termName << endl;
			//cout << "oldTermName" << oldTermName << endl;

			for (auto blob : mTermGroup)
			{
				if (blob.second.name == oldTermName)
				{
					//DEBUG
					//cout << "A match! blob:" << blob.second.name << "with" << oldTermName << endl;
					//cout << "entering loop" << endl;

					for (int i = 0; i < blob.second.childGroups.size() - 1;++i) //loop for each child group
					{
						//DEBUG
						//cout << "blob i:" << i << " is " << blob.second.childGroups[1].at(i) << endl;
						//cout << "longest_word i:" << i << " is " << longest_string[i] << endl;

						bool isGood = false;
						for (int count = 0; count < longest_string.length(); count++)
						{
							if ( blob.second.childGroups[i].at(count)[0] == longest_string[count])
							{
								isGood = true;
								//cout << "WERE GOOD" << endl;
							}
							else
								isGood = false;
						}

						if (isGood)
						{
							for (int count = 0;count < longest_string.length(); count++)
							{
								//replace eveyr character with characters of S'
								if (count == 0)
								{
									blob.second.childGroups[i].at(count) = termName[0];

									//DEBUG
									//cout << "NEW NAME: " << blob.second.childGroups[i].at(count) << endl;
								}
								else if (count == 1)
								{
									blob.second.childGroups[i].at(count) = termName[1];
								}
								else //remaining characters for length of longest_string get erased in that term's childgroup
								{
									blob.second.childGroups[i].at(count).erase();

									//DEBUG
									//cout << "ERASED:" << blob.second.childGroups[i].at(count) << endl;
								}
							}
						}
					}
				}
			}
		}
	}
}



bool LL1::replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
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
	mLambdaSet = GenerateLambdaSet();

	// 2. if B begins with a terminal x, add terminal x to First(B)
		//map<string, Term> mFirstSet;
		//temp1.children.push_back(mTermGroup.find(temp2.name));
	for (auto term : mTermGroup)
	{
		//term.first is set name
		if (IsTerminal(term.second) == false)
		{
			for (auto child : term.second.childGroups)
			{

				//checks each character, childGroup[i], of each childGroup string 
				// 2.if child[0] is lowercase, add it to First(X), where X is term.first

				if (child.size() > 1)
				{
					//	2. If B’s production begins with a terminal(eg.B->aX), then add a to First(B).{
					if (IsUpper(child[0]) == false)
					{
						//mFirstSet[term.second.name].push_back(child[0]);
						InsertFirstSet(term.second.name, child[0]);
					}
					else
					{
						//first term is non-terminal
						//	4. If B begins with a non - terminal X(eg.B->XY)
						//	a.Compute First(X) and add First(X) – lambda to First(B)

						FirstSetRecurse(child[0]);
						for (auto firstSetItem : mFirstSet[child[0]])
						{
							if (firstSetItem.compare("?") != 0)
							{
								//mFirstSet[term.second.name].push_back(firstSetItem);
								InsertFirstSet(term.second.name, firstSetItem);
							}
						}
						//	b.If X is in the lambda set, then add First(remainder) – lambda to First(B)
						for (auto lambdaSetItem : mLambdaSet)
						{
							if (lambdaSetItem.compare(child[0]) == 0)
							{
								//X is in the lambda set 
								if (child.size() > 1)
								{
									//calculate the first set(remainder)
									for (int count = 1; count < child.size(); count++)
									{
										FirstSetRecurse(child[count]);
										if (IsUpper(child[count]) == true)
										{
											for (auto firstSetItem : mFirstSet[child[count]])
											{
												if (firstSetItem.compare("?") != 0)
												{
													//mFirstSet[term.second.name].push_back(firstSetItem);
													InsertFirstSet(term.second.name, firstSetItem);
												}
											}
										}
										else
										{
											//Check for X number of non terminals exist before this terminal and
											// if each non terminal goes to lambda eg PDQr	P->? D->? Q->? then r is in the first set
											bool allTerminal = true;
											for (int counter = 0; counter < count; counter++)
											{
												if (IsUpper(child[counter]) == false || IsInLambdaSet(child[counter]) == false)
												{
													allTerminal = false;
												}
											}
											if (allTerminal == true)
											{
												InsertFirstSet(term.second.name, child[count]);
											}
										}

									}
								}
							}
						}
					}
				}
			}
		}
		//	3. If B is in the lambda set, then add lambda to First(B)
		for (auto lambdaItem : mLambdaSet)
		{
			if (lambdaItem.compare(term.second.name) == 0)
			{
				//B is in the lambda set
				
				//mFirstSet[term.second.name].push_back("?");
				InsertFirstSet(term.second.name, "?");
			}
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

	/*vector<string> lambdaSet = GenerateLambdaSet();*/

	for (auto term : mTermGroup)
	{
		if (term.second.isStart == true)
		{
			//	1. Put $ in Follow(S) if S is the start symbol	
			//mFollowSet[term.second.name].push_back("$");
			InsertFollowSet(term.second.name, "$");
		}
		if (IsTerminal(term.second) == false)
		{
			//no follow set exists for terminal terms.
			for (auto childGroup : term.second.childGroups)
			{

				//Check through each child in childgroup
				int rightBound;
				for (rightBound = 0; rightBound < childGroup.size(); rightBound++)
				{
					if (IsUpper(childGroup[rightBound]) != true)
					{
						break;
					}
				}
				if (rightBound == 0)
				{
					//first child of the child group is terminal, check to see if the second child term of  is a non terminal
					
					//search to the right for a second terminal, 
					if (childGroup.size() > 1)
					{
						int secondTerminal;
						for (secondTerminal = 1; secondTerminal < childGroup.size(); secondTerminal++)
						{
							if (IsUpper(childGroup[secondTerminal]) == false)
							{
								break;
							}
						}
						if (secondTerminal>2)
						{
							//check to see if the non terminals
							
						}
						else if (secondTerminal==2)
						{

							if (childGroup.size() == 2)
							{
								if (IsUpper(childGroup[1]) == true && IsInLambdaSet(childGroup[1])==false)
								{
									//terminal followed by a non terminal
									for (auto firstSetItem : mFirstSet[childGroup[1]])
									{
										if (firstSetItem.compare("?")!=0)
										{
											InsertFollowSet(term.second.name, firstSetItem);
										}
									}

								}
					
							}
						}
						else if (secondTerminal==1)
						{
							//terminal followed by a terminal
							InsertFollowSet(term.second.name, childGroup[1]);
						}
					}
				}
				else if (rightBound != 0)
				{
					//2. If there is a production of the form : A->By where y is a terminal, add y to Follow(B)
					//rightbound is terminal, the Term to the left is non terminal
					if (IsUpper(childGroup[0]) == false)
					{
						InsertFollowSet(childGroup[rightBound - 1], childGroup[rightBound]);
					}
					
					for (int counterTwo = 1; counterTwo < rightBound; counterTwo++)
					{
						//3. If there is a production of the form : A->BC where C is a non - terminal, then add First(C) – lambda
						//to Follow(B)
						for (auto firstSetItem : mFirstSet[childGroup[counterTwo]])
						{
							if (firstSetItem.compare("?") != 0 && firstSetItem.compare("$") != 0)
							{
								InsertFollowSet(childGroup[counterTwo - 1], firstSetItem);
							}
						}
						// 4. If there is a production of the form A->BC, then add Follow(A) to Follow(C)
	
						for (auto followSetItem : mFollowSet[term.second.name])
						{
							if (followSetItem.compare("?") != 0 && followSetItem.compare("$"))
							{
								InsertFollowSet(childGroup[counterTwo], followSetItem);
							}
						}

						//	5. If there is a production of the form A->BC and C is in the lambda set, then add Follow(A) to
						//	Follow(B)

						if (IsInLambdaSet(childGroup[counterTwo]))
						{
							for (auto followSetItem : mFollowSet[term.second.name])
							{
								if (followSetItem.compare("$") != 0){
									InsertFollowSet(childGroup[counterTwo - 1], followSetItem);
								}
							}
						}

					}
					if (rightBound > 1)
					{
						//6. If there is a production of the form A->BCD and C is in the lambda set, then add First(D) – lambda to
						//the Follow(B).
						for (int counterTwo = 2; counterTwo < rightBound; counterTwo++)
						{
							if (IsInLambdaSet(childGroup[counterTwo - 1]))
							{
								for (auto firstSetItem : mFirstSet[childGroup[counterTwo]])
								{
									if (firstSetItem.compare("?") != 0)
									{
										InsertFollowSet(childGroup[counterTwo - 2], firstSetItem);
									}
								}
							}
						}
					}
					bool allTerminal = true;
					for (int counterTwo = 0; counterTwo < rightBound; counterTwo++)
					{

						if (IsInLambdaSet(childGroup[counterTwo]) == false)
						{
							allTerminal = false;
							break;
						}

					}
					if (allTerminal == true)
					{
						//each character up to the terminal is a Nonterminal and in the lambda set
						//the terminal needs to be added to the follow set for all Terminals before it
						for (int counterTwo = 0; counterTwo < rightBound; counterTwo++)
						{
							InsertFollowSet(childGroup[counterTwo], childGroup[rightBound]);
						}
					}

				}

			}
		}
	}
}





//A recursive method for creating firstSets
void LL1::FirstSetRecurse(string term)
{
	//Base case
	if (IsTerminal(mTermGroup[term]))
	{
		return;
	}


	//1. Generate a set of non - terminals that can generate a lambda, called it the lambda set.
	// FirstSet is called before this so the lambda set exists



	for (auto childGroup : mTermGroup[term].childGroups)
	{
		if (childGroup.size() >= 1)
		{
			if (IsUpper(childGroup[0]))
			{
					//non Terminal Child

				//	4. If B begins with a non - terminal X(eg.B->XY)
				//	a.Compute First(X) and add First(X) – lambda to First(B)
				FirstSetRecurse(childGroup[0]);
				for (auto firstSetItem : mFirstSet[childGroup[0]])
				{
					if (firstSetItem.compare("?") != 0)
					{
						//mFirstSet[term].push_back(firstSetItem);
						InsertFirstSet(term, firstSetItem);
					}
				}
				
				//	b.If X is in the lambda set, then add First(remainder) – lambda to First(B)
				if (childGroup.size() >= 2)
				{
					for (auto lambdaSetItem : mLambdaSet)
					{
						if (lambdaSetItem.compare(childGroup[0]) == 0)
						{
							for (int count = 1; count < childGroup.size(); count++)
							{
								FirstSetRecurse(childGroup[count]);

								for (auto firstSetItem : mFirstSet[childGroup[count]])
								{
									if (firstSetItem.compare("?") != 0)
									{
										//mFirstSet[term].push_back(firstSetItem);
										InsertFirstSet(term, firstSetItem);
									}
								}
							}

							break;
						}
					}
				}
			}
			else
			{
				//	2. If B’s production begins with a terminal(eg.B->aX), then add a to First(B).
				//terminal Child (wow that sound's bad)
				//mFirstSet[term].push_back(childGroup[0]);
				InsertFirstSet(term, childGroup[0]);
			}
		}
	}

	//	3. If B is in the lambda set, then add lambda to First(B)
	for (auto lambdaSetItem : mLambdaSet)
	{
		if (lambdaSetItem.compare(term) == 0)
		{
			//B is in the lambda set
			//mFirstSet[term].push_back("?");
			InsertFirstSet(term, "?");
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
	return true;
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

//Prints the contents of FirstSet
void LL1::PrintFirstSet() const
{	
	for (auto term : mFirstSet)
	{
		cout << term.first << "| ";
		for (auto firstSetItem : term.second)
		{
			cout << firstSetItem<<" ";
		}
		cout << endl;
	}
}

//Prints the contents of FollowSet
void LL1::PrintFollowSet() const
{
	for (auto term : mFollowSet)
	{
		cout << term.first << "| ";
		for (auto followSetItem : term.second)
		{
			cout << followSetItem <<" ";
		}
		cout << endl;
	}

}
//A helper method to avoid duplicate in the firstSet
void LL1::InsertFirstSet(string term, string firstSetItem)
{
	for (auto setItem : mFirstSet[term])
	{
		if (setItem.compare(firstSetItem) == 0)
		{
			//it already exists
			return;
		}
	}
	//insert it
	mFirstSet[term].push_back(firstSetItem);
}

//A helper method to avoid duplicate in the followSet
void LL1::InsertFollowSet(string term, string followSetItem)
{
	for (auto setItem : mFollowSet[term])
	{
		if (setItem.compare(followSetItem) == 0)
		{
			//it already exists
			return;
		}
	}
	//insert it
	mFollowSet[term].push_back(followSetItem);
}

bool LL1::IsInLambdaSet(string term)
{
	for (auto lambdaItem : mLambdaSet) 
	{
		if (lambdaItem.compare(term) == 0)
		{
			return true;
		}
	}
	return false;
}


//this drops items from first and follow sets into the table mTable and using InsertInTable method
void LL1::GenerateTable()
{
	//map<string, vector<string>> mFirstSet;
	//Contains the follow set for each production line after running the FirstSet, and then FollowSet Function.
	//map<string, vector<string>> mFollowSet;
	//This needs to be dynamic due to grammars changing in size.
	//map<int, map<int, string>> mTable;
	// eg LL1A.InsertInTable(1, 0, "hi"); 1=row, 0=col
	
	//First you need to create all the rows for each non terminal, then you'll create columns for all terminals
	//row 0 will be blank
	int count = 1;

	//print out rows
	for (auto term : mFirstSet)
	{
		InsertInTable(0, count, term.first);
		++count;
	}

	//print out columns based on list of terminals
	//look at all children for start set
	//insert dummy entry into upper left corner
	InsertInTable(0, 0, " "); //upper left corner entry

	int count2 = 1; // used in loop below
	map<string,int> StrToInt;

	for (auto term : mTermGroup)
	{
		if (term.second.children.empty()) //handles terminals only
		{
			//now, you create column after column for every terminal, at cross section with NonTerminal you use a function to grab the production

			//debug: this prints out every terminal
			//cout << "Here you would print out row: " << 0 << " and column " << count2 << endl;
			InsertInTable(count2, 0, term.first);
			
			StrToInt[term.first] = count2; //this will help with mapping when creating the entries, you can put in the nonterminal and get back column number

			count2++;			
		}
	}

	//at this point the rows and cols are printed out on the grid. Use the cross reference function to find the cross section of the non_terminal and terminal:
		// string LL1::CheckFirstFor(string checkthis, string forthis);
	
	int crossref_count = 0; //row count, increments at end of each term loop

	for (auto term : mTermGroup)
	{
		if (!term.second.children.empty()) //non-terminals only
		{

			//for every character in the first set, insert into table
			vector<string> vecStr = mFirstSet[term.first]; //vecStr is a vector containing all this term's terminals in first set

			for (int count = 0; count < vecStr.size(); count++)
			{
				int row = crossref_count;
				int col = StrToInt[vecStr[count]]; //terminal value's row
				string terminal = vecStr[count]; //terminal value;
				string non_terminal = term.first; //non-terminal value
				string production = CrossReference(non_terminal, terminal);

				InsertInTable(col, row, production);
			}
		}
		crossref_count++;
	}
}


//This function cross references the NonTerminal production for a Terminal value in the first set. if exists, it returns the production that has that terminal in first set, else, returns nullptr
//This function is designed to help with printing out the grid
string LL1::CrossReference(string non_terminal, string terminal)
{
	for (auto term : mTermGroup)
	{
		if (term.first == non_terminal) //find non_terminal in mTermGroup
		{
			for (int i = 0; i < term.second.childGroups.size();++i)
			{
				string production = "";
				//generate string production to return if successful e.g. "ABCd"
				for (int j = 0; j < term.second.childGroups[i].size();++j)
				{
					production += term.second.childGroups[i][j];
				}
				//e.g. production now == "ABCd"

				for (int k = 0; k < term.second.childGroups[i].size();++k)
				{
					//if childgroup has terminal char you're looking for, then quit and return production
					if (term.second.childGroups[i][k] == terminal)
					{
						return production;
					}
					//else if childGroup has lambda, then do nothing and continue through loop to keep checking for terminal
					else if (CheckFor(non_terminal, '?') == true) {}

					//if childgroup does not have lambda, break
					else { break; }

				}
			}

		}
	}

	return "";
}