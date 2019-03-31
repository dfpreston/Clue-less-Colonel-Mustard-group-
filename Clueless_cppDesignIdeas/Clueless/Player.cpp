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
inline std::string
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
			size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, options->size()) )) );

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
/// \param Card: counter-evidence
/// \param PersonType: opponent offering counter-evidence (character)
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Player::acceptCounterEvidence(
	const Card* const counter_evidence, //i - counter-evidence
	clueless::PersonType opponent) //i - opponent offering counter-evidence
{
	if( counter_evidence )
	{
		//record in detective's notebook
		_notebook.recordCardShownByPlayer( counter_evidence, opponent );
	}

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
Player::offerCounterEvidenceToSuggestion(
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
		counter_evidence = _notebook.decideWhichCardToShowOpponent( &in_hand );

		//make note that showed card to suggestor
		_notebook.recordHaveShownCardToPlayer(counter_evidence, suggestor->getCharacter());

		std::cout
			<< "  " << getName() << " showed counter-evidence card \'"
			<< counter_evidence->getName() << "\' to " << suggestor->getName() << "\n";
	}
	//otherwise, found no counter-evidence

	return counter_evidence;

} //end routine offerCounterEvidenceToSuggestion()


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
