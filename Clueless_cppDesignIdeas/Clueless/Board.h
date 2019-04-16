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

#include <list>		//for std::list use
#include <map>		//for std::map use
#include <set>		//for std::set use
#include <string>	//for std::string use
#include <tuple>	//for std::pair use


//forward declarations
struct Card;
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

	const Location* getDistanceToRoom(
		const Location* currentLocation,
		const Room* destinationRoom,
		size_t& distance) const;

	Location* fetchDistanceToRoom(
		const Location* startingPoint,
		clueless::RoomType destination,
		size_t& distance);


	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	std::ostringstream report() const;

	void recognizePlayerCharacterAssignments(std::list<Player*>* allPlayers);
	void buildRoomRelationshipsWithCards(const std::set<Card*> roomCards);

	//std::set<Location*> getMoveOptionsFrom(const Location* const startingPoint) const;
	bool movePlayerTo(Player* const player, Location* const destination);
	bool movePersonTokenToRoom(clueless::PersonType character, clueless::RoomType);
	bool moveWeaponTokenToRoom(clueless::WeaponType weapon, clueless::RoomType);

	//--------------------------------------------------------------------------
	// Accessors and Mutators - Protected Scope
	//--------------------------------------------------------------------------
protected:
	PersonPiece* fetchPersonToken(clueless::PersonType person) const;
	WeaponPiece* fetchWeaponToken(clueless::WeaponType weapon) const;
	Room* fetchRoom(clueless::RoomType) const;

protected:
	void randomlyDistributeWeapons(); //uniquely amongst rooms
	void assignCharacterToPlayer(Player* player, clueless::PersonType character);

private:
	//locations
	void createRooms();
	void createConnectingHallways();

	//game pieces
	void createPersonTokens();
	void createWeaponTokens();

public: /// \resolve Should scope be private? -- 07 Mar 2019, mem
	Location* const chooseLocation(std::set<Location*>* rooms) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	std::set<Location*> _rooms;
	std::set<Location*> _hallways;
	std::set<Location*> _personHomes;

	std::map<clueless::PersonType, PersonPiece*> _personTokens;
	std::map<clueless::WeaponType, WeaponPiece*> _weaponTokens;

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

	//location to room distance chart with next location on shortest path
	std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>> _roomDistanceChart;

	void addDataToDistanceChart(
		const Location* startingLocation,
		const Room* destination,
		size_t distanceInNumberOfMoves,
		Location* nextStepShortestPath);

	Location* fetchDistanceToRoom(
		const Location* startingPoint,
		const Room* destination,
		size_t& distance);

	void populateDistanceChart_studyKitchen();
	void populateDistanceChart_loungeConservatory();

	void populateDistanceChart_hallwayStudyHall(Hallway* hallway);
	void populateDistanceChart_hallwayHallLounge(Hallway* hallway);
	void populateDistanceChart_hallwayStudyLibrary(Hallway* hallway);
	void populateDistanceChart_hallwayHallBilliardRoom(Hallway* hallway);
	void populateDistanceChart_hallwayLoungeDiningRoom(Hallway* hallway);
	void populateDistanceChart_hallwayLibraryBilliardRoom(Hallway* hallway);
	void populateDistanceChart_hallwayBilliardRoomDiningRoom(Hallway* hallway);
	void populateDistanceChart_hallwayLibraryConservatory(Hallway* hallway);
	void populateDistanceChart_hallwayBilliardRoomBallroom(Hallway* hallway);
	void populateDistanceChart_hallwayDiningRoomKitchen(Hallway* hallway);
	void populateDistanceChart_hallwayConservatoryBallroom(Hallway* hallway);
	void populateDistanceChart_hallwayBallroomKitchen(Hallway* hallway);

}; //end class Board defn


#endif //Board_h
