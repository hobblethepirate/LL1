#include "LL1.h"
#include <fstream>
#include <iostream>

LL1::LL1()
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



		//rhs may have spaces and pipes
		rhs = line.substr((pos + 1), (line.length() - pos));
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
					temp1.children.push_back(temp2);
					InsertTerm(temp2);
					leftIndex = rightIndex + 1;
					temp2.name = "";
				}
			}
		}
		//everything left over needs to be inserted
		temp2.name = rhs.substr(leftIndex, (rightIndex - leftIndex));
		temp1.children.push_back(temp2);
		InsertTerm(temp2);
		InsertTerm(temp1);

	}
}

void LL1::InsertTerm(Term term)
{
	if (mTermGroup.find(term.name) != mTermGroup.end())
	{
		//a redefinition should occur here
		mTermGroup.at(term.name).children = term.children;
		return;
	}

	mTermGroup[term.name] = term;
}

void LL1::PrintTerms() const
{
	for (auto term : mTermGroup)
	{
		cout << term.second.name;
		if (!term.second.children.empty())
		{
			cout << " has the children: ";
			for (int count = 0; count < term.second.children.size() - 1; count++)
			{
				cout << term.second.children[count].name <<", ";
			}
			cout << term.second.children[term.second.children.size() - 1].name << endl;
		}
		else
		{
			cout << endl;
			
		}
	}
}
//left factors the grammar and creates additional rules as needed.
void LL1::LeftFactor()
{

}

//Put it in a data structure (FirstSet map) runs the checkFor for each grammar rule
void LL1::FirstSet()
{

}

//inserts the $ for the parent and uses the information from the first set and running checkFor.
void LL1::FollowSet()
{

}

//Prints to an output file
void LL1::PrintTable()
{
}


//returns true if the given term is Terminal Value (empty children)
bool LL1::IsTerminal(Term t) const
{
	return t.children.empty();
}


//handles lambdas
//Uses terms and iterators findif
bool LL1::CheckFor()
{

	return false;
}