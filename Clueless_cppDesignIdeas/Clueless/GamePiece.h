////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file GamePiece.h
/// \brief
///
/// \date   25 Feb 2019  1203
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef GamePiece_h
#define GamePiece_h

#include "Player.h"

#include "CluelessEnums.h"		//for PersonType, WeaponType use

#include <sstream>	//for std::ostringstream use
#include <string>	//for std::string use


//forward declarations
class Location;


struct GamePiece
{
	//--------------------------------------------------------------------------
	// Class-Scoped Enumerated Types
	//--------------------------------------------------------------------------
protected:
	enum PieceType
	{
		PERSON_PIECE,
		WEAPON_PIECE

	}; //end enum PieceType defn

	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	GamePiece();

protected:
	/// \brief Extended construtor
	GamePiece(PieceType type);

public:
	/// \brief Destructor
	virtual ~GamePiece()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	bool isPersonPiece() const;
	bool isWeaponPiece() const;

	std::string getName() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
protected:
	virtual void populateName() = 0;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	std::string _name;

	Location* _location;

protected:
	PieceType _type;

}; //end struct GamePiece defn


////////////////////////////////////////////////////////////////////////////////
struct PersonPiece : public GamePiece
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	/// \brief Default constructor
	PersonPiece()
		: GamePiece( PERSON_PIECE )
		, _person( clueless::UNKNOWN_PERSON)
		, _assocPlayer( nullptr )
	{
	}

public:
	/// \brief Extended constructor
	PersonPiece(clueless::PersonType type)
		: GamePiece( GamePiece::PERSON_PIECE )
		, _person( type )
		, _assocPlayer( nullptr )
	{
		populateName();
	}


	/// \brief Destructor
	virtual ~PersonPiece()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	void setAssociatedPlayer(Player* player);

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
protected:
	virtual void populateName() override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	clueless::PersonType _person;

protected:
	Player* _assocPlayer;

}; //end struct PersonPiece defn


////////////////////////////////////////////////////////////////////////////////
struct WeaponPiece : public GamePiece
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
protected:
	/// \brief Default constructor
	WeaponPiece()
		: GamePiece( WEAPON_PIECE )
		, _weapon( clueless::UNKNOWN_WEAPON )
	{
	}

public:
	/// \brief Extended constructor
	WeaponPiece(clueless::WeaponType type)
		: GamePiece( GamePiece::WEAPON_PIECE )
		, _weapon( type )
	{
		populateName();
	}

	/// \brief Destructor
	virtual ~WeaponPiece()
	{
	}

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
protected:
	virtual void populateName() override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	clueless::WeaponType _weapon;

}; //end struct WeaponPiece defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline bool
GamePiece::isPersonPiece()
const
{
	return( GamePiece::PERSON_PIECE == _type );
}


inline bool
GamePiece::isWeaponPiece()
const
{
	return( GamePiece::WEAPON_PIECE == _type );
}


////////////////////////////////////////////////////////////////////////////////
inline std::string
GamePiece::getName()
const
{
	return _name;

} //end routine getName()


#endif //GamePiece_h
