////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Hallway.cpp
/// \brief
///
/// \date   25 Feb 2019  1216
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Location.h"

#include "GamePiece.h"
#include "Room.h"

#include <stdexcept>	//for std::logic_error use
#include <sstream>		//for std::ostringstream use

//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param None
/// \return None
/// \throw
/// - INCONSISTENT_DATA when same room specified for first and second.
/// \note None
////////////////////////////////////////////////////////////////////////////////
Hallway::Hallway(
	Room* firstConnectingRoom,  //i - 
	Room* secondConnectingRoom) //i - 
	: Location( HALLWAY )
	, _occupant( nullptr )
	, _connectingRoom1( firstConnectingRoom )
	, _connectingRoom2( secondConnectingRoom )
{
	if( firstConnectingRoom == secondConnectingRoom )
	{
		std::ostringstream msg;
		msg << "Hallway extended constructor\n"
			<< "  INCONSISTENT_DATA\n"
			<< "  expect to connect two unique rooms";
		throw std::logic_error( msg.str() );
	}

	std::ostringstream name;
	name << "hallway " << _connectingRoom1->getName() << "-" << _connectingRoom2->getName();
	_name = name.str();

	firstConnectingRoom->acceptAdjacentHallway( this );
	secondConnectingRoom->acceptAdjacentHallway( this );

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
Hallway::isOccupied()
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
Hallway::canAcceptAnotherOccupant()
const
{
	bool can_accept_another( true );

	if( isOccupied() )
	{
		//only one occupant allowed
		can_accept_another = false;
	}

	return can_accept_another;

} //end routine canAcceptAnotherOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Adds specified occupant to location, if allowed.
/// \param GamePiece: potential occupant
/// \return bool: whether occupant added to room
/// \throw
/// - INSUFFICIENT_DATA when occupant object does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Hallway::addOccupant(
	const GamePiece* piece)
{
	//prompt base class logic (primarily for error handling)
	Location::addOccupant( piece );

	//child class logic
	_occupant = piece;
	return true; //occupant set

} //end routine addOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns constant reference to hallway occupant.
/// \param None
/// \return GamePiece: hallway occupant; null pointer if unoccupied
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const GamePiece*
Hallway::getOccupant()
const
{
	return _occupant;

} //end routine getOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param 
/// \return bool: success installing piece as occupant
/// \throw
/// - LOGIC_ERROR when piece already occupies hallway --> must move into room
/// - LOGIC_ERROR when attempt to place piece when already occupied
/// - LOGIC_ERROR when prompting clearing hallway... use explicit method instead
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Hallway::setOccupant(
	const GamePiece* piece) //i - new occupant
{
	if( isOccupied() )
	{
		if( _occupant == piece )
		{
			std::ostringstream msg;
			msg << "Hallway::setOccupant()\n"
				<< "  LOGIC_ERROR\n"
				<< "  \'" << _occupant->_name << "\' already occupies hallway; must move into an adjacent room";
			throw std::logic_error( msg.str() );
		}
		else //requesting new occupant
		{
			std::ostringstream msg;
			msg << "Hallway::setOccupant()\n"
				<< "  LOGIC_ERROR\n"
				<< "  hallway already occupied by \'" << _occupant->_name << "\'";
			throw std::logic_error( msg.str() );
		}

		return false; //new occupant not set -- will not reach due to throw
	}

	if( nullptr == piece )
	{
		std::ostringstream msg;
		msg << "Hallway::setOccupant()\n"
			<< "  LOGIC_ERROR\n"
			<< "  use recognizeOccupantLeft() to explicitly clear hallway";
		throw std::logic_error( msg.str() );
	}

	_occupant = piece;
	return true; //occupant set

} //end routine setOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Empties hallway, clearing occupant reference.
/// \param GamePiece: previous occupant
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Hallway::recognizeOccupantLeft(
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
Hallway::getMoveOptions()
const
{
	if( ! _connectingRoom1 || ! _connectingRoom2 )
	{
		std::ostringstream msg;
		msg << "Hallway::getMoveOptions()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << " missing connecting room reference";
		throw std::logic_error( msg.str() );
	}

	std::set<Location*> destinations;

	destinations.insert( _connectingRoom1 );
	destinations.insert( _connectingRoom2 );

	return destinations;

} //end routine getMoveOptions()
