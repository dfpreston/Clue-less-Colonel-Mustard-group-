////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file CardDeck.h
/// \brief Responsible for creating, owning, and deleting all game cards
///  representing people, weapons, and rooms.
///
/// \date   26 Feb 2019  1202
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CardDeck_h
#define CardDeck_h

#include "Card.h"

#include <set>


//forward declarations
struct SolutionCardSet;


class CardDeck
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
public:
	CardDeck();

	virtual ~CardDeck();

	//--------------------------------------------------------------------------
	// Game Setup Methods
	//--------------------------------------------------------------------------
protected:
	void createPersonCards();
	void createWeaponCards();
	void createRoomCards();

	void chooseCaseFileSet();
	Card* dealCard();

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	bool areAnyCardsUndealt() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	bool doesAccusationMatchCaseFile(const SolutionCardSet& accusation) const;

protected:
	const Card* chooseCard(std::set<Card*>* cards) const;
	void removeCardFromUndealt(Card* card);

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	//primary storage (ownership) for all cards
	std::set<Card*> _personCards;
	std::set<Card*> _weaponCards;
	std::set<Card*> _roomCards;

	SolutionCardSet* _caseFile; //solution to crime

private:
	std::set<Card*> _undealtCards;

}; //end class CardDeck defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline bool
CardDeck::areAnyCardsUndealt()
const
{
	return( 0 < _undealtCards.size() );

} //end routine areAnyCardsUndealt()


#endif //CardDeck_h
