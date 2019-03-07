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

#include <list>		//for std::list use
#include <set>		//for std::set use


//forward declarations
class Player;
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
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	const std::set<Card*> getRoomCards() const;

protected:
	bool areAnyCardsUndealt() const;

	//--------------------------------------------------------------------------
	// Game Setup Methods
	//--------------------------------------------------------------------------
	void createPersonCards();
	void createWeaponCards();
	void createRoomCards();

	void chooseCaseFileSet();
	const Card* dealCard(Player* receivingPlayer);

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
public:
	void setup(std::list<Player*>* allPlayers);

	bool doesAccusationMatchCaseFile(const SolutionCardSet& accusation) const;

protected:
	Card* chooseCard(std::set<Card*>* cards) const;
	const Card* chooseCard(std::set<const Card*>* cards) const;

	void removeCardFromUndealt(const Card* card);

private:
	std::list<Player*>::const_iterator determineNextPlayer(
		const std::list<Player*>* const players,
		std::list<Player*>::const_iterator currPlayerIter);

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
	std::set<const Card*> _undealtCards;

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


////////////////////////////////////////////////////////////////////////////////
inline const std::set<Card*>
CardDeck::getRoomCards()
const
{
	return( _roomCards );

} //end routine getRoomCards()


#endif //CardDeck_h
