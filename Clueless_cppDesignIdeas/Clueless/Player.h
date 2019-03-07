////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Player.h
/// \brief
///
/// \date   01 Mar 2019  1200
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef Player_h
#define Player_h

#include "CluelessEnums.h"

#include <set>			//for std::set use
#include <sstream>		//for std::ostringstream use
#include <string>		//for std::string use


//forward declarations
struct Card;
struct PersonCard;
struct PersonPiece;
class Location;
struct SolutionCardSet;
struct WeaponCard;


class Player
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Player();

public:
	/// \brief Extended constructor
	Player(std::string name, clueless::PersonType gameCharacter);

	/// \brief Destructor
	virtual ~Player()
	{
		//clear hand (do not delete Card objects held by CardDeck)
		_hand.clear();
	}


	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	std::string getName() const;

	clueless::PersonType getCharacter() const;
	std::string getCharacterName() const;

	const Location* getLocation() const;
	std::string getLocationName() const;

	bool wasMovedToRoomOutOfTurn() const;
	void indicateMovedToRoomOutOfTurn();
	void clearMovedToRoomOutOfTurnIndicator();

	bool hasMadeFalseAccusation() const;

	std::ostringstream report() const;
	std::ostringstream reportHand() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void addCardToHand(const Card* card);

	SolutionCardSet buildAccusation(const PersonCard* suspect, const WeaponCard* weapon) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	std::string _name;

	clueless::PersonType _character;
	PersonPiece* _assocGameToken;

	std::set<const Card*> _hand; //holds card owned by CardDeck

protected:
	bool _wasMovedToRoomOutOfTurn;
	bool _hasMadeFalseAccusation;

}; //end class Player defn


//------------------------------------------------------------------------------
// Inlined Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
inline std::string
Player::getName()
const
{
	return _name;

} //end routine getName()


////////////////////////////////////////////////////////////////////////////////
inline clueless::PersonType
Player::getCharacter()
const
{
	return _character;

} //end routine getCharacterPersonType()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::wasMovedToRoomOutOfTurn()
const
{
	return _wasMovedToRoomOutOfTurn;

} //end routine wasMovedToRoomOutOfTurn()


inline void
Player::indicateMovedToRoomOutOfTurn()
{
	_wasMovedToRoomOutOfTurn = true;

} //end routine indicateMovedToRoomOutOfTurn()


inline void
Player::clearMovedToRoomOutOfTurnIndicator()
{
	_wasMovedToRoomOutOfTurn = false;

} //end routine clearMovedToRoomOutOfTurnIndicator()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::hasMadeFalseAccusation()
const
{
	return _hasMadeFalseAccusation;

} //end routine hasMadeFalseAccusation()


#endif //Player_h
