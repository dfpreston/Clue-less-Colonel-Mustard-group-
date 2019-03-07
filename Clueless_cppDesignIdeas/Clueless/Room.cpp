////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Room.cpp
/// \brief
///
/// \date   25 Feb 2019  1216
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Room.h"

#include "GamePiece.h"

#include <stdexcept>	//for std::logic_error use
#include <sstream>		//for std::ostringstream use

//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether at least one piece resides in room.
/// \param None
/// \return bool: whether at least one piece occupies room
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::isOccupied()
const
{
	return( 0 < _occupants.size() );

} //end routine isOccupied()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether can accept another occupant.
/// \param None
/// \return bool: whether can accept another occupant
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::canAcceptAnotherOccupant()
const
{
	//unlimited occupancy allowed
	return true; //can_accept_another;

} //end routine canAcceptAnotherOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether specified piece resides in room.
/// \param GamePiece: piece of interest
/// \return bool: whether piece occupies room
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::doesPieceResideInRoom(
	const GamePiece* piece) //i - potential occupant
const
{
	std::set<const GamePiece*>::const_iterator piece_iter( _occupants.find( piece ) );

	//if found piece, is occupant
	return( _occupants.end() != piece_iter );

} //end routine doesPieceResideInRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Add specified occupant to collection.
/// \param GamePiece: occupant moving into room
/// \return bool: success installing piece as occupant
/// \throw
/// - LOGIC_ERROR when attempt to place piece already occupying room
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::addOccupant(
	const GamePiece* piece) //i - new occupant
{
	//prompt base class logic (primarily for error handling)
	Location::addOccupant( piece );

	//child class logic
	_occupants.insert( piece );
	return true; //occupant set

} //end routine addOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Recognizes that specified person left room and clears occupant reference.
/// \param GamePiece: previous occupant
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Room::recognizeOccupantLeft(
	const GamePiece* prev_occupant) //i - person who left room
{
	std::set<const GamePiece*>::iterator occupant( _occupants.find(prev_occupant) );

	if( occupant != _occupants.end() ) //found previous occupant
	{
		_occupants.erase( occupant );
	}

} //end routine recognizeOccupantLeft()


////////////////////////////////////////////////////////////////////////////////
/// \brief Populates a reference to adjacent hallway with specified hallway.
/// \param Hallway: adjacent hallway
/// \return None
/// \throw
/// - LOGIC_ERROR when already have references to two adjacent hallways.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Room::acceptAdjacentHallway(
	Hallway* hallway) //i - adjacent hallway
{
	bool can_accept_another_hallway( false );

	if( 2 > _adjacentHallways.size() )
	{
		//all rooms have at least 2 adjacent hallways
		can_accept_another_hallway = true;
	}
	else if(
		(2 == _adjacentHallways.size()) &&
		areMoreThanTwoHallwaysExpected() )
	{
		can_accept_another_hallway = true;
	}
	else if(
		(3 == _adjacentHallways.size()) &&
		areMoreThanThreeHallwaysExpected() )
	{
		can_accept_another_hallway = true;
	}

	if( can_accept_another_hallway )
	{
		_adjacentHallways.insert( hallway );
	}
	else //already have reference to max number hallways for room type
	{
		std::ostringstream msg;
		msg << "Room::acceptAdjacentHallway()\n"
			<< "  LOGIC_ERROR\n"
			<< "  " << getName() << " already has references to " << _adjacentHallways.size() << " hallways...\n";
		
		std::set<Hallway*>::const_iterator hallway_iter( _adjacentHallways.begin() );
		while( _adjacentHallways.end() != hallway_iter )
		{
			msg << "    " << (*hallway_iter)->getName() << "\n";
			++hallway_iter;

		} //end while (more hallways)

		throw std::logic_error( msg.str() );
	}

} //end routine acceptAdjacentHallway()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether room can access another via secret passage.
/// \param None
/// \return bool: whether has secret passage to another room
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::hasSecretPassage()
const
{
	return( nullptr != _roomAccessedBySecretPassage );

} //end routine hasSecretPassage()


////////////////////////////////////////////////////////////////////////////////
/// \brief Populates secret passage connection with specified room.
/// \param Room: destination room
/// \return void
/// \throw
/// - LOGIC_ERROR when have previous defined secret passage
/// - INSUFFICIENT_DATA when room not specified
/// - LOGIC_ERROR when attempt secret passage to self
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Room::createSecretPassageTo(
	Room* destination) //i - room accessible via secret passage
{
	if( _roomAccessedBySecretPassage )
	{
		std::ostringstream msg;
		msg << "Room::createSecretPassageTo()\n"
			<< "  LOGIC_ERROR\n"
			<< "  " << getName() << ": already has secret passage to \'"
			<< _roomAccessedBySecretPassage->getName() << "\'";
		throw std::logic_error( msg.str() );
	}
	else if( nullptr == destination )
	{
		std::ostringstream msg;
		msg << "Room::createSecretPassageTo()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << "\'s secret passage destination not specified";
		throw std::logic_error( msg.str() );
	}
	else if( destination == this )
	{
		std::ostringstream msg;
		msg << "Room::createSecretPassageTo()\n"
			<< "  LOGIC_ERROR\n"
			<< "  " << getName() << ": cannot create secret passage to self";
		throw std::logic_error( msg.str() );
	}

	_roomAccessedBySecretPassage = destination;

} //end routine createSecretPassageTo()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns whether room expects egress to more than two or three
///  hallways depending on room type.
/// \param none
/// \return bool: whether expects more than two or three hallways
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Room::areMoreThanTwoHallwaysExpected()
const
{
	bool does_expect_more_than_two( false );

	switch( _type )
	{
	case clueless::HALL:
	case clueless::LIBRARY:
	case clueless::BILLIARD_ROOM:
	case clueless::DINING_ROOM:
	case clueless::BALLROOM:
		does_expect_more_than_two = true;
		break;

	default:
		; //still does not expect more than two

	} //end switch (on room type)

	return does_expect_more_than_two;

} //end routine areMoreThanTwoHallwaysExpected()


bool
Room::areMoreThanThreeHallwaysExpected()
const
{
	bool does_expect_more_than_three( false );

	if( clueless::BILLIARD_ROOM == _type )
	{
		does_expect_more_than_three = true;
	}

	return does_expect_more_than_three;

} //end routine areMoreThanThreeHallwaysExpected()


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Reports class information.
/// \param None
/// \return ostringstream: report
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::ostringstream
Room::report()
const
{
	std::ostringstream report;

	report
		<< Location::report().str() << " (" << clueless::translateRoomTypeToText(_type) << "): "
		<< _occupants.size() << " occupants [";

	std::set<const GamePiece*>::const_iterator occupant_iter( _occupants.begin() );
	if( _occupants.end() != occupant_iter )
	{
		//first occupant
		report << (*occupant_iter)->getName();
		++occupant_iter;
	}

	while( _occupants.end() != occupant_iter )
	{
		//subsequent occupants
		report << ", " << (*occupant_iter)->getName();
		++occupant_iter;

	} //end while (more occupants)

	report << "]";

	//no need to report secret passage

	return report;

} //end routine report()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns collection of possible move destinations based on specified
///  starting point.
/// \param Location: starting point
/// \return set<Location*>: possible destinations
/// \throw None
/// \note
/// - Does not include adjacent hallway if occupied.
/// - Room connected via secret passage is assumed to have unlimited capacity.
////////////////////////////////////////////////////////////////////////////////
std::set<Location*>
Room::getMoveOptions()
const
{
	if( _adjacentHallways.empty() )
	{
		std::ostringstream msg;
		msg << "Hallway::getMoveOptions()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << " missing adjacent hallway references";
		throw std::logic_error( msg.str() );
	}

	std::set<Location*> destinations;

	std::set<Hallway*>::const_iterator hallway_iter( _adjacentHallways.begin() );
	while( _adjacentHallways.end() != hallway_iter )
	{
		if( ! (*hallway_iter)->isOccupied() ) //not occupied
		{
			destinations.insert( *hallway_iter );
		}

		++hallway_iter; //next hallway

	} //end while (more hallways)

	//if has secret passage
	if( _roomAccessedBySecretPassage )
	{
		//include room accessable by secret passage
		destinations.insert( _roomAccessedBySecretPassage );
	}

	return destinations;

} //end routine getMoveOptions()
