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
	, _numFalseAccusers( 0 )
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
	_players.clear(); //does not own players

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
		_players.push_back( *player_iter ); //class storage

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

	if( player->isGameWinner() ) //winner
	{
		return; //no need for further turn options
	}
	else if( player->isFalseAccuser() ) //false accuser
	{
		//lose turn
		std::cout << "  has made false accusation => no longer active\n";
		return;
	}

	//if player has not moved yet  AND
	//          has not made suggestion
	if( ! player->hasMovedDuringTurn() &&
		! player->hasMadeSuggestionDuringTurn() )
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
		Location* destination( nullptr );

		//consult player for preference
		destination = player->provideMovePreference(move_options);

		if( ! destination ) //no clear preference
		{
			//note: may have down-selected to fewer options
			std::cout << "  random move choice... ";
			destination = _board.chooseLocation(move_options);
		}

		_board.movePlayerTo(player, destination);
		player->indicateHasMovedDuringTurn();

		std::cout << "  move to " << destination->getName() << "\n";
	}
		break;

	case clueless::MAKE_SUGGESTION:
	{
		//build suggestion
		SolutionCardSet suggestion( player->buildSuggestion() );

		notifyAllPlayers_playerMadeSuggestion( player->getCharacterName() );

		//pull suspected person and weapon into room
		clueless::PersonType suggested_person( suggestion.getPersonType() );
		clueless::WeaponType suggested_weapon( suggestion.getWeaponType() );
		clueless::RoomType suggested_room( suggestion.getRoomType() );

		if( _board.movePersonTokenToRoom(suggested_person, suggested_room) )
		{
			notifyAllPlayers_personTokenMovedForSuggestion(suggested_person, suggested_room);
		}

		if( _board.moveWeaponTokenToRoom(suggested_weapon, suggested_room) )
		{
			notifyAllPlayers_weaponTokenMovedForSuggestion(suggested_weapon, suggested_room);
		}

		//offer suggestion for other players to refute
		clueless::PersonType opponent_providing_counter_evidence( clueless::UNKNOWN_PERSON );

		const Card* counter_evidence(
			requestCounterEvidenceToPlayerSuggestion(
				player, //suggestor
				&suggestion,
				opponent_providing_counter_evidence) );

		//provide feedback to suggestor
		player->acceptCounterEvidence(
			&suggestion,
			counter_evidence,
			opponent_providing_counter_evidence );

		player->indicateHasMadeSuggestionDuringTurn();

		if( counter_evidence )
		{
			notifyAllPlayers_playerRefutedSuggestion( opponent_providing_counter_evidence );
		}
		else //unrefuted
		{
			notifyAllPlayers_playerSuggestionUnrefuted( player->getCharacterName() );
		}
	}
		break;

	case clueless::MAKE_ACCUSATION:
	{
		//build accusation
		SolutionCardSet accusation( player->buildAccusation() );

		//compare accusation to Case File
		if( _cards.doesAccusationMatchCaseFile( accusation ) )
		{
			//player wins game :)
			_winner = player;
			player->indicateIsGameWinner();

			notifyAllPlayers_gameWinner( player );
		}
		else //accusation does not match case file
		{
			//player is false accuser
			player->indicateHasMadeFalseAccusation();
			++_numFalseAccusers;

			notifyAllPlayers_falseAccuser( player );
		}
	}
		break;

	case clueless::DO_NOTHING_ELSE_THIS_TURN:
	default:
		;

	} //end switch (on player's choice)

} //end routine executePlayerChoice()


////////////////////////////////////////////////////////////////////////////////
/// \brief Request counter-evidence from opponents for player's suggestion.
/// \param Player: current suggestor
/// \param SolutionCardSet: suggestion
/// \param Player: opponent with counter-evidence (character)
/// \return Card: counter-evidence to suggestion
/// \throw None
/// \note
/// - It is important to ask each opponent for counter-evidence in the order of
///   play, starting with the opponent whose turn is next after suggestor.
////////////////////////////////////////////////////////////////////////////////
const Card*
Game::requestCounterEvidenceToPlayerSuggestion(
	const Player* suggestor, //i - suggestor
	const SolutionCardSet* suggestion, //i - suggestion
	clueless::PersonType& opponent_character) // o- opponent with counter-evidence
const
{
	//have not found counter-evidence for suggestor yet
	const Card* counter_evidence( nullptr );

	//determine suggestor iterator
	bool have_found_suggestor_iter( false );
	std::list<Player*>::const_iterator suggestor_iter( _players.begin() );
	while( ! have_found_suggestor_iter &&
		(_players.end() != suggestor_iter) )
	{
		if( *suggestor_iter == suggestor )
		{
			have_found_suggestor_iter = true;
		}
		else //not suggestor
		{
			++suggestor_iter;
		}
	}

	//player following suggestor
	std::list<Player*>::const_iterator player_iter( ++suggestor_iter );

	//while have not found counter-evidence  AND
	///     more players in list after suggestor
	while( ! counter_evidence &&
		(_players.end() != player_iter) )
	{
		counter_evidence = (*player_iter)->offerEvidenceCounterToSuggestion(suggestion, suggestor);
		opponent_character = (*player_iter)->getCharacter();

		++player_iter; //next player

	} //end while (more players in list after suggestor)

	//continue at start of player list
	player_iter = _players.begin();
	
	//while have not found counter-evidence  AND
	//      more players in list before suggestor
	while( ! counter_evidence &&
		(suggestor_iter != player_iter) )
	{
		counter_evidence = (*player_iter)->offerEvidenceCounterToSuggestion(suggestion, suggestor);
		opponent_character = (*player_iter)->getCharacter();

		++player_iter; //next player

	} //end while (more players in list before suggestor)

	//if no counter-evidence uncovered
	if( ! counter_evidence )
	{
		//clear last opponent
		opponent_character = clueless::UNKNOWN_PERSON;
	}

	return counter_evidence;

} //end routine requestCounterEvidenceToPlayerSuggestion()


//------------------------------------------------------------------------------
// Player Notifications
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Notify all Players of 
/// \param string: suggestor's character name
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Game::notifyAllPlayers_playerMadeSuggestion(
	const std::string& suggestor) //i - suggestor's character name
const
{
	std::cout << "\nNotice to All Players...\n"
		<< suggestor << " made a Suggestion\n";

} //end routine notifyAllPlayers_playerMadeSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Notify all Players of 
/// \param Player: 
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Game::notifyAllPlayers_playerRefutedSuggestion(
	clueless::PersonType refuter) //i - player's character
const
{
	std::cout << "\nNotice to All Players...\n"
		<< clueless::translatePersonTypeToText(refuter) << " refuted Suggestion\n";

} //end routine notifyAllPlayers_playerRefutedSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Notify all Players of 
/// \param string: suggestor's character name
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Game::notifyAllPlayers_playerSuggestionUnrefuted(
	const std::string& suggestor) //i - suggestor's character name
const
{
	std::cout << "\nNotice to All Players...\n"
		<< suggestor << "\'s Suggestion is unrefuted\n";

} //end routine notifyAllPlayers_playerSuggestionUnrefuted()


void
Game::notifyAllPlayers_personTokenMovedForSuggestion(
	clueless::PersonType character, //i - person token moved
	clueless::RoomType new_location) //i - new location
const
{
	std::cout << "\nNotice to All Players...\n"
		<< clueless::translatePersonTypeToText( character ) << " moved to "
		<< clueless::translateRoomTypeToText( new_location ) << " in support of suggestion\n";

} //end routine notifyAllPlayers_personTokenMovedForSuggestion()


void
Game::notifyAllPlayers_weaponTokenMovedForSuggestion(
	clueless::WeaponType wpn, //i - weapon token moved
	clueless::RoomType new_location) //i - new location
const
{
	std::cout << "\nNotice to All Players...\n"
		<< clueless::translateWeaponTypeToText( wpn ) << " moved to "
		<< clueless::translateRoomTypeToText( new_location ) << " in support of suggestion\n";

} //end routine notifyAllPlayers_weaponTokenMovedForSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Notify all Players of game winner and Case File solution.
/// \param Player: winner
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Game::notifyAllPlayers_gameWinner(
	const Player* winner) //i - game winner
const
{
	std::cout << "\n*** " << winner->getName() << " WINS game\n"
		<< "solution: " << _cards.reportCaseFile().str();

} //end routine notifyAllPlayers_gameWinner()


////////////////////////////////////////////////////////////////////////////////
/// \brief Notify all Players of false accusation by Player.
/// \param Player: false accuser
/// \return None
/// \throw None
/// \note
/// - Do not share contents of false accusation with all players.
////////////////////////////////////////////////////////////////////////////////
void
Game::notifyAllPlayers_falseAccuser(
	const Player* false_accuser) //i - false accuser
const
{
	std::cout << "\n*** " << false_accuser->getName() << " has made a false accusation\n"
		<< "    eliminated from active game play\n";

} //end routine notifyAllPlayers_falseAccuser()

