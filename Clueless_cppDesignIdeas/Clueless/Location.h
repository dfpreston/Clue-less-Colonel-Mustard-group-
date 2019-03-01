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
	// Constructors / Destructor
	//--------------------------------------------------------------------------
public:
	/// \brief Default constructor
	Location()
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
	virtual bool isOccupied() const = 0;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	virtual std::ostringstream report() const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	std::string _name;

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
		: _connectingRoom1( nullptr )
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
	const GamePiece* getOccupant() const;
	bool setOccupant(const GamePiece* occupant);
	void recognizeOccupantLeft();

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
private:
	Room* _connectingRoom1;
	Room* _connectingRoom2;

	const GamePiece* _occupant;

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
		: _personToken( nullptr )
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
	const PersonPiece* getOccupant() const;
	void recognizeOccupantLeft();

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


#endif //Location_h
