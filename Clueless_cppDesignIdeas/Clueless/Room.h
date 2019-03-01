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


class Room : public Location
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Room()
		: _type( clueless::UNKNOWN_ROOM )
		, _roomAccessedBySecretPassage( nullptr )
	{
	}

public:
	/// \brief Extended constructor
	Room(clueless::RoomType type)
		: _type(type)
		, _roomAccessedBySecretPassage( nullptr )
	{
		_name = clueless::translateRoomTypeToText( _type );
	}

	/// \brief Destructor
	virtual ~Room()
	{
		_occupants.clear(); //not responsible for deleting occupant objects

		_roomAccessedBySecretPassage = nullptr;
	}

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	virtual bool isOccupied() const override;
	bool doesPieceResideInRoom(const GamePiece* occupant) const;
	bool addOccupant(const GamePiece* occupant);
	void recognizeOccupantLeft(const GamePiece* previousOccupant);

	bool hasSecretPassage() const;
	void createSecretPassageTo(Room* destination);

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	virtual std::ostringstream report() const override;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
	clueless::RoomType _type;
	std::set<const GamePiece*> _occupants;

	Room* _roomAccessedBySecretPassage;

}; //end class Room defn


#endif //Room_h defn
