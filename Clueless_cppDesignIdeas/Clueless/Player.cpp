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
Player::Player()
	: _name( "" )
	, _character( clueless::UNKNOWN_PERSON )
	, _assocGameToken( nullptr )
	, _wasMovedToRoomOutOfTurn( false )
	, _hasMadeFalseAccusation( false )
{
} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Player::Player(
	std::string name, //i - 
	clueless::PersonType game_character) //i - 
	: _name( name )
	, _character( game_character )
	, _assocGameToken( nullptr )
	, _wasMovedToRoomOutOfTurn( false )
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
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Player::addCardToHand(
	const Card* card) //i - card to join hand
{
	_hand.insert( card );

} //end routine addCardToHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
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
