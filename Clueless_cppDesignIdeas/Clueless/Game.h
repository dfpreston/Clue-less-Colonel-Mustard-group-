////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Game.h
/// \brief
///
/// \date   01 Mar 2019  1454
///
/// \note
/// - The Game owns all of the elements required for play including the Board,
///   CardDeck, and Players.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Game_h
#define Game_h

#include "Board.h"
#include "CardDeck.h"

#include <list>		//for std::list use

//forward declarations
class Player;


class Game
{
	//--------------------------------------------------------------------------
	// Class-Scoped Enumerated Types
	//--------------------------------------------------------------------------
	// - none

public:
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
	Game();
	virtual ~Game();

	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	bool hasWinner() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void setup(std::list<Player*>* allPlayers);

	void executePlayerTurn(Player* const player);

	void determinePlayerTurnOptions(Player* const player,
		const std::set<Location*>* const moveOptions,
		std::set<clueless::TurnOptionType>* turnOptions) const;

	void executePlayerChoice(Player* const player,
		clueless::TurnOptionType choice,
		std::set<Location*>* const move_options);

	const Card* requestCounterEvidenceToPlayerSuggestion(
		const Player* suggestor,
		const SolutionCardSet* suggestion,
		clueless::PersonType& opponentWithCounterEvidence) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	Board _board;
	CardDeck _cards;

	std::list<Player*> _players;
	Player* _winner;
	size_t _numFalseAccusers;

}; //end class Game defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline bool
Game::hasWinner()
const
{
	return( nullptr != _winner );

} //end routine hasWinner()


#endif //Game_h defn
