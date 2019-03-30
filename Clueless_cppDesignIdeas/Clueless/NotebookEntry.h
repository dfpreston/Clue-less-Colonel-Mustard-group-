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

#include "CluelessEnums.h"	//for ElementType use

#include <set>		//for std::set use

//forward declarations
struct Card;
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
		, _owner( nullptr )
	{
	}

public:
	/// \brief Extended constructor
	NotebookEntry(const Card* const card, const Player* const owner)
		: _card( card )
		, _owner( owner )
	{
	}

	/// \brief destructor
	virtual ~NotebookEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	bool haveShownToPlayer(const Player* player) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	const Card* const _card; //not owned

	const Player* const _owner;
	std::set<const Player*> _playersShown;

}; //end struct NotebookEntry defn


struct PersonEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	PersonEntry()
		: NotebookEntry(nullptr, nullptr)
	{
	}

public:
	PersonEntry(const Card* const card, const Player* const owner)
		: NotebookEntry(card, owner)
	{
	}

	virtual ~PersonEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::PersonType _person;

}; //end struct PersonEntry defn


struct WeaponEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	WeaponEntry()
		: NotebookEntry(nullptr, nullptr)
	{
	}

public:
	WeaponEntry(const Card* const card, const Player* const owner)
		: NotebookEntry(card, owner)
	{
	}

	virtual ~WeaponEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::WeaponType _weapon;

}; //end struct WeaponEntry defn


struct RoomEntry : public NotebookEntry
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	RoomEntry()
		: NotebookEntry(nullptr, nullptr)
	{
	}

public:
	RoomEntry(const Card* const card, const Player* const owner)
		: NotebookEntry(card, owner)
	{
	}

	virtual ~RoomEntry()
	{
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::RoomType _room;

}; //end struct RoomEntry defn


#endif //NotebookEntry_h defn
