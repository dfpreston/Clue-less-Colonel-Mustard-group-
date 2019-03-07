////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Room.h
/// \brief
///
/// \date   26 Feb 2019  1145
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Room_h
#define Room_h

#include "Location.h"

#include "CluelessEnums.h"		//for RoomType use


//forward declarations
struct GamePiece;
class Hallway;
struct RoomCard;


class Room : public Location
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Room()
		: Location( ROOM )
		, _type( clueless::UNKNOWN_ROOM )
		, _roomAccessedBySecretPassage( nullptr )
		, _assocCard( nullptr )
	{
	}

public:
	/// \brief Extended constructor
	Room(clueless::RoomType type)
		: Location( ROOM )
		, _type(type)
		, _roomAccessedBySecretPassage( nullptr )
		, _assocCard( nullptr )
	{
		_name = clueless::translateRoomTypeToText( _type );
	}

	/// \brief Destructor
	virtual ~Room()
	{
		_occupants.clear(); //not responsible for deleting occupant objects

		_adjacentHallways.clear(); //do not delete hallways owned by board

		_roomAccessedBySecretPassage = nullptr;
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	virtual bool isOccupied() const override;
	virtual bool canAcceptAnotherOccupant() const override;
	virtual bool addOccupant(const GamePiece* occupant) override;
	virtual void recognizeOccupantLeft(const GamePiece* previousOccupant) override;

	virtual bool isAccusationAllowedHere() const override;

	bool doesPieceResideInRoom(const GamePiece* occupant) const;

	void acceptAdjacentHallway(Hallway* adjacentHallway);

	bool hasSecretPassage() const;
	void createSecretPassageTo(Room* destination);

protected:
	bool areMoreThanTwoHallwaysExpected() const;
	bool areMoreThanThreeHallwaysExpected() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
public:
	virtual std::ostringstream report() const override;

	virtual std::set<Location*> getMoveOptions() const override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::RoomType _type;
	std::set<const GamePiece*> _occupants;

	std::set<Hallway*> _adjacentHallways;

	Room* _roomAccessedBySecretPassage;

	RoomCard* _assocCard;

}; //end class Room defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline bool
Room::isAccusationAllowedHere()
const
{
	return true;

} //end routine isAccusationAllowedHere()


#endif //Room_h defn
