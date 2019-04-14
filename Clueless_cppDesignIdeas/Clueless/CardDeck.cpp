////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file CardDeck.cpp
/// \brief
///
/// \date   26 Feb 2019  1205
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "CardDeck.h"

#include "Card.h"
#include "Player.h"
#include "SolutionCardSet.h"

#include "CluelessEnums.h"	//for PersonType, WeaponType, RoomType enum use
#include "mersenneTwister.h"

#include <math.h>			//for std::floor use
#include <iostream>			//for std::cout use
#include <stdexcept>		//for std::logic_error use

//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Default constructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
CardDeck::CardDeck()
	: _caseFile( nullptr )
{
	createPersonCards();
	createWeaponCards();
	createRoomCards();

	chooseCaseFileSet();

} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Destructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
CardDeck::~CardDeck()
{
	//delete cards
	std::set<Card*>::iterator pcard_iter( _personCards.begin() );
	Card* curr_pcard( nullptr );
	while( _personCards.end() != pcard_iter )
	{
		curr_pcard = *pcard_iter;
		_personCards.erase( pcard_iter );
		delete curr_pcard;

		pcard_iter = _personCards.begin();
	}

	std::set<Card*>::iterator wcard_iter( _weaponCards.begin() );
	Card* curr_wcard( nullptr );
	while( _weaponCards.end() != wcard_iter )
	{
		curr_wcard = *wcard_iter;
		_weaponCards.erase( wcard_iter );
		delete curr_wcard;

		wcard_iter = _weaponCards.begin();
	}

	std::set<Card*>::iterator rcard_iter( _roomCards.begin() );
	Card* curr_rcard( nullptr );
	while( _roomCards.end() != rcard_iter )
	{
		curr_rcard = *rcard_iter;
		_roomCards.erase( rcard_iter );
		delete curr_rcard;

		rcard_iter = _roomCards.begin();
	}

	//case file
	delete _caseFile;
	_caseFile = nullptr;

} //end routine destructor


//------------------------------------------------------------------------------
// Game Setup Methods
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::createPersonCards()
{
	using clueless::PersonType;

	_personCards.insert( new PersonCard(PersonType::MISS_SCARLET) );
	_personCards.insert( new PersonCard(PersonType::COLONEL_MUSTARD) );
	_personCards.insert( new PersonCard(PersonType::MRS_WHITE) );
	_personCards.insert( new PersonCard(PersonType::MR_GREEN) );
	_personCards.insert( new PersonCard(PersonType::MRS_PEACOCK) );
	_personCards.insert( new PersonCard(PersonType::PROFESSOR_PLUM) );

	std::set<Card*>::iterator card_iter( _personCards.begin() );
	while( _personCards.end() != card_iter )
	{
		//add reference to undealt card stack
		_undealtCards.insert( *card_iter );
		++card_iter;

	} //end while (more room cards)

} //end routine createPersonCards()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::createWeaponCards()
{
	using clueless::WeaponType;

	_weaponCards.insert( new WeaponCard(WeaponType::LEAD_PIPE) );
	_weaponCards.insert( new WeaponCard(WeaponType::KNIFE) );
	_weaponCards.insert( new WeaponCard(WeaponType::ROPE) );
	_weaponCards.insert( new WeaponCard(WeaponType::CANDLESTICK) );
	_weaponCards.insert( new WeaponCard(WeaponType::REVOLVER) );
	_weaponCards.insert( new WeaponCard(WeaponType::WRENCH) );

	std::set<Card*>::iterator card_iter( _weaponCards.begin() );
	while( _weaponCards.end() != card_iter )
	{
		//add reference to undealt card stack
		_undealtCards.insert( *card_iter );
		++card_iter;

	} //end while (more room cards)

} //end routine createWeaponCards()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::createRoomCards()
{
	using clueless::RoomType;

	_roomCards.insert( new RoomCard(RoomType::STUDY) );
	_roomCards.insert( new RoomCard(RoomType::HALL) );
	_roomCards.insert( new RoomCard(RoomType::LOUNGE) );
	_roomCards.insert( new RoomCard(RoomType::LIBRARY) );
	_roomCards.insert( new RoomCard(RoomType::BILLIARD_ROOM) );
	_roomCards.insert( new RoomCard(RoomType::DINING_ROOM) );
	_roomCards.insert( new RoomCard(RoomType::CONSERVATORY) );
	_roomCards.insert( new RoomCard(RoomType::BALLROOM) );
	_roomCards.insert( new RoomCard(RoomType::KITCHEN) );

	std::set<Card*>::iterator card_iter( _roomCards.begin() );
	while( _roomCards.end() != card_iter )
	{
		//add reference to undealt card stack
		_undealtCards.insert( *card_iter );
		++card_iter;

	} //end while (more room cards)

} //end routine createRoomCards()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::chooseCaseFileSet()
{
	//choose one card from each category
	PersonCard* person( (PersonCard*)chooseCard(&_personCards) );
	WeaponCard* weapon( (WeaponCard*)chooseCard(&_weaponCards) );
	RoomCard* room( (RoomCard*)chooseCard(&_roomCards) );

	//copy into case file
	_caseFile = new SolutionCardSet(*person, *weapon, *room);

	std::cout << "Case File... " << _caseFile->report().str() << "\n\n";

	//remove from undealt collection
	removeCardFromUndealt( person );
	removeCardFromUndealt( weapon );
	removeCardFromUndealt( room );

} //end routine chooseCaseFileSet()


////////////////////////////////////////////////////////////////////////////////
/// \brief Chooses card from specified container with uniform random draw.
/// \param set<Card>: collection of cards
/// \return Card: choosen card
/// \throw
/// - INSUFFICIENT_DATA when empty collection of cards
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::setup(
	std::list<Player*>* players)
{
	std::list<Player*>::const_iterator player_iter( (*players).begin() );
	Card* chosen_card( nullptr );

	while( areAnyCardsUndealt() )
	{
		//deal card from undealt to receiving player (incl remove card from undealt)
		dealCard( *player_iter );

		//cycle to next player
		player_iter = determineNextPlayer(players, player_iter);

	} //end while (undealt cards)

} //end routine setup()


////////////////////////////////////////////////////////////////////////////////
/// \brief Chooses card from specified container with uniform random draw.
/// \param set<Card>: collection of cards
/// \return Card: choosen card
/// \throw
/// - INSUFFICIENT_DATA when empty collection of cards
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Card*
CardDeck::chooseCard(
	std::set<Card*>* cards) //i - cards to choose amongst
const
{
	Card* choice( nullptr );

	if( cards->empty() ) //no cards from which to choose
	{
		std::ostringstream msg;
		msg << "CardDeck::chooseCard()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  no cards from which to choose";
		throw std::logic_error( msg.str() );
	}
	else if( 1 == cards->size() )
	{
		//only one choice
		choice = *(cards->begin());
	}
	else //more than one card
	{
		//create position based on floor of random draw in [0, num cards)
		size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, (double)(cards->size())) )) );
	
		std::set<Card*>::const_iterator card_iter( cards->begin() );
		for(size_t pos_index(0);
			pos_index < zero_based_pos;
			++pos_index)
		{
			++card_iter;
		}

		choice = *card_iter;
	}

	return choice;

} //end routine chooseCard()


const Card*
CardDeck::chooseCard(
	std::set<const Card*>* cards) //i - cards to choose amongst
const
{
	const Card* choice( nullptr );

	if( cards->empty() ) //no cards from which to choose
	{
		std::ostringstream msg;
		msg << "CardDeck::chooseCard()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  no cards from which to choose";
		throw std::logic_error( msg.str() );
	}
	else if( 1 == cards->size() )
	{
		//only one choice
		choice = *(cards->begin());
	}
	else //more than one card
	{
		//create position based on floor of random draw in [0, num cards)
		size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, (double)(cards->size())) )) );

		std::set<const Card*>::const_iterator card_iter( cards->begin() );
		for(size_t pos_index(0);
			pos_index < zero_based_pos;
			++pos_index)
		{
			++card_iter;
		}

		choice = *card_iter;
	}

	return choice;

} //end routine chooseCard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Removes specified card from undealt cards collection.
/// \param Card: card to remove
/// \return None
/// \throw
/// - INCONSISTENT_DATA when specified card not found
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
CardDeck::removeCardFromUndealt(
	const Card* card) //i - card to remove
{
	std::set<const Card*>::iterator card_iter( _undealtCards.find(card) );
	
	if( _undealtCards.end() != card_iter ) //found card
	{
		_undealtCards.erase( card_iter );
	}
	else //card not found
	{
		std::ostringstream msg;
		msg << "CardDeck::removeCardFromUndealt()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card \'" << card->getName() << "\' not found in undealt set";
		throw std::logic_error( msg.str() );
	}

} //end routine removeCardFromUndealt()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const Card*
CardDeck::dealCard(
	Player* player) //io- player to whom card will be dealt
{
	const Card* card( nullptr );

	if( 0 == _undealtCards.size() )
	{
		std::ostringstream msg;
		msg << "CardDeck::dealCard()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  no undealt cards in deck";
		throw std::logic_error( msg.str() );
	}
	
	//uniform random draw of card
	card = chooseCard( &_undealtCards );
	player->addCardToHand( card );

	//remove reference from undealt card collection
	removeCardFromUndealt( card );

	return card; //dealt

} //end routine dealCard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines next player given collection of players and current player.
/// \param set<Player*>: all players
/// \param set<Player*>::iterator: current player
/// \return set<Player*>::iterator: next player
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::list<Player*>::const_iterator
CardDeck::determineNextPlayer(
	const std::list<Player*>* const players, //i - 
	std::list<Player*>::const_iterator curr_player_iter)
{
	std::list<Player*>::const_iterator next_player_iter( curr_player_iter );

	//increment iterator
	++next_player_iter;

	//if iterator represents end of collection
	if( (*players).end() == next_player_iter )
	{
		//cycle back to first player
		next_player_iter = (*players).begin();
	}
	
	return next_player_iter;

} //end routine determineNextPlayer()


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Reports contents of Case File (solution).
/// \param None
/// \return ostringstream: text report
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::ostringstream
CardDeck::reportCaseFile()
const
{
	return _caseFile->report();

} //end routine reportCaseFile()


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
CardDeck::doesAccusationMatchCaseFile(
	const SolutionCardSet& accusation) //i - accusation to compare with case file
const
{
	return( *_caseFile == accusation );

} //end routine doesAccusationMatchCaseFile()
