////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Hallway.cpp
/// \brief
///
/// \date   28 Feb 2019  1355
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Location.h"

#include "GamePiece.h"	//for GamePiece, PersonPiece use

#include <stdexcept>	//for std::logic_error use
#include <sstream>		//for std::ostringstream use

//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param PersonPiece: person token who starts in location
/// \param Hallway: hallway into which character enters from home
/// \return None
/// \throw
/// - INSUFFICIENT_DATA when person token object dne
/// - INSUFFICIENT_DATA when adjacent hallway object dne
/// \note None
////////////////////////////////////////////////////////////////////////////////
HomeLocation::HomeLocation(
	PersonPiece* person_token, //i - person token who starts in location
	Hallway* adjacent_hallway) //i - hallway into which character enters from home
	: Location( HOME )
	, _personToken( person_token )
	, _adjacentHallway( adjacent_hallway )
	, _occupant( person_token )
{
	if( nullptr == person_token)
	{
		std::ostringstream msg;
		msg << "HomeLocation extended constructor\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  need to specify existing person token";
		throw std::logic_error( msg.str() );
	}

	if( nullptr == adjacent_hallway)
	{
		std::ostringstream msg;
		msg << "HomeLocation extended constructor\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  need to specify existing adjacent hallway";
		throw std::logic_error( msg.str() );
	}

	std::ostringstream name;
	name << person_token->getName() << " start location";
	_name = name.str();

	_personToken->_location = this;

} //end routine extended constructor


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether hallway is occupied.
/// \param None
/// \return bool: whether hallway is occupied
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
HomeLocation::isOccupied()
const
{
	return( nullptr != _occupant );

} //end routine isOccupied()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether can accept another occupant.
/// \param None
/// \return bool: whether can accept another occupant
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
HomeLocation::canAcceptAnotherOccupant()
const
{
	//except for inital board laydown, never accept occupant (even one who started here)
	return false; //can_accept_another;

} //end routine canAcceptAnotherOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns constant reference to starting place occupant (must be person
///  token not weapon).
/// \param None
/// \return PersonPiece: starting place occupant; null pointer if unoccupied
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const PersonPiece*
HomeLocation::getOccupant()
const
{
	return _occupant;

} //end routine getOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Empties starting place, clearing occupant reference.
/// \param GamePiece: previous occupant
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
HomeLocation::recognizeOccupantLeft(
	const GamePiece* prev_occupant) //i - person who left room
{
	_occupant = nullptr;

} //end routine recognizeOccupantLeft()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns collection of possible move destinations based on specified
///  starting point.
/// \param Location: starting point
/// \return set<Location*>: possible destinations
/// \throw
/// - INSUFFICIENT_DATA when missing connecting room reference.
/// \note
/// - Adjacent rooms accept an unlimited number of occupants.
////////////////////////////////////////////////////////////////////////////////
std::set<Location*>
HomeLocation::getMoveOptions()
const
{
	if( ! _adjacentHallway )
	{
		std::ostringstream msg;
		msg << "HomeLocation::getMoveOptions()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << " missing adjacent hallway reference";
		throw std::logic_error( msg.str() );
	}

	std::set<Location*> destinations;

	destinations.insert( _adjacentHallway );

	return destinations;

} //end routine getMoveOptions()
