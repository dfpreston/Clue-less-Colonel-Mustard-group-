////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file NotebookEntry.h
/// \brief 
///
/// \date   30 Mar 2019  1129
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef NotebookEntry_h
#define NotebookEntry_h

#include "Card.h"

#include "CluelessEnums.h"	//for ElementType use

#include <set>		//for std::set use

//forward declarations
class Player;


struct NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief default constructor
	NotebookEntry()
		: _card( nullptr )
		, _ownerCharacter( clueless::UNKNOWN_PERSON )
	{
	}

public:
	/// \brief Extended constructor
	NotebookEntry(const Card* const card, clueless::PersonType ownerCharacter)
		: _card( card )
		, _ownerCharacter( ownerCharacter )
	{
		//not object owner for any referenced data members
	}

	/// \brief destructor
	virtual ~NotebookEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	bool haveShownToPlayer(clueless::PersonType player) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	const Card* const _card; //not owned

	clueless::PersonType _ownerCharacter;
	std::set<clueless::PersonType> _playersShown; //player character type

}; //end struct NotebookEntry defn


struct PersonNotebookEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	PersonNotebookEntry()
		: NotebookEntry(nullptr, clueless::UNKNOWN_PERSON) //card, card owner's character
		, _person( clueless::UNKNOWN_PERSON )
	{
	}

public:
	PersonNotebookEntry(const Card* const card, clueless::PersonType ownerCharacter)
		: NotebookEntry(card, ownerCharacter)
		, _person( ((const PersonCard*)card)->_person )
	{
	}

	virtual ~PersonNotebookEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::PersonType _person;

}; //end struct PersonNotebookEntry defn


struct WeaponNotebookEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	WeaponNotebookEntry()
		: NotebookEntry(nullptr, clueless::UNKNOWN_PERSON) //card, card owner's character
		, _weapon( clueless::UNKNOWN_WEAPON )
	{
	}

public:
	WeaponNotebookEntry(const Card* const card, clueless::PersonType ownerCharacter)
		: NotebookEntry(card, ownerCharacter)
		, _weapon( ((const WeaponCard*)card)->_weapon )
	{
	}

	virtual ~WeaponNotebookEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::WeaponType _weapon;

}; //end struct WeaponNotebookEntry defn


struct RoomNotebookEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	RoomNotebookEntry()
		: NotebookEntry(nullptr, clueless::UNKNOWN_PERSON) //card, card owner's character
		, _room( clueless::UNKNOWN_ROOM )
	{
	}

public:
	RoomNotebookEntry(const Card* const card, clueless::PersonType ownerCharacter)
		: NotebookEntry(card, ownerCharacter)
		, _room( ((const RoomCard*)card)->_room )
	{
	}

	virtual ~RoomNotebookEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::RoomType _room;

}; //end struct RoomNotebookEntry defn


#endif //NotebookEntry_h defn
