////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Card.h
/// \brief
///
/// \date   26 Feb 2019  1223
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef SolutionCardSet_h
#define SolutionCardSet_h

#include "Card.h"

#include <sstream>		//for std::ostringstream use


//forward declarations
// - none

struct SolutionCardSet
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	SolutionCardSet()
		: _person( clueless::UNKNOWN_PERSON )
		, _weapon( clueless::UNKNOWN_WEAPON )
		, _room( clueless::UNKNOWN_ROOM )
	{
	}

public:
	SolutionCardSet(const PersonCard& person, const WeaponCard& weapon, const RoomCard& room)
		: _person( person )
		, _weapon( weapon )
		, _room( room )
	{
	}

	//--------------------------------------------------------------------------
	// Overloaded Operators
	//--------------------------------------------------------------------------
	virtual bool
	operator==(const SolutionCardSet& other)
	const
	{
		return(
			(_person == other._person) &&
			(_weapon == other._weapon) &&
			(_room   == other._room) );
	}

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	virtual std::ostringstream
	report()
	const
	{
		std::ostringstream report;
		report
			<< _person.getName()
			<< " with the " << _weapon.getName()
			<< " in the " << _room.getName();

		return report;
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	PersonCard _person;
	WeaponCard _weapon;
	RoomCard   _room;

}; //end struct SolutionCardSet defn


#endif //SolutionCardSet_h defn
