////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Card.h
/// \brief
///
/// \date   26 Feb 2019  1200
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Card_h
#define Card_h


#include "CluelessEnums.h"	//for ElementType, PersonType, WeaponType, RoomType use

#include <sstream>		//for std::ostringstream use
#include <stdexcept>	//for std::logic_error use
#include <string>		//for std::string use


//forward declarations
// - none


struct Card
{
	//--------------------------------------------------------------------------
	// Class-Scoped Enumerated Types
	//--------------------------------------------------------------------------
	// - none

	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	/// \note private scope to force use of extended constructor by children
	Card()
		: _type( clueless::PERSON )
	{
	}

protected:
	/// \brief Copy constructor
	Card(const Card& source)
		: _type( source._type )
		, _name( source._name )
	{
	}

	/// \brief Extended constructor
	/// \param CardType: card type (person, weapon, room)
	/// \note protected scope to force construction of child class
	Card(clueless::ElementType type)
		: _type( type )
	{
	}

public:
	/// \brief Destructor
	virtual ~Card()
	{
	}

	//--------------------------------------------------------------------------
	// Overloaded Operators
	//--------------------------------------------------------------------------
	virtual bool operator==(const Card& other) const
	{
		return(
			(_type == other._type) &&
			(0 == _name.compare( other._name )) );
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	std::string getName() const;
	void rename(const std::string& newName);

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::ElementType _type;
	std::string _name;

}; //end struct Card defn


struct PersonCard
	: public Card
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	/// \brief Default constructor
	/// \note protected scope to force use of extended constructor
	PersonCard()
		: Card(clueless::PERSON )
		, _person( clueless::UNKNOWN_PERSON )
	{
	}

	/// \brief Copy constructor accepting base class
	PersonCard(const Card& source)
		: Card( source ) //performs base class member copy
	{
		if(clueless::PERSON != source._type )
		{
			std::ostringstream msg;
			msg << "PersonCard() copy constructor\n"
				<< "  INCONSISTENT_DATA\n"
				<< "  cannot create PersonCard from card of type: "
				<< clueless::translateElementTypeToText( source._type );
			throw std::logic_error( msg.str() );
		}

		//copy local data values
		_person = (static_cast<PersonCard>(source))._person;
	}

public:
	/// \brief Extended constructor
	/// \param PersonType: person type
	PersonCard(clueless::PersonType type)
		: Card(clueless::PERSON )
		, _person( type )
	{
		_name = clueless::translatePersonTypeToText( type );
	}

	/// \brief Destructor
	virtual ~PersonCard()
	{
	}

	//--------------------------------------------------------------------------
	// Overloaded Operators
	//--------------------------------------------------------------------------
	virtual bool operator==(const Card& other) const override
	{
		if(clueless::PERSON != other._type ) //other not person
		{
			return false; //must be different
		}

		PersonCard other_person( other );
		return(	*this == other_person );
	}

	virtual bool operator==(const PersonCard& other) const
	{
		return( 
			(this->Card::operator==( other )) &&
			(_person == other._person) );
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::PersonType _person;

}; //end struct PersonCard defn


struct WeaponCard
	: public Card
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	/// \brief Default constructor
	/// \note protected scope to force use of extended constructor
	WeaponCard()
		: Card(clueless::WEAPON )
		, _weapon( clueless::UNKNOWN_WEAPON )
	{
	}

	/// \brief Copy constructor accepting base class
	WeaponCard(const Card& source)
		: Card( source ) //performs base class member copy
	{
		if(clueless::WEAPON != source._type )
		{
			std::ostringstream msg;
			msg << "WeaponCard() copy constructor\n"
				<< "  INCONSISTENT_DATA\n"
				<< "  cannot create WeaponCard from card of type: "
				<< clueless::translateElementTypeToText( source._type );
			throw std::logic_error( msg.str() );
		}

		//copy local data values
		_weapon = (static_cast<WeaponCard>(source))._weapon;
	}

public:
	/// \brief Extended constructor
	/// \param WeaponType: weapon type
	WeaponCard(clueless::WeaponType type)
		: Card(clueless::WEAPON )
		, _weapon( type )
	{
		_name = clueless::translateWeaponTypeToText( type );
	}

	/// \brief Destructor
	virtual ~WeaponCard()
	{
	}

	//--------------------------------------------------------------------------
	// Overloaded Operators
	//--------------------------------------------------------------------------
	virtual bool operator==(const Card& other) const override
	{
		if(clueless::WEAPON != other._type ) //other not weapon
		{
			return false; //must be different
		}

		WeaponCard other_wpn( other );
		return(	*this == other_wpn );
	}

	virtual bool operator==(const WeaponCard& other) const
	{
		return( 
			(this->Card::operator==( other )) &&
			(_weapon == other._weapon) );
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::WeaponType _weapon;

}; //end struct WeaponCard defn


struct RoomCard
	: public Card
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	/// \brief Default constructor
	/// \note protected scope to force use of extended constructor
	RoomCard()
		: Card(clueless::ROOM )
		, _room( clueless::UNKNOWN_ROOM )
	{
	}

	/// \brief Copy constructor accepting base class
	RoomCard(const Card& source)
		: Card( source ) //performs base class member copy
	{
		if(clueless::ROOM != source._type )
		{
			std::ostringstream msg;
			msg << "RoomCard() copy constructor\n"
				<< "  INCONSISTENT_DATA\n"
				<< "  cannot create RoomCard from card of type: "
				<< clueless::translateElementTypeToText( source._type );
			throw std::logic_error( msg.str() );
		}

		//copy local data values
		_room = (static_cast<RoomCard>(source))._room;
	}

public:
	/// \brief Extended constructor
	/// \param RoomType: room type
	RoomCard(clueless::RoomType type)
		: Card( clueless::ROOM )
		, _room( type )
	{
		_name = clueless::translateRoomTypeToText( type );
	}

	/// \brief Destructor
	virtual ~RoomCard()
	{
	}

	//--------------------------------------------------------------------------
	// Overloaded Operators
	//--------------------------------------------------------------------------
	virtual bool operator==(const Card& other) const override
	{
		if(clueless::ROOM != other._type ) //other not person
		{
			return false; //must be different
		}

		RoomCard other_room( other );
		return(	*this == other_room );
	}

	virtual bool operator==(const RoomCard& other) const
	{
		return( 
			(this->Card::operator==( other )) &&
			(_room == other._room) );
	}

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::RoomType _room;

}; //end struct RoomCard defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline std::string
Card::getName()
const
{
	return _name;

} //end routine getName()


////////////////////////////////////////////////////////////////////////////////
/// \brief Populates name attribute with specified new name.
/// \param string: new name
/// \return None
/// \note
/// - Currently, nothing prohibits renaming to default for or same name as
///   another card.
/// \throw  None
////////////////////////////////////////////////////////////////////////////////
inline void
Card::rename(
	const std::string& new_name) //i - new name for card
{
	_name = new_name;

} //end routine rename()


#endif //Card_h defn
