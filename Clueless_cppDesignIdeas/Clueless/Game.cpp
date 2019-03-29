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
#include "SolutionCardSet.h"

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
	if( player->isFalseAccuser() )
	{
		std::cout << "  has made false accusation => skip turn\n";
		return;
	}

	std::set<clueless::TurnOptionType> turn_options;
	std::set<Location*> move_options( player->getLocation()->getMoveOptions() );

	//--------------------------------------------------------------------------
	// determine move options
	//--------------------------------------------------------------------------
	do
	{
		determinePlayerTurnOptions(player, &move_options, &turn_options);

		//if at least one option
		if( ! turn_options.empty() )
		{
			clueless::TurnOptionType choice( player->makeTurnChoice(&turn_options) );

			if( clueless::DO_NOTHING_ELSE_THIS_TURN != choice )
			{
				executePlayerChoice(player, choice, &move_options);
			}
		} //end if (at least one turn option)

	} while ( ! turn_options.empty() );

	//clear per turn information so ready for next turn
	player->prepareForNewTurn();

} //end routine executePlayerTurn()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determine player's options for current turn.
/// \param Player: current turn taker
/// \param set<TurnOptionType>: valid turn options
/// \param set<Location>: valid move options... may be empty
/// \return None
/// \throw None
/// \note
/// - Assume: player object exists
////////////////////////////////////////////////////////////////////////////////
void
Game::determinePlayerTurnOptions(
	Player* const player,
	const std::set<Location*>* const move_options, //i - valid move options
	std::set<clueless::TurnOptionType>* turn_options) // o- valid turn options
const
{
	turn_options->clear(); //no known options yet

	if( player->isFalseAccuser() ) //false accuser
	{
		//lose turn
		std::cout << "  has made false accusation => no longer active\n";
		return;
	}

	if( ! player->hasMovedDuringTurn() ) //player has not moved yet
	{
		//if possible moves found
		if( 0 < move_options->size() )
		{
			turn_options->insert( clueless::MOVE );
		}
		else //no permissible moves
		{
			std::cout << "no permissible moves\n";
		}

		//if moved to current room by game play since last turn AND
		//   have not made suggestion
		if( player->wasMovedToRoomOutOfTurn() &&
			! player->hasMadeSuggestionDuringTurn() )
		{
			//allowed to stay in room and make suggestion
			std::cout << "  allow to stay in room and make suggestion\n";
			turn_options->insert( clueless::MAKE_SUGGESTION );
		}

	}
	else if(	//player has moved during turn  AND
		player->getLocation()->isRoom() &&        // in room  AND
		! player->hasMadeSuggestionDuringTurn() ) // has not made suggestion
	{
		turn_options->insert( clueless::MAKE_SUGGESTION );
	}

	/// \note "make accusation" always option but only include if ready to make
	///  accusation (for now) -- 29 Mar 2019, mem
	if( player->isReadyToMakeAccusation() )
	{
		turn_options->insert( clueless::MAKE_ACCUSATION );
	}

} //end routine determinePlayerTurnOptions()


////////////////////////////////////////////////////////////////////////////////
/// \brief Execute player's choice during turn.
/// \param Player: current turn taker
/// \param TurnOptionType: player's choice
/// \param set<Location>: move options... may be empty collection
/// \return None
/// \throw None
/// \note
/// - Assume: player object exists
////////////////////////////////////////////////////////////////////////////////
void
Game::executePlayerChoice(
	Player* const player, //i - turn taker
	clueless::TurnOptionType choice, //i - turn option choice
	std::set<Location*>* const move_options) //i - move options
{
	switch( choice )
	{
	case clueless::MOVE:
	{
		//if only one option
		if( 1 == move_options->size() )
		{
			Location* destination( *(move_options->begin()) );
			_board.movePlayerTo(player, destination);

			std::cout << "  move to " << destination->getName() << "\n";
		}
		else //multiple move options
		{
			std::cout << "  random move choice... ";
			Location* destination( _board.chooseLocation( move_options ) );

			_board.movePlayerTo(player, destination);

			std::cout << "  move to " << destination->getName() << "\n";
		}

		player->indicateHasMovedDuringTurn();
	}
		break;

	case clueless::MAKE_SUGGESTION:
	{
		//build suggestion
		SolutionCardSet suggestion( player->buildSuggestion() );

		/// \todo offer suggestion for other players to refute

		/// \todo provide feedback to suggestor

		player->indicateHasMadeSuggestionDuringTurn();
	}
		break;

	case clueless::MAKE_ACCUSATION:
	{
		//build accusation
		std::cout << "  STUB: build accusation\n";
	}
		break;

	case clueless::DO_NOTHING_ELSE_THIS_TURN:
	default:
		;

	} //end switch (on player's choice)

} //end routine executePlayerChoice()
