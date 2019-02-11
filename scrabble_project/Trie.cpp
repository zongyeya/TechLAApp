#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Exceptions.h"
#include "Trie.h"
#include "Util.h"
using namespace std;
	

TrieNode::TrieNode()
{
	parent = nullptr;

	for(int i = 0; i < 26; i++)
	{
		children[i] = nullptr;
	}

	occurences = 0;

}



int TrieNode::getOccurences()
{

	return occurences;

}



bool TrieNode::inSet()
{

	if(occurences > 0)
	{
		return true;
	}

	else 
	{
		return false;
	}

}


void TrieNode::increaseOccurences()
{

	occurences++;

}



void TrieNode::decreaseOccurences()
{

	occurences--;

}



TrieSet::TrieSet()	//Constructor initializing Root Node
{
	Root = new TrieNode();

}

TrieSet::TrieSet(std::string file_name)	//Constructor initializing Root Node
{

	Root = new TrieNode();


	std::ifstream dictFileStream(file_name);

	if(!dictFileStream)
	{
		throw FileException("DICTIONARY");
	}

	while(!dictFileStream.eof())
	{
		std::string word;
		dictFileStream >> word;

		if (dictFileStream.eof() && word.empty())
		{
			break;
		}

		makeLowercase(word);
		this->insert(word);

	}

}



TrieSet::~TrieSet()	//Deconstructor
{

	delete_set(Root);
	delete Root;
	Root = nullptr;

}


void TrieSet::delete_set(TrieNode* current)
{


	bool isLeaf = true;


	for(int i = 0 ; i < 26 ; i ++)

	{
		if (current -> children[i] != nullptr)
		{

			isLeaf = false;

		}

	} 

	if(isLeaf)
	{
		return;
	}


	for (int i = 0; i < 26; i++)
	{

		if(current -> children[i] != nullptr)
		{

			delete_set(current->children[i]);
			TrieNode* temp = current->children[i];
			delete temp;
			current->children[i] = nullptr;
		}
	}
}

void TrieSet::insert(string input)
{

	int index = 0;
	TrieNode* current = Root;

	while(input[index] != '\0')
	{

		if (current -> children[char(input[index])- 'a'] == nullptr)
		{

			current -> children[char(input[index])- 'a'] = new TrieNode();
			current -> children[char(input[index])- 'a'] -> parent = current;

		}

		current = current -> children[char(input[index])- 'a'];
		index++;


	}

	current -> increaseOccurences();
}


TrieNode* TrieSet::Search(string input)
{
	TrieNode* current = Root;
	int index = 0;
	while(input[index]!= '\0')
	{
		if(current -> children[char(input[index])- 'a'] != nullptr)
		{

			current = current -> children[char(input[index])- 'a'];

		}

		else 
		{
			return nullptr;
		}
		index++;
	}

	if(current -> getOccurences() > 0)
		{

			return current;
		}

		else 
		{
			return nullptr;
		}
}


TrieNode* TrieSet::prefix(string px)
{
	TrieNode* current = Root;
	int index = 0;
	while(px[index]!= '\0')
	{
		if(current -> children[char(px[index])- 'a'] != nullptr)
		{

			current = current -> children[char(px[index])- 'a'];

		}

		else 
		{

			return nullptr;

		}
		index ++;
	}
			return current;
}


TrieNode* TrieSet::traverse(TrieNode* start, char letter_of_child)
{

	TrieNode* current = start;

	letter_of_child = tolower(letter_of_child);

	current = start->children[letter_of_child - 'a'];


	return current;


}


void TrieSet::printSet(TrieNode* current, vector <char> word)
{


	if (current -> getOccurences() > 0)
	{

		for(auto it = word.begin(); it != word.end(); it++)
			{

				cout << *it;
			}
			cout << " " << current -> getOccurences() << endl;

	}


	for (int i = 0; i < 26; i++)
	{

		if(current -> children[i] != nullptr)
		{

			word.push_back('a' + i);
			printSet(current->children[i] , word);
			word.pop_back();
		}
	}
}

void TrieSet::remove(string input)
{

	TrieNode* currentNode = Search(input);

	if(currentNode != nullptr)
	{


		currentNode -> decreaseOccurences();
		TrieNode* parent_curr = nullptr;
		bool isLeaf = true;


		for(int i = 0; i < 26; i++)
		{
			if (currentNode -> children[i] != nullptr)
			{

				isLeaf = false;
				break;
			}

		}

		while (currentNode->parent != nullptr && isLeaf && currentNode -> getOccurences() == 0)
		{

			parent_curr = currentNode -> parent;

			for(int i = 0 ; i < 26; i++)
			{

				if(parent_curr -> children[i] == currentNode)
				{
					parent_curr -> children[i] = nullptr;
					delete currentNode;
					currentNode = parent_curr;
				}

				else if(parent_curr -> children[i] != nullptr)
				{
				
					isLeaf = false;
				
				}
			}

		}		

	} 
}


