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

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	Board _board;
	CardDeck _cards;

	std::list<Player*> _players;
	Player* _winner;

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
