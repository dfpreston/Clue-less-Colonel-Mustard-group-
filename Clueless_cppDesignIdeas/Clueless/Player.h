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

#include "DetectiveNotebook.h"

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
	Player(std::string name, clueless::PersonType gameCharacter, bool isGameCreator = false);

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

	bool isGameCreator() const;
	bool isFalseAccuser() const;
	bool isGameWinner() const;

	clueless::PersonType getCharacter() const;
	std::string getCharacterName() const;

	PersonPiece* fetchAssocGameToken() const;
	void acceptAssocCharacterToken(PersonPiece* personToken);

	const Location* getLocation() const;
	std::string getLocationName() const;

	bool isCardInHand(const Card* card) const;
	//bool isRoomInHand(const Room* room) const;

	bool wasMovedToRoomOutOfTurn() const;
	void indicateMovedToRoomOutOfTurn();
	//void clearMovedToRoomOutOfTurnIndicator();

	bool hasMovedDuringTurn() const;
	bool hasMadeSuggestionDuringTurn() const;

	void indicateHasMovedDuringTurn();
	void indicateHasMadeSuggestionDuringTurn();

	bool isReadyToMakeAccusation() const;
	void indicateIsGameWinner();
	void indicateHasMadeFalseAccusation();

	std::ostringstream report() const;
	std::ostringstream reportHand() const;

	//--------------------------------------------------------------------------
	// Additional Member Functions
	//--------------------------------------------------------------------------
	void addCardToHand(const Card* card);

	void prepareForNewTurn();
	clueless::TurnOptionType makeTurnChoice(std::set<clueless::TurnOptionType>* turnOptions) const;

	// Move
	Location* provideMovePreference(std::set<Location*>* moveOptions) const;

	// Make Suggestion
	SolutionCardSet buildSuggestion() const;
	void acceptCounterEvidence(
		const SolutionCardSet* suggestion,
		const Card* const card,
		clueless::PersonType opponentOfferingCounterEvidence);
	const Card* offerEvidenceCounterToSuggestion(const SolutionCardSet* suggestion, const Player* suggestorCharacter);

	// Make Accusation
	SolutionCardSet buildAccusation() const;

protected:
	std::set<const Card*> findCounterEvidenceInHand(const SolutionCardSet* suggestion) const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
protected:
	std::string _name;

	clueless::PersonType _character;
	PersonPiece* _assocGameToken;

	std::set<const Card*> _hand; //holds card owned by CardDeck

public:
	DetectiveNotebook _notebook;

protected:
	//elements impacting options within single turn
	bool _wasMovedToRoomOutOfTurn;

	bool _hasMovedDuringTurn; //at most one move permitted per turn
	bool _hasMadeSuggestionDuringTurn; //at most one suggestion permitted per turn

	//bool _isReadyToMakeAccusation;
	bool _hasMadeFalseAccusation;

private:
	bool _isGameCreator;
	bool _isGameWinner;

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
inline bool
Player::isGameCreator()
const
{
	return _isGameCreator;

} //end routine isGameCreator()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::isGameWinner()
const
{
	return _isGameWinner;

} //end routine isGameWinner()


inline void
Player::indicateIsGameWinner()
{
	_isGameWinner = true;

} //end routine indicateIsGameWinner()


////////////////////////////////////////////////////////////////////////////////
inline clueless::PersonType
Player::getCharacter()
const
{
	return _character;

} //end routine getCharacterPersonType()


inline PersonPiece*
Player::fetchAssocGameToken()
const
{
	return _assocGameToken;

} //end routine fetchAssocGameToken()


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


//inline void
//Player::clearMovedToRoomOutOfTurnIndicator()
//{
//	_wasMovedToRoomOutOfTurn = false;
//
//} //end routine clearMovedToRoomOutOfTurnIndicator()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::hasMovedDuringTurn()
const
{
	return _hasMovedDuringTurn;

} //end routine hasMovedDuringTurn()


inline void
Player::indicateHasMovedDuringTurn()
{
	_hasMovedDuringTurn = true;

} //end routine indicateHasMovedDuringTurn()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::hasMadeSuggestionDuringTurn()
const
{
	return _hasMadeSuggestionDuringTurn;

} //end routine hasMadeSuggestionDuringTurn()


inline void
Player::indicateHasMadeSuggestionDuringTurn()
{
	_hasMadeSuggestionDuringTurn = true;

} //end routine indicateHasMadeSuggestionDuringTurn()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::isReadyToMakeAccusation()
const
{
	return _notebook.hasAllElementsForAccusation();

} //end routine isReadyToMakeAccusation()


////////////////////////////////////////////////////////////////////////////////
inline bool
Player::isFalseAccuser()
const
{
	return _hasMadeFalseAccusation;

} //end routine isFalseAccuser()


inline void
Player::indicateHasMadeFalseAccusation()
{
	_hasMadeFalseAccusation = true;

} //end routine indicateHadMadeFalseAccusation()


#endif //Player_h
