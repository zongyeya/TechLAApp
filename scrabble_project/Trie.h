#include <vector>
#include <iostream>

struct TrieNode
{

public:

	TrieNode(); // constructor for TrieNode
	bool inSet();
	int getOccurences();
	void increaseOccurences();
	void decreaseOccurences();
	TrieNode* parent; 
	TrieNode* children[26];


private:

	int occurences;


};

class TrieSet
{

public:
	TrieSet(std::string file_name);
	TrieSet();
	~TrieSet();
	void insert (std::string input);
	void remove (std::string input);
	void printSet(TrieNode* current, std::vector <char> word);
	TrieNode* traverse(TrieNode* start, char letter_of_child);
	TrieNode* prefix(std::string px);


//private:

	TrieNode* Root;
	void delete_set(TrieNode* current);
	TrieNode* Search(std::string input);





};