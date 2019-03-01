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
#include "Player.h"

#include "CluelessEnums.h"	//for PersonType use

#include <iostream>			//for std::cout use
#include <stdio.h>
#include <stdexcept>		//for std::exception use


//void main()
int main( int argc, const char* argv[] )
{
	try
	{
		Board clueless_board;

		std::cout << clueless_board.report().str() << "\n";
		std::cout << clueless_board._hallways.size() << " Hallways\n";
		std::cout << clueless_board._personHomes.size() << " Character Starting Spots\n";

		CardDeck cards;

		std::cout << "\nPlayer / Character assignments...\n";
		Player player_1("Player 1", clueless::COLONEL_MUSTARD);
		clueless_board.assignCharacterToPlayer(&player_1, clueless::COLONEL_MUSTARD);

		Player player_2("Player 2", clueless::PROFESSOR_PLUM);
		clueless_board.assignCharacterToPlayer(&player_1, clueless::PROFESSOR_PLUM);

		Player player_3("Player 3", clueless::MISS_SCARLET);
		clueless_board.assignCharacterToPlayer(&player_1, clueless::MISS_SCARLET);

		std::cout << "\n";
	}
	catch(const std::exception& error)
	{
		std::cout << error.what() << "\n\n";
	}

	return 0;

} //end routine main


