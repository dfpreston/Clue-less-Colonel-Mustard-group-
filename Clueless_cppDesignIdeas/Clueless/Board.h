////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Board.h
/// \brief
///
/// \date   25 Feb 2019  1205
///
/// \note
/// - The Board owns all of the Location objects (Rooms, Hallways, Starting spots
///   for person tokens).  Therefore, it is responsible for Location object
///   deletion.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Board_h
#define Board_h

#include "Location.h"

#include "CluelessEnums.h"	//for PersonType, WeaponType use

#include <map>		//for std::map use
#include <string>	//for std::string use


//forward declarations
struct PersonPiece;
struct WeaponPiece;
class Player;


class Board
{
public:
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
	Board();

	virtual ~Board();

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	void listRooms() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	std::ostringstream report() const;

	void assignCharacterToPlayer(Player* player, clueless::PersonType character);

protected:
	void randomlyDistributeWeapons(); //uniquely amongst rooms

private:
	//locations
	void createRooms();
	void createConnectingHallways();

	//game pieces
	void createPersonTokens();
	void createWeaponTokens();

	Room* chooseRoom(std::set<Room*>* rooms) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	std::set<Room*> _rooms;
	std::set<Hallway*> _hallways;
	std::set<Location*> _personHomes;

	std::map<clueless::PersonType, PersonPiece*> _personTokens;
	std::map<clueless::WeaponType, GamePiece*> _weaponTokens;

private:
	/// \note for ease of access... use rooms collection for deletion
	Room* _study;
	Room* _hall;
	Room* _lounge;
	Room* _library;
	Room* _billiardRoom;
	Room* _diningRoom;
	Room* _conservatory;
	Room* _ballroom;
	Room* _kitchen;

	PersonPiece* _missScarlet;
	PersonPiece* _colonelMustard;
	PersonPiece* _mrsWhite;
	PersonPiece* _mrGreen;
	PersonPiece* _mrsPeacock;
	PersonPiece* _professorPlum;

	WeaponPiece* _leadPipe;
	WeaponPiece* _knife;
	WeaponPiece* _rope;
	WeaponPiece* _candlestick;
	WeaponPiece* _revolver;
	WeaponPiece* _wrench;

}; //end class Board defn


#endif //Board_h
