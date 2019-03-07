////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Location.cpp
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

#include <sstream>		//for std::ostringstream use
#include <stdexcept>	//for std::logic_error use


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
Location::report()
const
{
	std::ostringstream report;

	report << "\'" << getName() << "\'";

	return report;

} //end routine report()


////////////////////////////////////////////////////////////////////////////////
/// \brief Adds specified occupant to location, if allowed.
/// \param GamePiece: potential occupant
/// \return bool: whether occupant added to room
/// \throw
/// - INSUFFICIENT_DATA when occupant object does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
Location::addOccupant(
	const GamePiece* piece)
{
	if( ! piece )
	{
		std::ostringstream msg;
		msg << "Location::addOccupant()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << ": occupant object dne";
		throw std::logic_error( msg.str() );
	}
	else if( this == piece->_location )
	{
		//piece already occupant here
		std::ostringstream msg;
		msg << "Location::addOccupant()\n"
			<< "  LOGIC_ERROR\n"
			<< "  \'" << piece->_name << "\' already occupies " << getName()
			<< "; must move into a connecting space";
	}
	else if( ! canAcceptAnotherOccupant() )
	{
		std::ostringstream msg;
		msg << "Location::addOccupant()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  " << getName() << ": cannot accept another occupant";
		throw std::logic_error( msg.str() );
	}

	//cannot add occupant through base class object
	return false;

} //end routine addOccupant()
