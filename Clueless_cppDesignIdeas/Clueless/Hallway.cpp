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
	: _occupant( nullptr )
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
	return( nullptr == _occupant );

} //end routine isOccupied()


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
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Hallway::recognizeOccupantLeft()
{
	_occupant = nullptr;

} //end routine recognizeOccupantLeft()
