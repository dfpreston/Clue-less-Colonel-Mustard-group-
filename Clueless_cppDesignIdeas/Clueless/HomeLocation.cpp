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
	: _personToken( person_token )
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
	return( nullptr == _occupant );

} //end routine isOccupied()


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
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
HomeLocation::recognizeOccupantLeft()
{
	_occupant = nullptr;

} //end routine recognizeOccupantLeft()
