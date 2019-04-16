////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Player.cpp
/// \brief
///
/// \date   01 Mar 2019  1527
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Player.h"

#include "Card.h"				//for Card, RoomCard use
#include "GamePiece.h"
#include "Location.h"
#include "Room.h"
#include "SolutionCardSet.h"

#include "CluelessEnums.h"	//for RoomType use
#include "mersenneTwister.h"

#include <algorithm>		//for std::min() use
#include <limits.h>			//for UINT_MAX use

//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Default constructor
/// \param None
/// \return None
/// \throw None
/// \note
/// - Player cannot be marked as the "game creator".
////////////////////////////////////////////////////////////////////////////////
Player::Player()
	: _name( "" )
	, _isGameCreator( false )
	, _character( clueless::UNKNOWN_PERSON )
	, _assocGameToken( nullptr )
	, _notebook( clueless::UNKNOWN_PERSON )
	, _wasMovedToRoomOutOfTurn( false )
	, _hasMovedDuringTurn( false )
	, _hasMadeSuggestionDuringTurn( false )
	//, _isReadyToMakeAccusation( false )
	, _hasMadeFalseAccusation( false )
	, _isGameWinner( false )
{
} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param string: name
/// \param PersonType: game character
/// \param bool: is game creator?
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Player::Player(
	std::string name, //i - 
	clueless::PersonType game_character, //i - 
	bool is_game_creator) //i - whether player requested game creation
	: _name( name )
	, _isGameCreator( is_game_creator )
	, _character( game_character )
	, _assocGameToken( nullptr )
	, _notebook( game_character )
	, _wasMovedToRoomOutOfTurn( false )
	, _hasMovedDuringTurn( false )
	, _hasMadeSuggestionDuringTurn( false )
	//, _isReadyToMakeAccusation( false )
	, _hasMadeFalseAccusation( false )
	, _isGameWinner( false )
{
} //end routine Player(name, character)


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
std::string
Player::getCharacterName()
const
{
	if( _assocGameToken )
	{
		return _assocGameToken->getName();
	}

	return( clueless::translatePersonTypeToText( _character ) );

} //end routine getCharacterName()


////////////////////////////////////////////////////////////////////////////////
void
Player::acceptAssocCharacterToken(
	PersonPiece* token) //i - associated character game token
{
	if( token->_person != _character )
	{
		std::ostringstream msg;
		msg << "Player::acceptAssocCharacterToken()\n"
			<< "  INCONSISTENT_DATA\n"
			<< "  token for \'" << clueless::translatePersonTypeToText(token->_person)
			<< "\' does not match character choice of \'" << getCharacterName() << "\'";
		throw std::logic_error( msg.str().c_str() );

	} //end if (token does not match character choice)

	//otherwise, match => accept token association
	_assocGameToken = token;

} //end routine acceptAssocCharacterToken()


////////////////////////////////////////////////////////////////////////////////
const Location*
Player::getLocation()
const
{
	const Location* location( nullptr );

	if( _assocGameToken )
	{
		location = _assocGameToken->_location;
	}

	return location;

} //end routine getLocation()


std::string
Player::getLocationName()
const
{
	if( _assocGameToken )
	{
		return _assocGameToken->_location->getName();
	}

	return "location not specified";

} //end routine getLocationName()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether specified card is in player's hand.
/// \param Card: card of interest
/// \return bool: whether card in hand
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Player::isCardInHand(
	const Card* card) //i - card of interest
const
{
	return( _hand.end() != _hand.find(card) );

} //end routine isCardInHand()


//bool
//Player::isRoomInHand(
//	const Room* room) //i - room of interest
//const
//{
//	bool is_in_hand( false );
//
//	std::set<const Card*>::const_iterator card_iter( _hand.begin() );
//	while( ! is_in_hand &&
//		(_hand.end() != card_iter) )
//	{
//		if( clueless::ROOM == (*card_iter)->_type )
//		{
//			//matching room type
//			if( room->_type == ((const RoomCard*)(*card_iter))->_room )
//			{
//				is_in_hand = true;
//			}
//			else
//			{
//				++card_iter;
//			}
//		}
//		else //not room card
//		{
//			++card_iter;
//		}
//	} //end while (more cards to consider)
//
//	return is_in_hand;
//
//} //end routine isRoomInHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief Full report of player state.
/// \param None
/// \return ostringstream: reported information
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::ostringstream
Player::report()
const
{
	std::ostringstream report;

	report
		<< getName() << " (" << getCharacterName() << ")\n"
		<< " location: " << getLocationName() << "\n"
		<< reportHand().str();

	return report;

} //end routine report()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::ostringstream
Player::reportHand()
const
{
	std::ostringstream report;

	/// \note prefer specific order to mimic board schematic
	report
		<< " hand:";

	if( ! _hand.empty() ) //at least one card
	{
		report << "\n";

		std::set<const Card*>::const_iterator card_iter( _hand.begin() );
		while( _hand.end() != card_iter )
		{
			report
				<< "   " << (*card_iter)->getName() << "\n";

			++card_iter;

		} //end while (more cards)

		report << "\n";
	}
	else //empty hand
	{
		report << " -empty-\n";

	} //end else (empty hand)


	return report;

} //end routine reportHand()


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Added dealt card to hand.
/// \param Card: card to add
/// \return None
/// \throw None
/// \note
/// - Player uses card but does not own it.
////////////////////////////////////////////////////////////////////////////////
void
Player::addCardToHand(
	const Card* card) //i - card to join hand
{
	_hand.insert( card );
	_notebook.recordCardInHand( card );

} //end routine addCardToHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief Clears internal flags in preparation for new turn.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Player::prepareForNewTurn()
{
	_wasMovedToRoomOutOfTurn = false;

	_hasMovedDuringTurn = false;
	_hasMadeSuggestionDuringTurn = false;

} //end routine prepareForNewTurn()


////////////////////////////////////////////////////////////////////////////////
/// \brief Makes selection amongst options for current turn.
/// \param set<TurnOptionType>: valid options for current turn
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
clueless::TurnOptionType
Player::makeTurnChoice(
	std::set<clueless::TurnOptionType>* options) //i - valid options for turn
const
{
	clueless::TurnOptionType choice( clueless::DO_NOTHING_ELSE_THIS_TURN );

	//if not ready to make accusation
	if( ! isReadyToMakeAccusation() )
	{
		if( 1 == options->size() ) //only one option
		{
			//choose it
			choice = *(options->begin());
		}
		else
		{
			//create position based on floor of random draw in [0, num choices)
			size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, (double)(options->size())) )) );

			std::set<clueless::TurnOptionType>::const_iterator opt_iter( options->begin() );
			for(size_t pos_index(0);
				pos_index < zero_based_pos;
				++pos_index)
			{
				++opt_iter;
			}

			choice = *opt_iter;
		}
	}
	else //ready to make accusation
	{
		choice = clueless::MAKE_ACCUSATION;
	}

	std::cout
		<< "  " << getName() << " chooses to "
		<< clueless::translateTurnOptionTypeToText( choice ) << "\n";

	return choice;

} //end routine makeTurnChoice


////////////////////////////////////////////////////////////////////////////////
/// \brief Offers preference between multiple move options.
/// \param set<Location>: move options
/// \return Location: preferred move destination; null if no preference
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Location*
Player::provideMovePreference(
	std::set<Location*>* move_options) //i - move options
const
{
	Location* preferred_destination( nullptr );
	std::map<Location*, unsigned int> prioritized_options;
	std::set<Location*>::iterator opt_iter( move_options->begin() );

	if( 1 < move_options->size() ) //more than one option
	{
		//if have suspected room
		if( _notebook.haveSuspectedRoom() )
		{
			/// \todo closest "in hand" or suspected... "in hand" preferred

			while( ! preferred_destination &&
				(move_options->end() != opt_iter) )
			{
				if( (*opt_iter)->isRoom() )
				{
					// 1. room in hand or suspected
					prioritized_options[*opt_iter] =
						(unsigned int)
						_notebook.determineShortestPathForDestinationSuspectedOrInHand(
							getLocation(), //current location
							(*opt_iter)); //next step

				}
				else //destination not room (hallway)
				{
					// 1. room still needing questioning
					prioritized_options[*opt_iter] =
						(unsigned int)
						_notebook.determineShortestPathForDestinationSuspectedOrInHand(
							getLocation(), //current location (probably room)
							(*opt_iter) ); //next step on path

										   //prioritized_options[*opt_iter] = 15;
				}

				++opt_iter; //next option


				//if( _notebook.isRoomInHand( ((Room*)(*opt_iter))->_type ) )
				//{
				//	preferred_destination = *opt_iter;
				//}
				//else //not in hand
				//{
				//	++opt_iter;
				//}
			} //end while (more options to prioritize)

		}
		else //no suspected room
		{
			while( move_options->end() != opt_iter )
			{
				// 2: destination is room
				if( (*opt_iter)->isRoom() )
				{
					// 1. room still needing questioning
					prioritized_options[*opt_iter] =
						(unsigned int)
						_notebook.determineShortestPathForDestinationNeedingQuestion(
							getLocation(), //current location
							(*opt_iter) ); //next step

					//// 3: room without counter-evidence
					//if( ! _notebook.haveCounterEvidenceForRoom( (Room*)(*opt_iter) ) )
					//{
					//	prioritized_options[*opt_iter] = 5;
					//}
					//else //have counter-evidence for room
					//{
					//	// 4: in hand
					//	if( _notebook.isRoomInHand( ((Room*)(*opt_iter))->_type ) )
					//	{
					//		prioritized_options[*opt_iter] = 7;
					//	}
					//	else //not in hand
					//	{
					//		prioritized_options[*opt_iter] = 10;
					//	}
					//} //end else (have counter-evidence for room)
				}
				else //destination not room (hallway)
				{
					// 1. room still needing questioning
					prioritized_options[*opt_iter] =
						(unsigned int)
						_notebook.determineShortestPathForDestinationNeedingQuestion(
							getLocation(), //current location (probably room)
							(*opt_iter) ); //next step on path

					//prioritized_options[*opt_iter] = 15;
				}

				++opt_iter; //next option

			} //end while (more options to prioritize)

		} //end else (no suspected room)

		//determine greatest priority (lowest value) option(s)
		unsigned int lowest_value( UINT_MAX );
		std::map<Location*, unsigned int>::iterator pri_iter( prioritized_options.begin() );
		for(pri_iter  = prioritized_options.begin();
			pri_iter != prioritized_options.end();
			++pri_iter)
		{
			//curr option lower value
			if( lowest_value > pri_iter->second )
			{
				move_options->clear();

				move_options->insert( pri_iter->first );
			}
			else if( lowest_value == pri_iter->second )
			{
				move_options->insert( pri_iter->first );
			}

			lowest_value = std::min(lowest_value, pri_iter->second);

		} //end for (each prioritized option)
	} //end if (more than one move option supplied)

	if( 1 == move_options->size() )
	{
		preferred_destination = *(move_options->begin());
	}

	return preferred_destination;

} //end routine provideMovePreference()


////////////////////////////////////////////////////////////////////////////////
/// \brief Constructs a Suggestion.
/// \param None
/// \return None
/// \throw
/// - LOGIC_ERROR if player's token not in a room.
/// \note
/// - Room must be current location.
////////////////////////////////////////////////////////////////////////////////
SolutionCardSet
Player::buildSuggestion()
const
{
	if( ! getLocation()->isRoom() )
	{
		std::cout
			<< "A suggestion must be made from a room.\n"
			<< getName() << " is currently in the " << getLocation()->getName();

		//exit routine without exception
		return SolutionCardSet(
			clueless::UNKNOWN_PERSON,
			clueless::UNKNOWN_WEAPON,
			((const Room*)getLocation())->_type );
	}

	clueless::PersonType suspect( _notebook.choosePersonForSuggestion() );
	clueless::WeaponType wpn( _notebook.chooseWeaponForSuggestion() );

	SolutionCardSet suggestion(
		suspect,
		wpn,
		((const Room*)getLocation())->_type ); //room must be current token location

	std::cout << "  suggestion... " << suggestion.report().str() << "\n";

	return suggestion;

} //end routine buildSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Accepts counter-evidence to Suggestion.
/// \param SolutionCardSet: suggestion posed to opponents
/// \param Card: counter-evidence
/// \param PersonType: opponent offering counter-evidence (character)
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Player::acceptCounterEvidence(
	const SolutionCardSet* suggestion, //i - suggestion
	const Card* const counter_evidence, //i - counter-evidence
	clueless::PersonType opponent) //i - opponent offering counter-evidence
{
	if( counter_evidence )
	{
		//record in detective's notebook
		_notebook.recordCardShownByPlayer( counter_evidence, opponent );
	}
	else //no counter_evidence
	{
		//----------------------------------------------------------------------
		// person
		//----------------------------------------------------------------------
		//if card not in hand
		if( ! isCardInHand( &(suggestion->_person) ) )
		{
			_notebook.notifyNoCounterEvidenceForCard( &(suggestion->_person) );
		}

		//----------------------------------------------------------------------
		// weapon
		//----------------------------------------------------------------------
		//if card not in hand
		if( ! isCardInHand( &(suggestion->_weapon) ) )
		{
			_notebook.notifyNoCounterEvidenceForCard( &(suggestion->_weapon) );
		}

		//----------------------------------------------------------------------
		// room
		//----------------------------------------------------------------------
		//if card not in hand
		if( ! isCardInHand( &(suggestion->_room) ) )
		{
			_notebook.notifyNoCounterEvidenceForCard( &(suggestion->_room) );
		}

	} //end else (no counter-evidence)

} //end routine acceptCounterEvidence()


////////////////////////////////////////////////////////////////////////////////
/// \brief Offers counter-evidence to opponent's suggestion.
/// \param SolutionCardSet: suggestion to refute
/// \param Player: opponent with suggestion
/// \return Card: counter-evidence; null if no counter-evidence
/// \throw None
/// \note
/// - If have counter-evidence, must let opponent know.
/// - If can refute more than one element of suggestion, still only show one
///   card to opponent.
////////////////////////////////////////////////////////////////////////////////
const Card*
Player::offerEvidenceCounterToSuggestion(
	const SolutionCardSet* suggestion, //i - suggestion
	const Player* suggestor) //i - player authoring suggestion
{
	const Card* counter_evidence( nullptr );

	//find counter-evidence in hand
	std::set<const Card*> in_hand( findCounterEvidenceInHand(suggestion) );

	//if found counter-evidence
	if( ! in_hand.empty() )
	{
		//decide which card to show opponent
		counter_evidence = _notebook.decideWhichCardToShowOpponent( &in_hand, suggestor->getCharacter() );

		//make note that showed card to suggestor
		_notebook.recordHaveShownCardToPlayer(counter_evidence, suggestor->getCharacter());

		std::cout
			<< "  " << getName() << " showed counter-evidence card \'"
			<< counter_evidence->getName() << "\' to " << suggestor->getName() << "\n";
	}
	//otherwise, found no counter-evidence

	return counter_evidence;

} //end routine offerEvidenceCounterToSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Searches in hand for counter-evidence to suggestion.
/// \param SolutionCardSet: suggestion
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::set<const Card*>
Player::findCounterEvidenceInHand(
	const SolutionCardSet* suggestion) //i - suggestion
const
{
	std::set<const Card*> counter_evidence;
	std::set<const Card*>::const_iterator card_iter( _hand.begin() );

	//while have not found three pieces of counter-evidence  AND
	//      more cards in hand
	while(
		(3 > counter_evidence.size() ) &&
		(_hand.end() != card_iter) )
	{
		//if current card matches element of suggestion
		if( suggestion->doesCardMatchAnElement(*card_iter) )
		{
			//add to counter-evidence
			counter_evidence.insert( *card_iter );
		}

		++card_iter; //next card in hand

	} //end while (more to look for)

	return counter_evidence;

} //end routine findCounterEvidenceInHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief Constructs an Accusation based on detective's notebook entries.
/// \param None
/// \return SolutionCardSet: accusation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
SolutionCardSet
Player::buildAccusation()
const
{
	if( ! isReadyToMakeAccusation() )
	{
		std::cout << "  STUB: add details to determine elements to make accusation\n";
	}

	//accusation
	return( _notebook.getAccusation() );

} //end routine buildAccusation()
