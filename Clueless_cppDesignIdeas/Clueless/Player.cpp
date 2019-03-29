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
	, _wasMovedToRoomOutOfTurn( false )
	, _hasMovedDuringTurn( false )
	, _hasMadeSuggestionDuringTurn( false )
	, _isReadyToMakeAccusation( false )
	, _hasMadeFalseAccusation( false )
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
	, _wasMovedToRoomOutOfTurn( false )
	, _hasMovedDuringTurn( false )
	, _hasMadeSuggestionDuringTurn( false )
	, _isReadyToMakeAccusation( false )
	, _hasMadeFalseAccusation( false )
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
	if( ! _isReadyToMakeAccusation )
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

	std::cout << "  STUB: build suggestion in " << getLocation()->getName() << "\n";

	clueless::PersonType suspect( clueless::UNKNOWN_PERSON );
	clueless::WeaponType wpn( clueless::UNKNOWN_WEAPON );


	return SolutionCardSet(
		suspect,
		wpn,
		((const Room*)getLocation())->_type ); //room must be current token location

} //end routine buildSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Constructs an Accusation.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
SolutionCardSet
Player::buildAccusation(
	const PersonCard* suspect, //i - suspect (person) card
	const WeaponCard* weapon)  //i - weapon card
const
{
	RoomCard* room_card( nullptr );

	if( getLocation()->isAccusationAllowedHere() )
	{
		//must be room
		room_card = ((const Room*)getLocation())->_assocCard;
	}
	else //not allowed to accuse
	{
	}

	//accusation
	return( SolutionCardSet(*suspect, *weapon, *room_card) );

} //end routine buildAccusation()
