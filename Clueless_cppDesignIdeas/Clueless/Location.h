////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Location.h
/// \brief
///
/// \date   25 Feb 2019  1150
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Location_h
#define Location_h

#include <iostream>	//for std::cout use
#include <set>		//for std::set use
#include <sstream>	//for std::ostringstream use
#include <string>	//for std::string use


//forward declarations
struct GamePiece;
struct PersonPiece;
class Room;


class Location
{
	//--------------------------------------------------------------------------
	// Class-Scoped Enumerated Types
	//--------------------------------------------------------------------------
protected:
	enum LocationType
	{
		ROOM,
		HALLWAY,
		HOME //starting space

	}; //end enum LocationType defn

	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Location()
	{
	}

public:
	/// \brief Extended constructor
	Location( LocationType type )
		: _type( type )
	{
	}

	/// \brief Destructor
	virtual ~Location()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	std::string getName() const;

	bool isRoom() const;
	bool isHallway() const;
	bool isHome() const;

	virtual bool isOccupied() const = 0;
	virtual bool canAcceptAnotherOccupant() const = 0;
	virtual bool addOccupant(const GamePiece* occupant);
	virtual void recognizeOccupantLeft(const GamePiece* previousOccupant) = 0;

	virtual bool isAccusationAllowedHere() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	virtual std::ostringstream report() const;

	virtual std::set<Location*> getMoveOptions() const = 0;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	std::string _name;
	LocationType _type;

}; //end class Location defn


////////////////////////////////////////////////////////////////////////////////
class Hallway : public Location
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Hallway()
		: Location( HALLWAY )
		, _connectingRoom1( nullptr )
		, _connectingRoom2( nullptr )
		, _occupant( nullptr )
	{
	}

public:
	Hallway(Room* firstConnectingRoom, Room* secondConnectingRoom);

	/// \brief Destructor
	virtual ~Hallway()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	virtual bool isOccupied() const override;
	virtual bool canAcceptAnotherOccupant() const override;
	virtual bool addOccupant(const GamePiece* occupant) override;
	virtual void recognizeOccupantLeft(const GamePiece* previousOccupant) override;

	const GamePiece* getOccupant() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
public:
	virtual std::set<Location*> getMoveOptions() const override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
private:
	Room* _connectingRoom1;
	Room* _connectingRoom2;

	const PersonPiece* _occupant;

}; //end class Hallway defn


////////////////////////////////////////////////////////////////////////////////
class HomeLocation : public Location
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	HomeLocation()
		: Location( HOME )
		, _personToken( nullptr )
		, _adjacentHallway( nullptr )
		, _occupant( nullptr )
	{
	}

public:
	HomeLocation(PersonPiece* personToken, Hallway* adjacentHallway);

	/// \brief Destructor
	virtual ~HomeLocation()
	{
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	virtual bool isOccupied() const override;
	virtual bool canAcceptAnotherOccupant() const override;
	virtual void recognizeOccupantLeft(const GamePiece* previousOccupant) override;

	const PersonPiece* getOccupant() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	virtual std::set<Location*> getMoveOptions() const override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
private:
	PersonPiece* _personToken;
	Hallway* _adjacentHallway;

	const PersonPiece* _occupant;

}; //end class HomeLocation defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline std::string
Location::getName()
const
{
	return _name;

} //end routine getName()


////////////////////////////////////////////////////////////////////////////////
inline bool
Location::isRoom()
const
{
	return( ROOM == _type );

} //end routine isRoom()


////////////////////////////////////////////////////////////////////////////////
inline bool
Location::isHallway()
const
{
	return( HALLWAY == _type );

} //end routine isHallway()


////////////////////////////////////////////////////////////////////////////////
inline bool
Location::isHome()
const
{
	return( HOME == _type );

} //end routine isHome()


////////////////////////////////////////////////////////////////////////////////
inline bool
Location::isAccusationAllowedHere()
const
{
	return false; //probably not

} //end routine isAccusationAllowedHere()


#endif //Location_h
