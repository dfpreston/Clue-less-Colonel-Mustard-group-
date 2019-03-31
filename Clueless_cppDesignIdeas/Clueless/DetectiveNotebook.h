////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file DetectiveNotebook.h
/// \brief 
///
/// \date   30 Mar 2019  0804
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DetectiveNotebook_h
#define DetectiveNotebook_h

#include "NotebookEntry.h"

#include "CluelessEnums.h"	//for ElementType use

#include <map>		//for std::map use
#include <set>		//for std::set use

//forward declarations
struct Card;
class Board;
class Hallway;
class Location;
class Room;
struct SolutionCardSet;


class DetectiveNotebook
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
public:
	DetectiveNotebook();
	DetectiveNotebook( clueless::PersonType ownerCharacter );

	virtual ~DetectiveNotebook();

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	//void setRoomDistanceChart(const std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>>* chart);
	void setBoard(Board* boardWithRoomDistanceChart);

	NotebookEntry* fetchNotebookEntry(const Card* card) const;

	bool isRoomInHand(clueless::RoomType room) const;

	bool doesRoomNeedQuestioning(clueless::RoomType room) const;
	bool hasRoomQuestionBeenAnswered(clueless::RoomType room) const;
	void noteRoomQuestionHasBeenAnswered(clueless::RoomType room);

	bool haveCounterEvidenceForRoom(const Room* room) const;

	bool haveShownCardToPlayer(const Card* const card, clueless::PersonType playerCharacter) const;
	bool haveShownCardToAnyPlayer(const Card* const card) const;
	void recordHaveShownCardToPlayer(const Card* const card, clueless::PersonType playerCharacter);

	bool haveSuspectedPerson() const;
	bool haveSuspectedWeapon() const;
	bool haveSuspectedRoom() const;

	bool hasAllElementsForAccusation() const;
	SolutionCardSet getAccusation() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void recordCardInHand(const Card* const cardInHand);
	void recordCardShownByPlayer(const Card* const card, clueless::PersonType playerCharacter);

	void notifyNoCounterEvidenceForCard(const Card* const card);

	clueless::PersonType choosePersonForSuggestion() const;
	clueless::WeaponType chooseWeaponForSuggestion() const;

	const Card* decideWhichCardToShowOpponent(const std::set<const Card*>* cards) const;

	size_t determineShortestPathForDestinationNeedingQuestion(
		const Location* startingPoint,
		Location* nextStep) const;
	size_t determineShortestPathForDestinationSuspectedOrInHand(
		const Location* startingPoint,
		Location* nextStep) const;

protected:
	void initializeRoomsNeedingQuestioning();

	void addEntryForCard(const Card* const card, clueless::PersonType cardOwnerCharacter);

	std::set<clueless::PersonType> retrievePeopleInHand() const;
//	std::set<clueless::PersonType> retrievePeopleNotSeen() const;

	std::set<clueless::WeaponType> retrieveWeaponsInHand() const;
//	std::set<clueless::WeaponType> retrieveWeaponsNotSeen() const;

	clueless::PersonType determineMissingPerson() const;
	clueless::WeaponType determineMissingWeapon() const;
	clueless::RoomType determineMissingRoom() const;

	const Card* randomlyChooseCard(const std::set<const Card*>* cards) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	clueless::PersonType _ownerCharacter;

	std::map<clueless::ElementType, std::set<NotebookEntry*> > _notebook;

	//building accusation...
	clueless::PersonType _suspectedPerson;
	clueless::WeaponType _suspectedWeapon;
	clueless::RoomType   _suspectedRoom;

	std::set<clueless::RoomType> _roomsInHand;
	std::set<clueless::RoomType> _roomsNeedingQuestioning;
	clueless::RoomType _nextRoomDestination;

	/// \todo remove cheat if pull chart out of board with methods
	//const std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>>* _roomDistanceChart; //from board
	Board* _assocBoard;

}; //end class DetectiveNotebook defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline bool
DetectiveNotebook::isRoomInHand(
	clueless::RoomType room) //i - room of interest
const
{
	//in hand if in collection
	return( _roomsInHand.end() != _roomsInHand.find(room) );

} //end routine isRoomInHand()


////////////////////////////////////////////////////////////////////////////////
inline bool
DetectiveNotebook::doesRoomNeedQuestioning(
	clueless::RoomType room)
const
{
	//still needs questioning if in collection
	return( _roomsNeedingQuestioning.end() != _roomsNeedingQuestioning.find(room) );

} //end routine doesRoomNeedQuestioning()


inline bool
DetectiveNotebook::hasRoomQuestionBeenAnswered(
	clueless::RoomType room) //i - room of interest
const
{
	//have visited if not in collection
	return( _roomsNeedingQuestioning.end() == _roomsNeedingQuestioning.find(room) );

} //end routine hasRoomQuestionBeenAnswered()


////////////////////////////////////////////////////////////////////////////////
inline bool
DetectiveNotebook::haveSuspectedPerson()
const
{
	return( clueless::UNKNOWN_PERSON != _suspectedPerson );

} //end routine haveSuspectedRoom()


inline bool
DetectiveNotebook::haveSuspectedWeapon()
const
{
	return( clueless::UNKNOWN_WEAPON != _suspectedWeapon );

} //end routine haveSuspectedRoom()


inline bool
DetectiveNotebook::haveSuspectedRoom()
const
{
	return( clueless::UNKNOWN_ROOM != _suspectedRoom );

} //end routine haveSuspectedRoom()


#endif //DetectiveNotebook_h defn
