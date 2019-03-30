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

//#include "Player.h"

#include "CluelessEnums.h"	//for ElementType use

#include <map>		//for std::map use
#include <set>		//for std::set use

//forward declarations
struct Card;
class Player;
struct SolutionCardSet;


class DetectiveNotebook
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
public:
	DetectiveNotebook();
	//DetectiveNotebook( Player* owner );

	virtual ~DetectiveNotebook();

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	void setOwner(Player* owner);

	NotebookEntry* fetchNotebookEntry(const Card* card) const;

	bool haveShownCardToPlayer(const Card* const card, const Player* player) const;
	bool haveShownCardToAnyPlayer(const Card* const card) const;
	void recordHaveShownCardToPlayer(const Card* const card, const Player* player);

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void recordCardInHand(const Card* const cardInHand);
	void recordCardShownByPlayer(const Card* const card, const Player* player);

protected:
	void addEntryForCard(const Card* const card, const Player* cardOwner);

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	Player* _owner;

	std::map<clueless::ElementType, std::set<NotebookEntry*> > _notebook;

}; //end class DetectiveNotebook defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
inline void
DetectiveNotebook::setOwner(
	Player* owner)
{
	_owner = owner;

} //end routine setOwner()


#endif //DetectiveNotebook_h defn
