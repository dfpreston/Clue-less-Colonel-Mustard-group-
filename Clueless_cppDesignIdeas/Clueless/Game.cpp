////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Board.cpp
/// \brief
///
/// \date   25 Feb 2019  1457
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Game.h"

#include "CluelessEnums.h"	//for RoomType use
//#include "mersenneTwister.h"
#include "Player.h"

#include <iostream>			//for std::cout use
#include <sstream>			//for std::ostringstream use
#include <stdexcept>		//for std::logic_error use

//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Default constructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Game::Game()
	: _winner( nullptr )
{
} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Destructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{
} //end routine destructor


//--------------------------------------------------------------------------
// Additional Member Functions
//--------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Creates and sets up board, cards, and game tokens.
/// \param None
/// \return None
/// \throw
/// - INSUFFICIENT_DATA when empty collection of players.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Game::setup(
	std::list<Player*>* players) //i - all players
{
	if( (*players).empty() )
	{
		std::ostringstream msg;
		msg << "Game::setup()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  no players known";
		throw std::logic_error( msg.str() );
	}

	//create bridge between player and character / game token
	_board.recognizePlayerCharacterAssignments( players );
	_board.buildRoomRelationshipsWithCards( _cards.getRoomCards() );

	//deal cards
	_cards.setup( players );

	std::list<Player*>::const_iterator player_iter( (*players).begin() );
	for(player_iter  = (*players).begin();
		player_iter != (*players).end();
		++player_iter )
	{
		std::cout << (*player_iter)->report().str();

	} //end for (each player)

	std::cout << _board.report().str() << "\n";

} //end routine setup()


////////////////////////////////////////////////////////////////////////////////
/// \brief Execute complete turn for specified player.
/// \param Player: current turn taker
/// \return None
/// \throw None
/// \note
/// - Assume: player object exists
////////////////////////////////////////////////////////////////////////////////
void
Game::executePlayerTurn(
	Player* const player) //i - turn taker
{
	std::cout << "\n" << player->getName() << "'s Turn (" << player->getCharacterName() << ")...\n";

	//if player made false Accusation
	if( player->hasMadeFalseAccusation() )
	{
		std::cout << "  has made false accusation\n";
		return;
	}

	//--------------------------------------------------------------------------
	// determine move options
	//--------------------------------------------------------------------------
	//if moved to current room by game play since last turn,
	if( player->wasMovedToRoomOutOfTurn() )
	{
		//allowed to stay in room and make suggestion
		std::cout << "STUB: allow to stay in room and make suggestion\n";

		player->clearMovedToRoomOutOfTurnIndicator();
	}

	std::set<Location*> move_options = player->getLocation()->getMoveOptions();

	//if no permissible moves
	if( 0 == move_options.size() )
	{
		std::cout << "no permissible moves\n";
		return;
	}

	//if only one option
	if( 1 == move_options.size() )
	{
		Location* destination( *(move_options.begin()) );
		_board.movePlayerTo(player, destination);

		std::cout << "move to " << destination->getName() << "\n";
	}
	else //multiple move options
	{
		std::cout << "random move choice... ";
		Location* destination( _board.chooseLocation( &move_options ) );

		_board.movePlayerTo(player, destination);

		std::cout << "move to " << destination->getName() << "\n";
	}

	//if in room
	if( player->getLocation()->isRoom() )
	{
		//build suggestion
		std::cout << "STUB: build suggestion in " << player->getLocation()->getName() << "\n";
	}

} //end routine executePlayerTurn()
