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
	std::set<const GamePiece*>::iterator occupant( _occupants.find(piece) );

	if( occupant != _occupants.end() ) //already an occupant
	{
		std::ostringstream msg;
		msg << "Room::addOccupant()\n"
			<< "  LOGIC_ERROR\n"
			<< "  \'" << piece->_name << "\' already occupies Room; must move into an adjacent hallway";

		if( hasSecretPassage() )
		{
			msg << " or secret passage";
		}

		throw std::logic_error( msg.str() );

		return false; //new occupant not set -- will not reach due to throw
	}

	_occupants.insert( piece );
	return true; //occupant set

} //end routine addOccupant()


////////////////////////////////////////////////////////////////////////////////
/// \brief Recognizes that specified person left room and clears occupant reference.
/// \param None
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

	std::cout << "created secret passage from \'" << getName() << "\' to \'" << _roomAccessedBySecretPassage->getName() << "\'\n";

} //end routine createSecretPassageTo()


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
