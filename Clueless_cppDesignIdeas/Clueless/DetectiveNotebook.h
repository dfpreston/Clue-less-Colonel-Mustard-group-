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
	NotebookEntry* fetchNotebookEntry(const Card* card) const;

	bool haveShownCardToPlayer(const Card* const card, clueless::PersonType playerCharacter) const;
	bool haveShownCardToAnyPlayer(const Card* const card) const;
	void recordHaveShownCardToPlayer(const Card* const card, clueless::PersonType playerCharacter);

	bool hasAllElementsForAccusation() const;
	SolutionCardSet getAccusation() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void recordCardInHand(const Card* const cardInHand);
	void recordCardShownByPlayer(const Card* const card, clueless::PersonType playerCharacter);

	clueless::PersonType choosePersonForSuggestion() const;
	clueless::WeaponType chooseWeaponForSuggestion() const;

	const Card* decideWhichCardToShowOpponent(const std::set<const Card*>* cards) const;

protected:
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

}; //end class DetectiveNotebook defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------


#endif //DetectiveNotebook_h defn
