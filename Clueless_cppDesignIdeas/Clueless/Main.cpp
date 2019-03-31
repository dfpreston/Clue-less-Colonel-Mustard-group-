////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Main.cpp
/// \brief
///
/// \date   25 Feb 2019  1227
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Board.h"
#include "CardDeck.h"
#include "Game.h"
#include "Player.h"
#include "SolutionCardSet.h"

#include "CluelessEnums.h"	//for PersonType use

#include <iostream>			//for std::cout use
#include <list>				//for std:list use
#include <stdio.h>
#include <stdexcept>		//for std::exception use


//void main()
int main( int argc, const char* argv[] )
{
	try
	{
		/// \note first player is marked as game creator
		std::list<Player*> players;
		Player player_1("Player 1", clueless::COLONEL_MUSTARD, true);
		players.push_back( &player_1 );

		Player player_2("Player 2", clueless::PROFESSOR_PLUM);
		players.push_back( &player_2 );

		Player player_3("Player 3", clueless::MISS_SCARLET);
		players.push_back( &player_3 );


		Game clueless;
		clueless.setup( &players );

		unsigned int turn_number( 0 );
		const unsigned int MAX_NUM_TURNS( 100 );

		while( ! clueless.hasWinner() &&
			(clueless._numFalseAccusers < clueless._players.size()) &&
			(MAX_NUM_TURNS > turn_number) )
		{
			if( 0 == turn_number % 5 )
			{
				std::cout << "--------------------------------------------------------------------------------\n";
			}
			else
			{
				std::cout
					<< "................................................................................\n";
			}
			std::cout << "Turn #" << (turn_number + 1) << "\n";

			//----------------------------------------------------------------------
			// Turn for Each Player
			//----------------------------------------------------------------------
			std::list<Player*>::const_iterator player_iter( players.begin() );
			while( ! clueless.hasWinner() &&
				(players.end() != player_iter) )
			{
				clueless.executePlayerTurn( *player_iter );

				++player_iter;

			} //end while (more players)

			std::cout << "\n";

			++turn_number; //next turn

		} //end while (reason to continue turns)

		std::cout << "\n";
	}
	catch(const std::exception& error)
	{
		std::cout << error.what() << "\n\n";
	}

	return 0;

} //end routine main


