//
// Main source file for Jamie's Scrabble solution
//

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream> // I wrote this

#include "Board.h"
#include "Bag.h"
#include "Dictionary.h"
#include "ConsolePrinter.h"
#include "Player.h"
#include "ConfigFile.h"
#include "Exceptions.h"
#include "rang.h"
#include "tries.h"
#include "Combo_Permu_Tree.h"	// I wrote this






void back_track_horizontal(Board* board, int start_col, int start_row, std::vector<int> combination, std::vector<int> index,
	int col, int row, std::string hand_string, TrieSet* prefix_tree, TrieNode* curr_prefix_node
	, std::string current_best_hand_string
	, std::string& returnMovestring , Player& cpuPlayer, Dictionary* dictionary, char first_blank 
									, char second_blank, int num_blank, char playerType, int& best_string_length, int& best_word_score) // if i change to non reference whole program fucks up
{


	std::string place_word;
	TrieNode* current_node = curr_prefix_node;


// std::cout << "this is col: " << std::endl;
// std::cout << col << std::endl;

//std::cout << "this is the current coordinates: " << row << " , " << col << std::endl;

	if((unsigned)col > board -> getColumns())
	{

		return;

	}

	if(board->getSquare(col , row) ->isOccupied())
	{
		current_node = prefix_tree -> traverse(curr_prefix_node, board->getSquare(col , row) -> getLetter());
		if(current_node != nullptr)
		{


			for(size_t i = 0; i < index.size(); i++)
			{

				place_word += hand_string[index[i]];

			}

		//std::cout << place_word << " is going to back track! " << std::endl;
			int col_curr = col;
			col_curr++;

			back_track_horizontal(board, start_col, start_row, combination,  index,
				col_curr, row, hand_string, prefix_tree, current_node, current_best_hand_string,
				returnMovestring, cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

			place_word.clear();


		}

		else if (current_node == nullptr)


		{
		// for(size_t i = 0; i < index.size(); i++)
		// {

		// 	place_word += hand_string[index[i]];

		// }

		//std::cout << place_word << " is gonna return! " << std::endl;
			return;
		}

	}


	else if (!board->getSquare(col , row) ->isOccupied())
	{


				// std::cout << "this is your combination: " << std::endl;

				// 	for(size_t i = 0; i < combination.size(); i++)
				// {


				// 	std::cout << combination[i] << " ";

				// }


		std::string check_move_string;


		




		for(size_t i = 0; i < hand_string.size(); i++)
		{
			if(combination[i] == 1)
			{
				combination[i] = 0;


			// std::cout << "this is your combination: " << std::endl;

			// for(size_t i = 0; i < combination.size(); i++)
			// {


			// 	std::cout << combination[i] << " ";

			// }
			// 	std::cout << std::endl;



			// std::cout << "currently this is your letter: " << hand_string[i] << std::endl;



				if (hand_string[i] == '?')
				{


					if(num_blank == 0)
					{

					//std::cout << "let me see if you are here!" << std::endl;

						num_blank ++;

						char alphabet_letter = 'a'; 
						for(int j = 0; j < 26; j ++)
						{
					//	std::cout << "this is the current alphabet_letter: " << alphabet_letter << std::endl;

							current_node = prefix_tree -> traverse(curr_prefix_node, alphabet_letter); 
							
							
							if(current_node != nullptr)
							{

								first_blank = alphabet_letter;
								alphabet_letter ++;
							//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
							// first_blank = '?';
								int col_curr = col + 1;	
								index.push_back(i);
								int blank_counter = 0;

								for(size_t i = 0; i < index.size(); i++)
								{
								if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
								{
									if(blank_counter == 0)
									{
										place_word += hand_string[index[i]];
										place_word += first_blank;
										blank_counter ++;
									}

									else if(blank_counter == 1)
									{

										place_word += hand_string[index[i]];
										place_word += second_blank;
										blank_counter ++;

									}

								}

								else
								{

									place_word += hand_string[index[i]];
								}

							}


							check_move_string += "place - ";
							check_move_string += " ";
							check_move_string += std::to_string(start_row);
							check_move_string += " ";
							check_move_string += std::to_string(start_col);
							check_move_string += " ";
							check_move_string += place_word;


							PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, true, place_word, &cpuPlayer);

							try
							{
								//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
								
								// first construct the move, which could throw an exception
								//playerMove = Move::parseMove(check_move_string, cpuPlayer);

								// now execute it, which could also throw exceptions
								// playerMove->execute(*board, *bag, *dictionary);

								std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


								for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
								{
									if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
									{
										throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
									}
								}

								cpuPlayer.addTiles(CPUplayerMove.tileVector());



								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int)best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}

								}







								}

						//std::cout << "CORRECT: " << check_move_string << std::endl;
							

							catch(MoveException & exception)
							{
								//std:: cout << "INCORRECT: " << check_move_string << std::endl;
								// print error message and reprompt the player
								//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
								//std::cout << exception.what() << std::endl;

								cpuPlayer.addTiles(CPUplayerMove.tileVector());

					// now, you can get on with your exception
								//throw moveException;

								//continue;
							}

							back_track_horizontal(board, start_col, start_row, combination, index,
								col_curr, row, hand_string, prefix_tree, current_node, current_best_hand_string,
								returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank,
								playerType, best_string_length, best_word_score);

							//first_blank = '?';
							check_move_string.clear();
							place_word.clear();
							index.pop_back();

						}
						
						

						else if (current_node == nullptr)
						{
							//combination[i] = 1;
							alphabet_letter ++;
							

						}
					}
				}
				

				

				else if(num_blank == 1)
				{

					//std::cout << "let me see if you are here!" << std::endl;

					num_blank ++;

					char alphabet_letter = 'a'; 
					for(int j = 0; j < 26; j ++)
					{
						//std::cout << "this is the current alphabet_letter: " << alphabet_letter << std::endl;

						current_node = prefix_tree -> traverse(curr_prefix_node, alphabet_letter); 


						if(current_node != nullptr)
						{


							second_blank = alphabet_letter;
							alphabet_letter ++;
							//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
							// first_blank = '?';

							int col_curr = col + 1;	
							index.push_back(i);
							int blank_counter = 0;

							for(size_t i = 0; i < index.size(); i++)
							{
								if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
								{
									if(blank_counter == 0)
									{
										place_word += hand_string[index[i]];
										place_word += first_blank;
										blank_counter ++;
									}

									else if(blank_counter == 1)
									{

										place_word += hand_string[index[i]];
										place_word += second_blank;
										blank_counter ++;

									}

								}

								else
								{

									place_word += hand_string[index[i]];
								}

							}


							check_move_string += "place - ";
							check_move_string += " ";
							check_move_string += std::to_string(start_row);
							check_move_string += " ";
							check_move_string += std::to_string(start_col);
							check_move_string += " ";
							check_move_string += place_word;
							PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, true, place_word, &cpuPlayer);

							try
							{
								//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
								
								// first construct the move, which could throw an exception
								//playerMove = Move::parseMove(check_move_string, cpuPlayer);

								// now execute it, which could also throw exceptions
								// playerMove->execute(*board, *bag, *dictionary);

								std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


								for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
								{
									if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
									{
										throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
									}
								}

								cpuPlayer.addTiles(CPUplayerMove.tileVector());



								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
							
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

							}


							catch(MoveException & exception)
							{
								//std:: cout << "INCORRECT: " << check_move_string << std::endl;
								// print error message and reprompt the player
								//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
								//std::cout << exception.what() << std::endl;
								//std::cout << "this is first BLANK: " << first_blank << std::endl;
								cpuPlayer.addTiles(CPUplayerMove.tileVector());

					// now, you can get on with your exception
								//throw moveException;

								//continue;
							}

							back_track_horizontal(board, start_col, start_row, combination, index,
								col_curr, row, hand_string, prefix_tree, current_node, current_best_hand_string,
								returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank,
								playerType, best_string_length, best_word_score);

							//second_blank = '?';
							check_move_string.clear();
							place_word.clear();
							index.pop_back();



						}
						
						

						else if (current_node == nullptr)
						{
							//combination[i] = 1;
							alphabet_letter ++;
							

						}

					}



				}


			}

			else if(hand_string[i]!= '?')
			{

					//std::cout << "are you here my friend? " << std::endl;


					current_node = prefix_tree -> traverse(curr_prefix_node, hand_string[i]); //might be bug here

					if(current_node != nullptr)
					{
						//std::cout << "are u ever here man!?!?!? woahlahwoahlah" << std::endl;
						//std::cout << "back track on " << "playerHand[" << i << "] " << std::endl;
						int col_curr = col;
						col_curr++;
						index.push_back(i);
						int blank_counter = 0;
						for(size_t i = 0; i < index.size(); i++)
						{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{

								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}

						check_move_string += "place - ";
						check_move_string += " ";
						check_move_string += std::to_string(start_row);
						check_move_string += " ";
						check_move_string += std::to_string(start_col);
						check_move_string += " ";
						check_move_string += place_word;
						PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, true, place_word, &cpuPlayer);
						try
						{	
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
						
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int)best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}	
								}

							
						}
						
						catch(MoveException & exception)
						{
							// std:: cout << "INCORRECT: " << check_move_string << std::endl;
							// print error message and reprompt the player
							//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

				// now, you can get on with your exception
							//throw moveException;

							//continue;
						}

						back_track_horizontal(board, start_col, start_row, combination, index,
							col_curr, row, hand_string, prefix_tree, current_node, 
							current_best_hand_string, returnMovestring, cpuPlayer, dictionary, first_blank, second_blank, num_blank
							, playerType, best_string_length, best_word_score);
						place_word.clear();
						check_move_string.clear();
						index.pop_back();
						combination[i] = 1;

					}

					else if (current_node == nullptr)
					{
						combination[i] = 1;
						//std::cout << "are you here my BAHGAH? " << std::endl;
						//std::cout << "this is the index of your hand: " << i << std::endl;
						continue;
					}
				} // here

		//std::cout << std::endl;
			}


		}
	}
}


void back_track_horizontal_start(Board* board, int col, int row, std::vector<int> combination, 
	std::string hand_string, TrieSet* prefix_tree, std::string current_best_hand_string, std::string& returnMovestring, Player& cpuPlayer, Dictionary* dictionary, 
	char first_blank, char second_blank, int num_blank, char playerType, int& best_string_length, int& best_word_score)
{

	//std::cout << "LOOK HERE " << cpuPlayer.getName() << std::endl; 


	//std::cout << "Hello World! welcome ur TREE!" << std::endl; 



	if(!(board->getSquare(col , row) ->isOccupied()))
	{
		TrieNode* current_node = prefix_tree -> Root;
		int col_curr = col;
		std::string place_word;
		std::string check_move_string;
		col_curr--;
		//std::cout << "this is your col_curr manh: " << col_curr << std::endl;

		if(col_curr >= 1) 
		{

			if(board->getSquare(col_curr , row) ->isOccupied())
			{

				while(col_curr >= 1 && board->getSquare(col_curr , row) ->isOccupied())
				{
					//std::cout << "you are here manh" << std::endl;
					col_curr--;

				}

				
				for(int i = col_curr+1 ; i < col; i++)
				{
					current_node = prefix_tree -> traverse(current_node , board->getSquare(i , row)-> getLetter());
					if(current_node == nullptr)
					{
						//std::cout << "YOUR PROGRAM IS FUCKED, JUST DROP SCHOOL!!!!" << std::endl;
						return;
					}
				}
			}
		}


		TrieNode* temp_node;
		for(size_t i = 0; i < hand_string.size(); i++)
		{
			if(combination[i] == 1)
			{
				combination[i] = 0;
				//current_node = prefix_tree -> traverse(current_node, playerHand[i]->getLetter());


				//std::cout << "this is combo mah mah on start layer " << std::endl;
				// for(size_t i = 0; i < playerHand.size(); i++)
				// {


				// 	std::cout << combination[i] << " "; 



				// }

				if (hand_string[i] == '?')
				{
					if(first_blank == '?')
					{

						num_blank ++;

						char alphabet_letter = 'a'; 
						for(int j = 0; j < 26; j ++)
						{



							temp_node = prefix_tree -> traverse(current_node ,alphabet_letter);
							


							
							if(temp_node != nullptr)
							{


								first_blank = alphabet_letter;
								alphabet_letter ++;
								//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
								// first_blank = '?';


								col_curr = col + 1;
								std::vector<int> index;
								index.push_back(i);

								

								int blank_counter = 0;

								for(size_t i = 0; i < index.size(); i++)
								{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{
								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}

						check_move_string += "place - ";
						check_move_string += " ";
						check_move_string += std::to_string(row);
						check_move_string += " ";
						check_move_string += std::to_string(col);
						check_move_string += " ";

						check_move_string += place_word;


						PlaceMove CPUplayerMove( (unsigned int) col, (unsigned int) row, true, place_word, &cpuPlayer);

						try
						{
							//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
							
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



							if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
						
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int)best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

						//std::cout << "CORRECT: " << check_move_string << std::endl;




						}
						catch(MoveException & exception)
						{
							//std:: cout << "INCORRECT: " << check_move_string << std::endl;
						// print error message and reprompt the player
						//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
						//std::cout << exception.what() << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

			// now, you can get on with your exception
						//throw moveException;

						//continue;
						}

						back_track_horizontal(board, col, row, combination, index,
							col_curr, row, hand_string, prefix_tree, temp_node, current_best_hand_string,
							returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

						// first_blank = '?';
						check_move_string.clear();
						place_word.clear();
						index.pop_back();
								// combination[i] = 1;


					}



					else if (temp_node == nullptr)
					{
								// combination[i] = 1;
						alphabet_letter ++;
						continue;

					}

				}



					//std::cout << "hello hello hello? " << std::endl;

			}
			combination[i] = 1;
		}


		else 
		{

			temp_node = prefix_tree -> traverse(current_node , hand_string[i]);

			if(temp_node != nullptr)
			{

						//std::cout << "back track on " << "playerHand[" << i << "] " << std::endl;


				col_curr = col + 1;
				std::vector<int> index;
				index.push_back(i);


				int blank_counter = 0;

				for(size_t i = 0; i < index.size(); i++)
				{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{
								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}





						//std::cout << "this is yo god damn word: " << place_word << std::endl;

						check_move_string += "place - ";
						check_move_string += " ";
						check_move_string += std::to_string(row);
						check_move_string += " ";
						check_move_string += std::to_string(col);
						check_move_string += " ";





						check_move_string += place_word;


						PlaceMove CPUplayerMove( (unsigned int) col, (unsigned int) row, true, place_word, &cpuPlayer);

						try
						{
							//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
							
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
							
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

						//std::cout << "CORRECT: " << check_move_string << std::endl;




						}
						catch(MoveException & exception)
						{
							//std:: cout << "INCORRECT: " << check_move_string << std::endl;
						// print error message and reprompt the player
						//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
						//std::cout << exception.what() << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

			// now, you can get on with your exception
						//throw moveException;

						//continue;
						}

						//std::cout << "this is index right before recursion: ";
						// for(size_t i = 0; i < index.size(); i++)
						// {
						// 	std::cout << index[i] << " ";

						// // }
						// std::cout << std::endl;

						back_track_horizontal(board, col, row, combination, index,
							col_curr, row, hand_string, prefix_tree, temp_node, current_best_hand_string,
							returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);


						

						check_move_string.clear();
						place_word.clear();
						index.pop_back();
						combination[i] = 1;
					}

					else if (temp_node == nullptr)
					{
						combination[i] = 1;

						
						continue;

					}
				}
			}

			//std::cout << std::endl;
		}

	}

	else if (board->getSquare(col , row) ->isOccupied())
	{
		
		return;
	}
	


	return;
}




void back_track_vertical(Board* board, int start_col, int start_row, std::vector<int> combination, std::vector<int> index,
	int col, int row, std::string hand_string, TrieSet* prefix_tree, TrieNode* curr_prefix_node
	, std::string current_best_hand_string
	, std::string& returnMovestring , Player& cpuPlayer, Dictionary* dictionary, char first_blank 
									, char second_blank, int num_blank, char playerType, int& best_string_length, int& best_word_score) // if i change to non reference whole program fucks up
{


	std::string place_word;
	TrieNode* current_node = curr_prefix_node;


// std::cout << "this is col: " << std::endl;
// std::cout << col << std::endl;

//std::cout << "this is the current coordinates: " << row << " , " << col << std::endl;

	if((unsigned)row > board -> getRows())
	{

		return;

	}

	if(board->getSquare(col , row) ->isOccupied())
	{
		current_node = prefix_tree -> traverse(curr_prefix_node, board->getSquare(col , row) -> getLetter());
		if(current_node != nullptr)
		{


			for(size_t i = 0; i < index.size(); i++)
			{

				place_word += hand_string[index[i]];

			}

		//std::cout << place_word << " is going to back track! " << std::endl;
			int row_curr = row;
			row_curr++;

			back_track_vertical(board, start_col, start_row, combination,  index,
				col, row_curr, hand_string, prefix_tree, current_node, current_best_hand_string,
				returnMovestring, cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

			place_word.clear();


		}

		else if (current_node == nullptr)


		{
		// for(size_t i = 0; i < index.size(); i++)
		// {

		// 	place_word += hand_string[index[i]];

		// }

		//std::cout << place_word << " is gonna return! " << std::endl;
			return;
		}

	}


	else if (!board->getSquare(col , row) ->isOccupied())
	{


				// std::cout << "this is your combination: " << std::endl;

				// 	for(size_t i = 0; i < combination.size(); i++)
				// {


				// 	std::cout << combination[i] << " ";

				// }


		std::string check_move_string;


		




		for(size_t i = 0; i < hand_string.size(); i++)
		{
			if(combination[i] == 1)
			{
				combination[i] = 0;


			// std::cout << "this is your combination: " << std::endl;

			// for(size_t i = 0; i < combination.size(); i++)
			// {


			// 	std::cout << combination[i] << " ";

			// }
			// 	std::cout << std::endl;



			// std::cout << "currently this is your letter: " << hand_string[i] << std::endl;





				if (hand_string[i] == '?')
				{


					if(num_blank == 0)
					{

					//std::cout << "let me see if you are here!" << std::endl;

						num_blank ++;

						char alphabet_letter = 'a'; 
						for(int j = 0; j < 26; j ++)
						{
					//	std::cout << "this is the current alphabet_letter: " << alphabet_letter << std::endl;

							current_node = prefix_tree -> traverse(curr_prefix_node, alphabet_letter); 
							
							
							if(current_node != nullptr)
							{

								first_blank = alphabet_letter;
								alphabet_letter ++;
							//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
							// first_blank = '?';
								int row_curr = row + 1;	
								index.push_back(i);
								int blank_counter = 0;

								for(size_t i = 0; i < index.size(); i++)
								{
								if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
								{
									if(blank_counter == 0)
									{
										place_word += hand_string[index[i]];
										place_word += first_blank;
										blank_counter ++;
									}

									else if(blank_counter == 1)
									{

										place_word += hand_string[index[i]];
										place_word += second_blank;
										blank_counter ++;

									}

								}

								else
								{

									place_word += hand_string[index[i]];
								}

							}


							check_move_string += "place - ";
							check_move_string += " ";
							check_move_string += std::to_string(start_row);
							check_move_string += " ";
							check_move_string += std::to_string(start_col);
							check_move_string += " ";
							check_move_string += place_word;


							PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, true, place_word, &cpuPlayer);

							try
							{
								//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
								
								// first construct the move, which could throw an exception
								//playerMove = Move::parseMove(check_move_string, cpuPlayer);

								// now execute it, which could also throw exceptions
								// playerMove->execute(*board, *bag, *dictionary);

								std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


								for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
								{
									if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
									{
										throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
									}
								}

								cpuPlayer.addTiles(CPUplayerMove.tileVector());

								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
									
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned)best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

						//std::cout << "CORRECT: " << check_move_string << std::endl;
							}

							catch(MoveException & exception)
							{
								//std:: cout << "INCORRECT: " << check_move_string << std::endl;
								// print error message and reprompt the player
								//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
								//std::cout << exception.what() << std::endl;

								cpuPlayer.addTiles(CPUplayerMove.tileVector());

					// now, you can get on with your exception
								//throw moveException;

								//continue;
							}

							back_track_vertical(board, start_col, start_row, combination, index,
								col, row_curr, hand_string, prefix_tree, current_node, current_best_hand_string,
								returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

							//first_blank = '?';
							check_move_string.clear();
							place_word.clear();
							index.pop_back();

						}
						
						

						else if (current_node == nullptr)
						{
							//combination[i] = 1;
							alphabet_letter ++;
							

						}
					}
				}
				

				

				else if(num_blank == 1)
				{

					//std::cout << "let me see if you are here!" << std::endl;

					num_blank ++;

					char alphabet_letter = 'a'; 
					for(int j = 0; j < 26; j ++)
					{
						//std::cout << "this is the current alphabet_letter: " << alphabet_letter << std::endl;

						current_node = prefix_tree -> traverse(curr_prefix_node, alphabet_letter); 


						if(current_node != nullptr)
						{


							second_blank = alphabet_letter;
							alphabet_letter ++;
							//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
							// first_blank = '?';

							int row_curr = row + 1;	
							index.push_back(i);
							int blank_counter = 0;

							for(size_t i = 0; i < index.size(); i++)
							{
								if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
								{
									if(blank_counter == 0)
									{

											place_word += hand_string[index[i]];
											place_word += first_blank;
											blank_counter ++;
									}

									else if(blank_counter == 1)
									{

										place_word += hand_string[index[i]];
										place_word += second_blank;
										blank_counter ++;

									}

								}

								else
								{

									place_word += hand_string[index[i]];
								}

							}


							check_move_string += "place | ";
							check_move_string += " ";
							check_move_string += std::to_string(start_row);
							check_move_string += " ";
							check_move_string += std::to_string(start_col);
							check_move_string += " ";
							check_move_string += place_word;
							PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, false, place_word, &cpuPlayer);

							try
							{
								//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
								
								// first construct the move, which could throw an exception
								//playerMove = Move::parseMove(check_move_string, cpuPlayer);

								// now execute it, which could also throw exceptions
								// playerMove->execute(*board, *bag, *dictionary);

								std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


								for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
								{
									if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
									{
										throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
									}
								}

								cpuPlayer.addTiles(CPUplayerMove.tileVector());



								if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{
							
									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

							}


							catch(MoveException & exception)
							{
								//std:: cout << "INCORRECT: " << check_move_string << std::endl;
								// print error message and reprompt the player
								//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
								//std::cout << exception.what() << std::endl;

								cpuPlayer.addTiles(CPUplayerMove.tileVector());

					// now, you can get on with your exception
								//throw moveException;

								//continue;
							}

							back_track_vertical(board, start_col, start_row, combination, index,
								col, row_curr, hand_string, prefix_tree, current_node, current_best_hand_string,
								returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

							//second_blank = '?';
							check_move_string.clear();
							place_word.clear();
							index.pop_back();



						}
						
						

						else if (current_node == nullptr)
						{
							//combination[i] = 1;
							alphabet_letter ++;
							

						}

					}



				}


			}

			else if(hand_string[i]!= '?')
			{

					//std::cout << "are you here my friend? " << std::endl;


					current_node = prefix_tree -> traverse(curr_prefix_node, hand_string[i]); //might be bug here

					if(current_node != nullptr)
					{
						//std::cout << "are u ever here man!?!?!? woahlahwoahlah" << std::endl;
						//std::cout << "back track on " << "playerHand[" << i << "] " << std::endl;
						int row_curr = row;
						row_curr++;
						index.push_back(i);
						int blank_counter = 0;
						for(size_t i = 0; i < index.size(); i++)
						{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{

								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}

						check_move_string += "place | ";
						check_move_string += " ";
						check_move_string += std::to_string(start_row);
						check_move_string += " ";
						check_move_string += std::to_string(start_col);
						check_move_string += " ";
						check_move_string += place_word;
						PlaceMove CPUplayerMove( (unsigned int) start_col, (unsigned int) start_row, false, place_word, &cpuPlayer);
						try
						{	
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



							if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}
						}
						
						catch(MoveException & exception)
						{
							//std:: cout << "INCORRECT: " << check_move_string << std::endl;
							// print error message and reprompt the player
							//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

				// now, you can get on with your exception
							//throw moveException;

							//continue;
						}

						back_track_vertical(board, start_col, start_row, combination, index,
							col, row_curr, hand_string, prefix_tree, current_node, 
							current_best_hand_string, returnMovestring, cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);
						place_word.clear();
						check_move_string.clear();
						index.pop_back();
						combination[i] = 1;

					}

					else if (current_node == nullptr)
					{
						combination[i] = 1;
						//std::cout << "are you here my BAHGAH? " << std::endl;
						//std::cout << "this is the index of your hand: " << i << std::endl;
						continue;
					}
				} // here

		//std::cout << std::endl;
			}


		}
	}
}


void back_track_vertical_start(Board* board, int col, int row, std::vector<int> combination, 
	std::string hand_string, TrieSet* prefix_tree, std::string current_best_hand_string, std::string& returnMovestring, Player& cpuPlayer, Dictionary* dictionary, 
	char first_blank, char second_blank, int num_blank, char playerType, int& best_string_length, int& best_word_score)
{

	//std::cout << "LOOK HERE " << cpuPlayer.getName() << std::endl; 


	//std::cout << "Hello World! welcome ur TREE!" << std::endl; 



	if(!(board->getSquare(col , row) ->isOccupied()))
	{
		TrieNode* current_node = prefix_tree -> Root;
		int row_curr = row;
		std::string place_word;
		std::string check_move_string;
		row_curr--;
		//std::cout << "this is your col_curr manh: " << col_curr << std::endl;

		if(row_curr >= 1) 
		{

			if(board->getSquare(col , row_curr) ->isOccupied())
			{

				while(row_curr >= 1 && board->getSquare(col , row_curr) ->isOccupied())
				{
					//std::cout << "you are here manh" << std::endl;
					row_curr--;

				}

				
				for(int i = row_curr+1 ; i < row; i++)
				{
					current_node = prefix_tree -> traverse(current_node , board->getSquare(col , i)-> getLetter());
					if(current_node == nullptr)
					{
						//std::cout << "YOUR PROGRAM IS FUCKED, JUST DROP SCHOOL!!!!" << std::endl;
						return;
					}
				}
			}
		}


		TrieNode* temp_node;
		for(size_t i = 0; i < hand_string.size(); i++)
		{
			if(combination[i] == 1)
			{
				combination[i] = 0;
				//current_node = prefix_tree -> traverse(current_node, playerHand[i]->getLetter());


				//std::cout << "this is combo mah mah on start layer " << std::endl;
				// for(size_t i = 0; i < playerHand.size(); i++)
				// {


				// 	std::cout << combination[i] << " "; 



				// }

				if (hand_string[i] == '?')
				{
					if(first_blank == '?')
					{

						num_blank ++;

						char alphabet_letter = 'a'; 
						for(int j = 0; j < 26; j ++)
						{



							temp_node = prefix_tree -> traverse(current_node ,alphabet_letter);
							


							
							if(temp_node != nullptr)
							{


								first_blank = alphabet_letter;
								alphabet_letter ++;
								//std::cout << "this is going to be first_blank: " << first_blank << std::endl;
								// first_blank = '?';


								row_curr = row + 1;
								std::vector<int> index;
								index.push_back(i);

								

								int blank_counter = 0;

								for(size_t i = 0; i < index.size(); i++)
								{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{
								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}




						check_move_string += "place | ";
						check_move_string += " ";
						check_move_string += std::to_string(row);
						check_move_string += " ";
						check_move_string += std::to_string(col);
						check_move_string += " ";

						check_move_string += place_word;


						PlaceMove CPUplayerMove( (unsigned int) col, (unsigned int) row, false, place_word, &cpuPlayer);

						try
						{
							//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
							
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



						if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}
						//std::cout << "CORRECT: " << check_move_string << std::endl;




						}
						catch(MoveException & exception)
						{
							//std:: cout << "INCORRECT: " << check_move_string << std::endl;
						// print error message and reprompt the player
						//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
						//std::cout << exception.what() << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

			// now, you can get on with your exception
						//throw moveException;

						//continue;
						}

						back_track_vertical(board, col, row, combination, index,
							col, row_curr, hand_string, prefix_tree, temp_node, current_best_hand_string,
							returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);

						//first_blank = '?';
						check_move_string.clear();
						place_word.clear();
						index.pop_back();
								// combination[i] = 1;


					}



					else if (temp_node == nullptr)
					{
								// combination[i] = 1;
						alphabet_letter ++;
						continue;

					}

				}



					//std::cout << "hello hello hello? " << std::endl;

			}
			combination[i] = 1;
		}


		else 
		{

			temp_node = prefix_tree -> traverse(current_node , hand_string[i]);

			if(temp_node != nullptr)
			{

						//std::cout << "back track on " << "playerHand[" << i << "] " << std::endl;


				row_curr = row + 1;
				std::vector<int> index;
				index.push_back(i);


				int blank_counter = 0;

				for(size_t i = 0; i < index.size(); i++)
				{
							if(hand_string[index[i]] == '?' ) // this is only for one ?, think about two ?'s'
							{
								if(blank_counter == 0)
								{
									place_word += hand_string[index[i]];
									place_word += first_blank;
									blank_counter ++;
								}

								else if(blank_counter == 1)
								{

									place_word += hand_string[index[i]];
									place_word += second_blank;
									blank_counter ++;

								}

							}

							else
							{

								place_word += hand_string[index[i]];
							}

						}





						//std::cout << "this is yo god damn word: " << place_word << std::endl;

						check_move_string += "place | ";
						check_move_string += " ";
						check_move_string += std::to_string(row);
						check_move_string += " ";
						check_move_string += std::to_string(col);
						check_move_string += " ";





						check_move_string += place_word;


						PlaceMove CPUplayerMove( (unsigned int) col, (unsigned int) row, false, place_word, &cpuPlayer);

						try
						{
							//PlaceMove CPUplayerMove( (unsigned int) row, (unsigned int) col, true, place_word, &cpuPlayer);
							
							// first construct the move, which could throw an exception
							//playerMove = Move::parseMove(check_move_string, cpuPlayer);

							// now execute it, which could also throw exceptions
							// playerMove->execute(*board, *bag, *dictionary);

							std::vector<std::pair<std::string, unsigned int>> sequencesFormed = board -> getPlaceMoveResults(CPUplayerMove);


							for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
							{
								if(!dictionary->isLegalWord(sequencesFormed[sequenceIndex].first))
								{
									throw MoveException("INVALIDWORD:" + sequencesFormed[sequenceIndex].first);
								}
							}

							cpuPlayer.addTiles(CPUplayerMove.tileVector());



					if(playerType == 'l')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										best_string_length = returnMovestring.size() - num_blank;
									}


									else if (returnMovestring != "pass")
									{
										current_best_hand_string = check_move_string;

										int current_string_length = current_best_hand_string.size();
										current_string_length = current_string_length - num_blank;

										if(current_string_length > best_string_length)
										{
											returnMovestring = current_best_hand_string;
											best_string_length = current_string_length;
										}


									}
								}

								else if (playerType == 's')
								{

									if(returnMovestring == "pass")
									{
										 
										returnMovestring = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
			
											best_word_score += sequencesFormed[sequenceIndex].second;
										}
	

										if( return_string_length == (int)hand_string.size())
										{
										
											best_word_score += 50;
										}
										
									}


									else if (returnMovestring != "pass")
									{
										
										current_best_hand_string = check_move_string;
										int return_string_length = place_word.size() - num_blank;


										unsigned int totalScore = 0;
										
										for(size_t sequenceIndex = 0; sequenceIndex < sequencesFormed.size(); ++sequenceIndex)
										{
				
												totalScore += sequencesFormed[sequenceIndex].second;
										}
		

										if( return_string_length == (int)hand_string.size())
										{
											
											totalScore += 50;
										}


										if(totalScore > (unsigned int) best_word_score)
										{
											returnMovestring = current_best_hand_string;
											best_word_score = totalScore;

										}
							

									}
								}

						//std::cout << "CORRECT: " << check_move_string << std::endl;




						}
						catch(MoveException & exception)
						{
							//std:: cout << "INCORRECT: " << check_move_string << std::endl;
						// print error message and reprompt the player
						//std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
						//std::cout << exception.what() << std::endl;

							cpuPlayer.addTiles(CPUplayerMove.tileVector());

			// now, you can get on with your exception
						//throw moveException;

						//continue;
						}

						//std::cout << "this is index right before recursion: ";
						// for(size_t i = 0; i < index.size(); i++)
						// {
						// 	std::cout << index[i] << " ";

						// // }
						// std::cout << std::endl;

						back_track_vertical(board, col, row, combination, index,
							col, row_curr, hand_string, prefix_tree, temp_node, current_best_hand_string,
							returnMovestring , cpuPlayer, dictionary, first_blank, second_blank, num_blank, playerType, best_string_length, best_word_score);


						

						check_move_string.clear();
						place_word.clear();
						index.pop_back();
						combination[i] = 1;
					}

					else if (temp_node == nullptr)
					{
						combination[i] = 1;

						
						continue;

					}
				}
			}

			//std::cout << std::endl;
		}

	}

	else if (board->getSquare(col , row) ->isOccupied())
	{
		
		return;
	}
	


	return;
}

// Get a friendly string to print to the user in response to a move exception
std::string getFriendlyError(MoveException const & exception)
{
	std::string exMessage(exception.what());

	// first half of invalid word exception message
	std::string invalidWordExPrefix("INVALIDWORD:");

	if(exMessage == "EMPTY")
	{
		return "Move would not manipulate any tiles!";
	}
	else if(exMessage == "MALFORMED")
	{
		return "Format error in command arguments!";
	}
	else if(exMessage == "UNKNOWN")
	{
		return "Unknown command word";
	}
	else if(exMessage == "WRONGTILES")
	{
		return "You don't have all the tiles you'd need for this move!";
	}
	else if(exMessage == "OUTOFBOUNDS")
	{
		return "Would go out of bounds!";
	}
	else if(exMessage == "OCCUPIED")
	{
		return "Coordinates of first tile are occupied!";
	}
	else if(exMessage == "NOSTART")
	{
		return "This is the first move, you have to use the start square!";
	}
	else if(exMessage == "NOWORDS")
	{
		return "Would not create a sequence of at least 2 letters!";
	}
	else if(exMessage == "NONEIGHBOR")
	{
		return "No tile is adjacent to a previously placed tile!";
	}
	else if(exMessage.substr(0, invalidWordExPrefix.size()) == invalidWordExPrefix)
	{
		// get invalid word out of exception message
		std::string invalidWord = exMessage.substr(invalidWordExPrefix.size(), std::string::npos);

		return "Would form the letter sequence \'" + invalidWord + "\', which is not a word!";
	}
	else
	{
		return "Unknown MoveException: " + exMessage;
	}

}

// Perform the final subtraction, whereby all players lose score equivalent to the
// values of the tiles they hold.  If emptyHandPlayerIndex is not -1, this denotes the player
// who ran out of tiles, and they will receive the total of all deducted points.

unsigned int doFinalSubtraction(std::vector<Player *> & players, ssize_t emptyHandPlayerIndex)
{
	unsigned int totalScore = 0;

	for(std::vector<Player *>::iterator playersIter = players.begin(); playersIter != players.end(); ++playersIter)
	{
		unsigned int remainingPoints = (*playersIter)->remainingPoints();
		totalScore += remainingPoints;
		(*playersIter)->subtractPoints(remainingPoints);
	}

	if(emptyHandPlayerIndex != -1)
	{
		players[emptyHandPlayerIndex]->addPoints(totalScore);
	}

	return totalScore;
}

#define MOVE_PROMPT_COLOR rang::fgB::blue << rang::style::bold
#define PLAYER_NAME_COLOR rang::fgB::red << rang::style::bold
#define TILE_NAME_COLOR rang::fg::green << rang::style::bold
#define SCORE_COLOR rang::fgB::magenta << rang::style::bold

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <configuration file>" << std::endl;
		return 3;
	}

	// data for running game
	// NOTE: must be pointers so that they can be initialized inside the try-catch block.
	ConfigFile * configFile;
	Board * board;
	Bag * bag;
	Dictionary * dictionary;
	std::vector<Player *> players;


	// attempt to read files and initialize game.
	// -------------------------------------------------------------------------------

	// NOTE: it's not required to be careful about missing files like this,
	// but I like being paranoid.
	try
	{
		configFile = new ConfigFile(argv[1]);
		bag = new Bag(configFile->tilesetFile, configFile->randomSeed);
		dictionary = new Dictionary(configFile->dictionaryFile);
		

		

		board = new Board(configFile->boardFile);



	}
	catch(FileException & fileException)
	{
		if(fileException.what() == std::string("MISSINGCONFIG"))
		{
			std::cerr << "Error: config file " << argv[1] << " was not found!" << std::endl;
			return 3;
		}
		else if(fileException.what() == std::string("BAG"))
		{
			std::cerr << "Error: tileset file " << configFile->tilesetFile << " could not be read!" << std::endl;
			return 4;
		}
		else if(fileException.what() == std::string("BOARD"))
		{
			std::cerr << "Error: board file " << configFile->boardFile << " could not be read!" << std::endl;
			return 5;
		}
		else if(fileException.what() == std::string("BOARDCONTENT"))
		{
			std::cerr << "Error: board file " << configFile->boardFile << " contained invalid content!" << std::endl;
			return 5;
		}
		else if(fileException.what() == std::string("DICTIONARY"))
		{
			std::cerr << "Error: dictionary file " << configFile->dictionaryFile << " could not be read!" << std::endl;
			return 6;
		}
		else
		{
			std::cerr << "Unexpected FileException: " << fileException.what() << std::endl;
			return 7;
		}
	}

	// question user about players to add
	// -------------------------------------------------------------------------------

	TrieSet* prefix_tree = new TrieSet(configFile->dictionaryFile); //I wrote
	
	std::ifstream ifile(argv[1]);
	

	std::string myline;

	while(getline(ifile,myline))
	{
		std::stringstream ss(myline);
		std::string parse_word;
		ss >> parse_word;

		if(parse_word == "INIT:")
		{
			board -> first_move_made();
			std::string init_file_name;
			ss >> init_file_name;


			std::ifstream initfile(init_file_name);


			for(size_t i = 1; i < board->getRows(); i++)
			{
				std::string init_line;

				getline(initfile , init_line);

				for(size_t j = 0; j < init_line.size(); j += 3)
				{

					char first = init_line[j];
					char second = init_line[j+1];
					char third = init_line[j+2];

					
					if(first == '.')
					{
						
					}

					if(first != '.')
					{
						int score = ((int)second - 48) * 10 + ((int)third - 48);
						Tile* tile_curr = new Tile (first , score);

						Square* square_curr = board->getSquare(j / 3 , i);
						square_curr -> placeTile(tile_curr);

					}



				}



			}

				initfile.close();

		}

	}
		ifile.close();










	size_t numPlayers;
	std::cout << "Please enter number of players: ";
	std::cin >> numPlayers;

	// this check is not strictly required but it is in the spec...
	if(numPlayers < 1 || numPlayers > 8)
	{
		std::cerr << "Invalid number of players, must be between 1 and 8." << std::endl;
		return 9;
	}
	std::cout << numPlayers << " players confirmed." << std::endl;

	// read in player names
	// -------------------------------------------------------------------------------

	// clear remaining data on cin from the previous prompt
	std::cin.ignore();


//////////////////////////////////////////////////I WROTE THIS//////////////////////////////////////////////////////////////////////
	for(size_t playerNum = 1; playerNum <= numPlayers ; ++playerNum)
	{
		std::cout << "Please enter name for player " << playerNum << ": ";
		std::string playerName;
		std::getline(std::cin, playerName);
		std::cout << "Player " << playerNum << ", named \"" << playerName << "\", has been added." << std::endl;

		// create player with a full initial hand
		players.push_back(new Player(playerName, configFile->handSize));


		// if (playerNum == 1)
		// {				//I wrote this for test, remove later
		// std::vector<Tile*> hand_to_add;
		// hand_to_add.push_back(new Tile('?' , 0));
		// hand_to_add.push_back(new Tile('?' , 0));
		// hand_to_add.push_back(new Tile('t' , 1));
		// // hand_to_add.push_back(new Tile('t' , 1));
		

		// players[playerNum - 1]->addTiles(hand_to_add);
		// }


		// if (playerNum == 2)
		// {				//I wrote this for test, remove later
		// std::vector<Tile*> hand_to_add;
		// hand_to_add.push_back(new Tile('a' , 1));
		// hand_to_add.push_back(new Tile('a' , 1));
		// hand_to_add.push_back(new Tile('c' , 1));
		// hand_to_add.push_back(new Tile('d' , 1));

		// players[playerNum - 1]->addTiles(hand_to_add);
		// }




		players[playerNum - 1]->addTiles(bag->drawTiles(configFile->handSize)); //(CHANGE THIS BACK AFTER DEBUG)
	}

	

	// std::cout << "Please enter name for player " << playerNum << ": ";
	// 	std::string playerName;
	// 	std::getline(std::cin, playerName);
	// 	std::cout << "Player " << playerNum << ", named \"" << playerName << "\", has been added." << std::endl;

	// 	// create player with a full initial hand
	// 	players.push_back(new Player(playerName, configFile->handSize));
	// 	players[playerNum - 1]->addTiles(bag->drawTiles(configFile->handSize));
	// 	vector


///////////////////////////////////////////////END I WROTE THIS//////////////////////////////////////////////////////////////////////


	// run game loop
	// -------------------------------------------------------------------------------

	bool gameWon = false;
	ssize_t emptyHandPlayerIndex;

	size_t sequentialPasses = 0;

	while(!gameWon)
	{
		for(size_t playerNum = 0; (!gameWon && playerNum < numPlayers); ++playerNum)
		{
			ConsolePrinter::printBoard(*board);
			ConsolePrinter::printHand(*players[playerNum]);

			Move * playerMove = nullptr;
			bool correctMove = false;

			while(!correctMove)
			{

				std::stringstream ss(players[playerNum]->getName());
				
	//////////////////////////////I wrote this, to detect if it is a CPU player (ask if there will be space between cpu)////////////////
				std::string first_three_letters; 
				char one_letter;
				for(int i = 0; i < 3; i++)
				{
					ss >> one_letter;
					if (one_letter == 'C')
					{
						one_letter = 'c';
					}

					else if (one_letter == 'P')
					{
						one_letter = 'p';
					}

					else if (one_letter == 'U')
					{
						one_letter = 'u';
					}

					first_three_letters += one_letter;

				}								 //I wrote this end

			if(first_three_letters == "cpu") //I wrote this
			{
				char fourth_letter;
				ss >> fourth_letter;

				if(fourth_letter == 'L')
				{
					fourth_letter = 'l';
				}

				else if (fourth_letter == 'S')
				{
					fourth_letter = 's';
				}



				if(fourth_letter == 'l')
				{


					std::set<Tile*> peek_peek = players[playerNum]->getHandTiles();
					std::set<Tile*>::iterator it;
					std::vector<Tile*>  player_hand;


					for (it = peek_peek.begin(); it != peek_peek.end(); it++)
					{
						player_hand.push_back(*it);
					}


					std::string return_my_string_horizontal = "pass";


					std::string return_my_string_vertical = "pass";

					int best_string_len_horizontal = 0;
					int best_string_len_vertical = 0;
					int best_string_score_horizontal = 0;
					int best_string_score_vertical = 0;

					std::string return_my_string;





					for(size_t i = 1; i <= board->getRows(); i++)
					{

						for(size_t j = 1; j <= board->getColumns(); j++)
						{
							int col_test = j;
							int row_test = i;
							std::vector<int> combination;
							for(size_t k = 0; k < player_hand.size(); k++)
							{

								combination.push_back(1);

							}

				// std::cout << "this is combination: " << std::endl;

				// for(size_t i = 0; i < combination.size(); i++)
				// {


				// 	std::cout << combination[i] << " ";

				// }
				// std::cout << std::endl;
							std::string playerHand_string;

							for(size_t i = 0; i < player_hand.size(); i++)
							{

								playerHand_string += player_hand[i]->getLetter();

							}

					//std::cout << "this is player hand string: " << playerHand_string << std::endl;




							back_track_vertical_start(board, col_test, row_test, combination, playerHand_string,
								prefix_tree, "pass" , return_my_string_vertical, 
								*players[playerNum], dictionary, '?', '?', 0, fourth_letter, best_string_len_vertical, best_string_score_vertical);


							back_track_horizontal_start(board, col_test, row_test, combination, playerHand_string,
								prefix_tree, "pass" , return_my_string_horizontal
								, *players[playerNum], dictionary, '?', '?', 0, fourth_letter, best_string_len_horizontal, best_string_score_horizontal);


							if(return_my_string_horizontal == return_my_string_vertical)
							{

								return_my_string = return_my_string_horizontal;

							}

							else if (return_my_string_horizontal.size() >= return_my_string_vertical.size())
							{

								return_my_string = return_my_string_horizontal;

							}

							else if (return_my_string_horizontal.size() < return_my_string_vertical.size())
							{

								return_my_string = return_my_string_vertical;

							}


							//std::cout << "return_my_string: " << return_my_string << std::endl;



							playerHand_string.clear();

						}
					}			

					try
					{
					// first construct the move, which could throw an exception
						playerMove = Move::parseMove(return_my_string, *players[playerNum]);

					// now execute it, which could also throw exceptions
						playerMove->execute(*board, *bag, *dictionary);

						if(playerMove->isPass())
						{
							++sequentialPasses;
						}
						else
						{
							sequentialPasses = 0;
						}

						correctMove = true;
					}

					catch(MoveException & exception)
					{
					// print error message and reprompt the player
						std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
					}


				}


				if(fourth_letter == 's')
				{


					std::set<Tile*> peek_peek = players[playerNum]->getHandTiles();
					std::set<Tile*>::iterator it;
					std::vector<Tile*>  player_hand;


					for (it = peek_peek.begin(); it != peek_peek.end(); it++)
					{
						player_hand.push_back(*it);
					}


					std::string return_my_string_horizontal = "pass";


					std::string return_my_string_vertical = "pass";

					int best_string_len_horizontal = 0;
					int best_string_len_vertical = 0;
					int best_string_score_horizontal = 0;
					int best_string_score_vertical = 0;

					std::string return_my_string;





					for(size_t i = 1; i <= board->getRows(); i++)
					{

						for(size_t j = 1; j <= board->getColumns(); j++)
						{
							int col_test = j;
							int row_test = i;
							std::vector<int> combination;
							for(size_t k = 0; k < player_hand.size(); k++)
							{

								combination.push_back(1);

							}

				// std::cout << "this is combination: " << std::endl;

				// for(size_t i = 0; i < combination.size(); i++)
				// {


				// 	std::cout << combination[i] << " ";

				// }
				// std::cout << std::endl;
							std::string playerHand_string;

							for(size_t i = 0; i < player_hand.size(); i++)
							{

								playerHand_string += player_hand[i]->getLetter();

							}

					//std::cout << "this is player hand string: " << playerHand_string << std::endl;




							back_track_vertical_start(board, col_test, row_test, combination, playerHand_string,
								prefix_tree, "pass" , return_my_string_vertical, 
								*players[playerNum], dictionary, '?', '?', 0, fourth_letter, best_string_len_vertical, best_string_score_vertical);


							back_track_horizontal_start(board, col_test, row_test, combination, playerHand_string,
								prefix_tree, "pass" , return_my_string_horizontal
								, *players[playerNum], dictionary, '?', '?', 0, fourth_letter, best_string_len_horizontal, best_string_score_horizontal);


							if(return_my_string_horizontal == return_my_string_vertical)
							{

								return_my_string = return_my_string_horizontal;

							}

							else if (best_string_score_horizontal >= best_string_score_vertical)
							{

								return_my_string = return_my_string_horizontal;

							}

							else if (best_string_score_horizontal < best_string_score_vertical)
							{

								return_my_string = return_my_string_vertical;

							}


							playerHand_string.clear();

						}
					}			

					try
					{
					// first construct the move, which could throw an exception
						playerMove = Move::parseMove(return_my_string, *players[playerNum]);

					// now execute it, which could also throw exceptions
						playerMove->execute(*board, *bag, *dictionary);

						if(playerMove->isPass())
						{
							++sequentialPasses;
						}
						else
						{
							sequentialPasses = 0;
						}

						correctMove = true;
					}

					catch(MoveException & exception)
					{
					// print error message and reprompt the player
						std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
					}


				}


			}											 
			//////////////////////////////////////////// //I wrote this end/////////////////////////////////////////////////////


			else
			{
				std::cout << MOVE_PROMPT_COLOR << "Your move, "	<< PLAYER_NAME_COLOR << players[playerNum]->getName() << MOVE_PROMPT_COLOR << ": " << rang::style::reset;
				std::string moveString;
				std::getline(std::cin, moveString);

				try
				{
					// first construct the move, which could throw an exception
					playerMove = Move::parseMove(moveString, *players[playerNum]);

					// now execute it, which could also throw exceptions
					playerMove->execute(*board, *bag, *dictionary);

					if(playerMove->isPass())
					{
						++sequentialPasses;
					}
					else
					{
						sequentialPasses = 0;
					}

					correctMove = true;
				}
				catch(MoveException & exception)
				{
					// print error message and reprompt the player
					std::cout << "Error in move: " << getFriendlyError(exception) << std::endl;
				}

			}

			// draw more tiles from the bag to bring the player up to a full hand
			std::vector<Tile *> newTiles = bag->drawTiles(players[playerNum]->getMaxTiles() - players[playerNum]->getNumTiles());
			players[playerNum]->addTiles(newTiles);

			// tell the user about what new tiles they got
			if(!newTiles.empty())
			{
				std::cout << "Picked up new tiles:" << TILE_NAME_COLOR;
				for(std::vector<Tile *>::iterator newTilesIter = newTiles.begin(); newTilesIter != newTiles.end(); ++newTilesIter)
				{
					std::cout << ' ' << static_cast<char>(std::toupper((*newTilesIter)->getLetter()));
				}
				std::cout << rang::style::reset << std::endl;
			}

			std::cout << "Your current score: " << SCORE_COLOR << players[playerNum]->getPoints() << rang::style::reset << std::endl;

			// wait for player confirmation
			std::cout << std::endl << "Press [enter] to continue.";
			std::cin.ignore();

			// if all players pass for one whole loop, the game is over
			if(sequentialPasses >= players.size())
			{
				gameWon = true;

				// no one gets the bonus
				emptyHandPlayerIndex = -1;
			}

			// if this player has run out of tiles, and no more could be drawn from the bag,
			// the game is also over and they get a bonus
			if(players[playerNum]->getNumTiles() == 0)
			{
				gameWon = true;

				// this player gets the bonus
				emptyHandPlayerIndex = playerNum;
			}

			delete playerMove;
		}
	}
}

	// total up scores
	// -------------------------------------------------------------------------------

doFinalSubtraction(players, emptyHandPlayerIndex);

	// put all players into a multimap by score
std::multimap<unsigned int, Player *> playerScores;
for(std::vector<Player *>::iterator playersIter = players.begin(); playersIter != players.end(); ++playersIter)
{
	playerScores.insert(std::make_pair((*playersIter)->getPoints(), *playersIter));
		//                                                           ^ dis is weird
}

	// get high score by looking at last (=largest) key in multimap
unsigned int highScore = (--playerScores.end())->first;

	// print all players with the high score
std::pair<std::multimap<unsigned int, Player *>::iterator, std::multimap<unsigned int, Player *>::iterator> winningPlayersRange = playerScores.equal_range(highScore);
size_t numWinners = static_cast<size_t>(std::distance(winningPlayersRange.first, winningPlayersRange.second));

if(numWinners == 1)
{
	std::cout << "Winner:";
}
else
{
	std::cout << "Winners:";
}
for(; winningPlayersRange.first != winningPlayersRange.second; ++winningPlayersRange.first)
{
	std::cout << ' ' << PLAYER_NAME_COLOR << winningPlayersRange.first->second->getName();
}
std::cout << rang::style::reset << std::endl;

	// now print score table
std::cout << "Scores: " << std::endl;
std::cout << "---------------------------------" << std::endl;

	// justify all integers printed to have the same amount of character as the high score, left-padding with spaces
std::cout << std::setw(static_cast<uint32_t>(floor(log10(highScore) + 1)));

for(std::multimap<unsigned int, Player *>::const_reverse_iterator playerScoresIter = playerScores.rbegin(); playerScoresIter != playerScores.rend(); ++playerScoresIter)
{
		// note: we print the scores first since the names could be any width but the scores will be all the same width
	std::cout << SCORE_COLOR << playerScoresIter->first << rang::style::reset << " | " << PLAYER_NAME_COLOR << playerScoresIter->second->getName() << rang::style::reset << std::endl;
}
	// cleanup
	// -------------------------------------------------------------------------------

for(size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex)
{
	delete players[playerIndex];
}

delete configFile;
delete board;
delete bag;
delete dictionary;
delete prefix_tree;

}

ConfigFile::ConfigFile(std::string const &configPath)
{
	// This parser uses a state machine to process the file.  It's a bit different from
	// the more common line-based parsers, but a lot more robust.

	enum class ParserState
	{
		LOOKING_FOR_KEY,    // not inside any structures at the moment, waiting for a key or a comment character
		IN_KEY,             // inside key, waiting for equals sign or whitespace
		LOOKING_FOR_VALUE,  // just saw colon, now looking for start of value
		IN_VALUE            // inside value, waiting for newline to end it.
	};

	std::ifstream configFileStream(configPath);
	if(!configFileStream)
	{
		throw FileException("MISSINGCONFIG");
	}

	std::string keyBuffer, valueBuffer;

	char currChar;
	ParserState state = ParserState::LOOKING_FOR_KEY;
	ParserState nextState;
	while(!configFileStream.eof() && configFileStream)
	{
		// read a single character
		configFileStream.get(currChar);

		if(configFileStream.eof())
		{
			break;
		}

		switch(state)
		{
			case ParserState::LOOKING_FOR_KEY:
			if(currChar == ' ' || currChar == '\n')
			{
					// keep looking
				nextState = ParserState::LOOKING_FOR_KEY;
			}
			else
			{
					// found something!
				nextState = ParserState::IN_KEY;
				keyBuffer = currChar;
			}
			break;

			case ParserState::IN_KEY:
			if(currChar == ':')
			{
					// last char of key
				nextState = ParserState::LOOKING_FOR_VALUE;
			}
			else
			{
					// keep scanning key
				nextState = ParserState::IN_KEY;
				keyBuffer += currChar;
			}
			break;

			case ParserState::LOOKING_FOR_VALUE:
			if(currChar == ' ')
			{
					// keep looking
				nextState = ParserState::LOOKING_FOR_VALUE;
			}
			else
			{
					//found start of value
				valueBuffer = currChar;
				nextState = ParserState::IN_VALUE;
			}
			break;

			case ParserState::IN_VALUE:
			if(currChar == '\n' || currChar == ' ')
			{
					//std::cout << "Parsed config value, key: " << keyBuffer << ", value: " << valueBuffer << std::endl;

					// done with value, now set things appropriately
				if(keyBuffer == "HANDSIZE")
				{
					handSize = static_cast<size_t>(std::stoi(valueBuffer));
				}
				else if(keyBuffer == "TILES")
				{
					tilesetFile = valueBuffer;
				}
				else if(keyBuffer == "DICTIONARY")
				{
					dictionaryFile = valueBuffer;
				}
				else if(keyBuffer == "SEED")
				{
					randomSeed = static_cast<uint32_t>(std::stoi(valueBuffer));
				}
				else if(keyBuffer == "BOARD")
				{
					boardFile = valueBuffer;
				}
				else
				{
						// unrecognized key, ignore
				}

				nextState = ParserState::LOOKING_FOR_KEY;
			}
			else
			{
					// keep scanning value
				valueBuffer += currChar;
				nextState = ParserState::IN_VALUE;
			}
			break;

		}

		state = nextState;
	}
}