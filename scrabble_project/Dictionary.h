/*
 * Dictionary.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The class Dictionary encodes the list of all legal words. */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <set>
#include <string>

class Dictionary
{
	//STUDENTS: Add private members & functions here
	std::set<std::string> words;

public:

	/* The constructor gets passed the name of the file from which
	   to read the word list.
	   If the file cannot be opened, it will throw a FileException
           with message "DICTIONARY". */
	Dictionary (std::string dictionary_file_name);

	~Dictionary ();

	/* Checks whether the given word is in the dictionary, and returns true if so.
	   Case should not matter. Must run in O(n) for the number of words in the dictionary. */
	bool isLegalWord (std::string const & word) const;

};


#endif /* DICTIONARY_H_ */
