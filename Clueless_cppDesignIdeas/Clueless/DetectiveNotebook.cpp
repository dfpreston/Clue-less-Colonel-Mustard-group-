////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file DetectiveNotebook.cpp
/// \brief
///
/// \date   30 Mar 2019  0804
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "DetectiveNotebook.h"

#include "Card.h"
#include "Board.h"
#include "Location.h"
#include "Player.h"
#include "Room.h"
#include "SolutionCardSet.h"

#include "mersenneTwister.h"

#include <algorithm>		//for std::min use
#include <limits.h>			//for UINT_MAX use
#include <sstream>			//for std::ostringstream use
#include <stdexcept>		//for std::logic_error use


//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief default constructor
////////////////////////////////////////////////////////////////////////////////
DetectiveNotebook::DetectiveNotebook()
	: _ownerCharacter( clueless::UNKNOWN_PERSON )
	, _suspectedPerson( clueless::UNKNOWN_PERSON )
	, _suspectedWeapon( clueless::UNKNOWN_WEAPON )
	, _suspectedRoom( clueless::UNKNOWN_ROOM )
	, _nextRoomDestination( clueless::UNKNOWN_ROOM )
{
	initializeRoomsNeedingQuestioning();

} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param PersonType: owner character
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
DetectiveNotebook::DetectiveNotebook(
	clueless::PersonType owner_character) //i - owner's character
	: _ownerCharacter( owner_character )
	, _suspectedPerson( clueless::UNKNOWN_PERSON )
	, _suspectedWeapon( clueless::UNKNOWN_WEAPON )
	, _suspectedRoom( clueless::UNKNOWN_ROOM )
	, _nextRoomDestination( clueless::UNKNOWN_ROOM )
{
	initializeRoomsNeedingQuestioning();

} //end routine extended constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief destructor
////////////////////////////////////////////////////////////////////////////////
DetectiveNotebook::~DetectiveNotebook()
{
	//--------------------------------------------------------------------------
	// notebook clean up
	//--------------------------------------------------------------------------
	std::map<clueless::ElementType, std::set<NotebookEntry*>>::iterator page_iter( _notebook.begin() );
	std::set<NotebookEntry*>::iterator entry_iter( page_iter->second.begin() );

	//while more element type pages
	while( _notebook.end() != page_iter )
	{
		for(entry_iter  = page_iter->second.begin();
			entry_iter != page_iter->second.end();
			++entry_iter)
		{
			delete *entry_iter; //notebook entry object
		}
		page_iter->second.clear();

		++page_iter; //next page

	} //end while (more element type pages)

	_notebook.clear();

} //end routine destructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Allows for consistent initalization of rooms needing questioning.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::initializeRoomsNeedingQuestioning()
{
	size_t room_index( clueless::UNKNOWN_ROOM + 1 );

	//while more entries to consider
	for(room_index  = (clueless::RoomType)(clueless::UNKNOWN_ROOM + 1);
		room_index <= clueless::getNumRoomTypes();
		++room_index)
	{
		_roomsNeedingQuestioning.insert( (clueless::RoomType)room_index );

	} //end while (more entries)

} //end routine initializeRoomsNeedingQuestioning()


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Populates reference to room distance chart.
/// \param chart
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
//void
//DetectiveNotebook::setRoomDistanceChart(
//	const std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>>* chart)
//{
//	_roomDistanceChart = chart;
//
//} //end routine setRoomDistanceChart()

void
DetectiveNotebook::setBoard(
	Board* board) //i - board with room distance chart
{
	_assocBoard = board;

} //end routine setBoard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns reference to entry associated with card.  Null if no entry.
/// \param Card: card of interest
/// \return NotebookEntry: associated entry
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
NotebookEntry*
DetectiveNotebook::fetchNotebookEntry(
	const Card* card)
const
{
	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator book_iter( _notebook.begin() );
	std::set<NotebookEntry*>::const_iterator entry_iter;
	NotebookEntry* entry( nullptr );

	switch( card->_type )
	{
	case clueless::PERSON:
	{
		book_iter = _notebook.find( clueless::PERSON );
		if( _notebook.end() != book_iter )
		{
			entry_iter = book_iter->second.begin();

			clueless::PersonType entry_person( clueless::UNKNOWN_PERSON );
			clueless::PersonType card_person( ((const PersonCard*)card)->_person );

			//while have not found entry  AND
			//      more entries of elmt type in book
			while( ! entry &&
				(entry_iter != book_iter->second.end()) )
			{
				entry_person = ((PersonNotebookEntry*)(*entry_iter))->_person;

				//if entry matches card
				if( entry_person == card_person )
				{
					//found entry for card
					entry = *entry_iter;
				}
				else
				{
					++entry_iter; //next entry of elmt type
				}

			} //end while (have not found and more entries)
		} //end if (person entries in notebook)
	}
		break;

	case clueless::WEAPON:
	{
		book_iter = _notebook.find( clueless::WEAPON );
		if( _notebook.end() != book_iter )
		{
			entry_iter = book_iter->second.begin();

			clueless::WeaponType entry_wpn( clueless::UNKNOWN_WEAPON );
			clueless::WeaponType card_wpn( ((const WeaponCard*)card)->_weapon );

			//while have not found entry  AND
			//      more entries of elmt type in book
			while( ! entry &&
				(entry_iter != book_iter->second.end()) )
			{
				entry_wpn = ((WeaponNotebookEntry*)(*entry_iter))->_weapon;

				//if entry matches card
				if( entry_wpn == card_wpn )
				{
					//found entry for card
					entry = *entry_iter;
				}
				else
				{
					++entry_iter; //next entry of elmt type
				}

			} //end while (have not found and more entries)
		} //end if (weapon entries in notebook)
	}
	break;

	case clueless::ROOM:
	{
		book_iter = _notebook.find( clueless::ROOM );
		if( _notebook.end() != book_iter )
		{
			entry_iter = book_iter->second.begin();

			clueless::RoomType entry_room( clueless::UNKNOWN_ROOM );
			clueless::RoomType card_room( ((const RoomCard*)card)->_room );

			//while have not found entry  AND
			//      more entries of elmt type in book
			while( ! entry &&
				(entry_iter != book_iter->second.end()) )
			{
				entry_room = ((RoomNotebookEntry*)(*entry_iter))->_room;

				//if entry matches card
				if( entry_room == card_room )
				{
					//found entry for card
					entry = *entry_iter;
				}
				else
				{
					++entry_iter; //next entry of elmt type
				}

			} //end while (have not found and more entries)
		} //end if (room entries in notebook)
	}
	break;

	default:
		std::ostringstream msg;
		msg << "DetectiveNotebook::fetchNotebookEntry()\n"
			<< "  INCONSISTENT_DATA\n"
			<< "  unknown notebook entry type";
		throw std::logic_error( msg.str().c_str() );
	}

	return entry; //not found

} //end routine fetchNotebookEntry()


////////////////////////////////////////////////////////////////////////////////
/// \brief Take note that player has visited specified room.
/// \param RoomType: room visited
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::noteRoomQuestionHasBeenAnswered(
	clueless::RoomType room) //i - room visited
{
	std::set<clueless::RoomType>::iterator room_iter(
		_roomsNeedingQuestioning.find(room) );

	//if previously unvisited
	if( _roomsNeedingQuestioning.end() != room_iter )
	{
		_roomsNeedingQuestioning.erase(room_iter);
	}

} //end routine noteRoomQuestionHasBeenAnswered()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have previously shown specified card to player.
/// \param string: name
/// \param Card: card of interest
/// \param PersonType: player character of interest
/// \return bool: whether have shown card to player
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
DetectiveNotebook::haveCounterEvidenceForRoom(
	const Room* room) //i - room
const
{
	bool have_counter_evidence( false );

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator room_iter(
		_notebook.find(clueless::ROOM) );
	if( _notebook.end() != room_iter )
	{
		std::set<NotebookEntry*>::const_iterator entry_iter( room_iter->second.begin() );
		while( ! have_counter_evidence &&
			(room_iter->second.end() != entry_iter) )
		{
			if( room->_type == ((RoomNotebookEntry*)(*entry_iter))->_room )
			{
				have_counter_evidence = true;
			}
			else
			{
				++entry_iter;
			}
		} //end while (more entries to consider)

	} //end if (have room entries)

	return have_counter_evidence;

} //end routine haveCounterEvidenceForRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have previously shown specified card to player.
/// \param string: name
/// \param Card: card of interest
/// \param PersonType: player character of interest
/// \return bool: whether have shown card to player
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
DetectiveNotebook::haveShownCardToPlayer(
	const Card* const card, //i - card of interest
	clueless::PersonType player_character) //i - player
const
{
	//if objects dne
	if( ! card )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::haveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card object must exist";
		throw std::logic_error( msg.str().c_str() );
	}

	bool have_shown_card( false );
	const NotebookEntry* entry( fetchNotebookEntry(card) );

	if( entry )
	{
		have_shown_card = entry->haveShownToPlayer( player_character );
	}

	return have_shown_card;

} //end routine haveShownCardToPlayer()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have previously shown specified card to any player.
/// \param string: name
/// \param Card: card of interest
/// \return bool: whether have shown card to any player
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
DetectiveNotebook::haveShownCardToAnyPlayer(
	const Card* const card) //i - card of interest
const
{
	//if object dne
	if( ! card )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::haveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card objects must exist";
		throw std::logic_error( msg.str().c_str() );
	}

	bool have_shown_card( false );
	const NotebookEntry* entry( fetchNotebookEntry(card) );

	if( entry )
	{
		have_shown_card = ( ! entry->_playersShown.empty() );
	}

	return have_shown_card;

} //end routine haveShownCardToAnyPlayer()


////////////////////////////////////////////////////////////////////////////////
/// \brief Records that have shown card from hand to player.
/// \param Card: card of interest
/// \param PersonType: player's character to whom card shown
/// \return None
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// - INCONSISTENT_DATA when card not owned by notebook owner.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::recordHaveShownCardToPlayer(
	const Card* const card, //i - card
	clueless::PersonType player_character) //i - player to whom card shown
{
	//if objects dne
	if( ! card )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::recordHaveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card object must exist";
		throw std::logic_error( msg.str().c_str() );
	}

	//retrieve notebook entry
	NotebookEntry* const entry( fetchNotebookEntry(card) );

	if( ! entry )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::recordHaveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  entry for \'" << card->getName() << "\' card not found in notebook";
		throw std::logic_error( msg.str().c_str() );
	}
	else if( entry->_ownerCharacter != _ownerCharacter )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::recordHaveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  cannot have shown card not in hand (" << card->getName() << ")";
		throw std::logic_error( msg.str().c_str() );
	}

	//indicate card shown to player
	entry->_playersShown.insert( player_character );

} //end routine recordHaveShownCardToPlayer()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have enough suspect elements to build an accusation.
/// \param None
/// \return bool: whether have all elements to build accusation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
DetectiveNotebook::hasAllElementsForAccusation()
const
{
	return(
		(clueless::UNKNOWN_PERSON != _suspectedPerson) &&
		(clueless::UNKNOWN_WEAPON != _suspectedWeapon) &&
		(clueless::UNKNOWN_ROOM   != _suspectedRoom) );

} //end routine hasAllElementsForAccusation()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have enough suspect elements to build an accusation.
/// \param None
/// \return bool: whether have all elements to build accusation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
SolutionCardSet
DetectiveNotebook::getAccusation()
const
{
	return( SolutionCardSet(_suspectedPerson, _suspectedWeapon, _suspectedRoom) );

} //end routine getAccusation()


//--------------------------------------------------------------------------
// Additional Member Functions
//--------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param 
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::recordCardInHand(
	const Card* const card) //i - card in hand
{
	//add entry to notebook
	addEntryForCard(card, _ownerCharacter);

	if( clueless::ROOM == card->_type )
	{
		_roomsInHand.insert( ((const RoomCard* const)(card))->_room );
	}

} //end routine recordCardInHand()

//void
//DetectiveNotebook::recordHand(
//	const std::set<const Card*>* const hand)
//{
//	//for each card in hand,
//	std::set<const Card*>::const_iterator card_iter( hand->begin() );
//	for(card_iter  = hand->begin();
//		card_iter != hand->end();
//		++card_iter)
//	{
//		//add entry to notebook
//		addEntryForCard(*card_iter, _ownerCharacter);
//	}
//
//} //end routine recordHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param Card: card
/// \param PersonType: character for player showing card
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::recordCardShownByPlayer(
	const Card* const card, //i - card shown
	clueless::PersonType player_character) //i - card owner's character
{
	//retrieve notebook entry
	NotebookEntry* const entry( fetchNotebookEntry(card) );

	if( ! entry ) //no previous entry
	{
		//add entry
		addEntryForCard(card, player_character);
	}

} //end routine recordCardShownByPlayer()


////////////////////////////////////////////////////////////////////////////////
/// \brief Adds entry for card to notebook.
/// \param Card: card
/// \param Player: card owner's character
/// \return None
/// \throw None
/// \note
/// - Assumes upstream check for existance of entry.
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::addEntryForCard(
	const Card* const card, //i - card
	clueless::PersonType card_owner_character) //i - card owner's character
{
	clueless::ElementType card_type( card->_type );

	//build entry
	NotebookEntry* new_entry( nullptr );
	
	//depending on card type
	switch( card_type )
	{
	case clueless::PERSON:
		new_entry = new PersonNotebookEntry(card, card_owner_character);
		break;

	case clueless::WEAPON:
		new_entry = new WeaponNotebookEntry(card, card_owner_character);
		break;

	case clueless::ROOM:
		new_entry = new RoomNotebookEntry(card, card_owner_character);
		_roomsNeedingQuestioning.erase( ((RoomCard*)card)->_room );
		break;

	default:
		;
	} //end switch (on card type)

	//add entry to page for element type
	_notebook[card_type].insert( new_entry );

	//if only one unseen card for element type
	// i.e. number entries for element type one less than total cards for element
	if( (clueless::getNumElementSubtypes(card_type) - 1) == _notebook.find(card_type)->second.size() )
	{
		//unseen card is suspected element
		switch( card_type )
		{
		case clueless::PERSON:
			_suspectedPerson = determineMissingPerson();
			break;

		case clueless::WEAPON:
			_suspectedWeapon = determineMissingWeapon();
			break;

		case clueless::ROOM:
			_suspectedRoom = determineMissingRoom();
			_roomsNeedingQuestioning.clear();
			break;

		default:
			;
		} //end switch (on card type)

	} //end if (only one card not seen)

} //end routine addEntryForCard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Allows notes to account for lack of counter-evidence for card.
/// \param Card: suggested card without counter-evidence
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::notifyNoCounterEvidenceForCard(
	const Card* const card) //i - card without counter-evidence
{
	switch( card->_type )
	{
	case clueless::PERSON:
		_suspectedPerson = ((const PersonCard* const)card)->_person;
		break;

	case clueless::WEAPON:
		_suspectedWeapon = ((const WeaponCard* const)card)->_weapon;
		break;

	case clueless::ROOM:
		_suspectedRoom = ((const RoomCard* const)card)->_room;
		_roomsNeedingQuestioning.clear();
		break;

	default:
		;
	} //end switch (on card type)

} //end routine notifyNoCounterEvidenceForCard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Chooses person for suggestion.
/// \param None
/// \return PersonType: chosen person
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
clueless::PersonType
DetectiveNotebook::choosePersonForSuggestion()
const
{
	clueless::PersonType chosen_person( clueless::UNKNOWN_PERSON );

	//if has suspected person
	if( haveSuspectedPerson() )
	{
		std::set<clueless::PersonType> in_hand( retrievePeopleInHand() );

		//if has person in hand
		if( ! in_hand.empty() )
		{
			//choose person from hand
			chosen_person = *(in_hand.begin());
		}
		else //no person in hand
		{
			//use suspected person
			chosen_person = _suspectedPerson;
		}
	}
	else //no suspected person yet
	{
		//choose person not seen
		chosen_person = determineMissingPerson(); //first missing
	}

	return chosen_person;

} //end routine choosePersonForSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Chooses weapon for suggestion.
/// \param None
/// \return WeaponType: chosen weapon
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
clueless::WeaponType
DetectiveNotebook::chooseWeaponForSuggestion()
const
{
	clueless::WeaponType chosen_weapon( clueless::UNKNOWN_WEAPON );

	//if has suspected weapon
	if( haveSuspectedWeapon() )
	{
		std::set<clueless::WeaponType> in_hand( retrieveWeaponsInHand() );

		//if has weapon in hand
		if( ! in_hand.empty() )
		{
			//choose weapon from hand
			chosen_weapon = *(in_hand.begin());
		}
		else //no weapon in hand
		{
			//use suspected weapon
			chosen_weapon = _suspectedWeapon;
		}
	}
	else //no suspected weapon yet
	{
		//choose weapon not seen
		chosen_weapon = determineMissingWeapon(); //first missing
	}

	return chosen_weapon;

} //end routine chooseWeaponForSuggestion()


////////////////////////////////////////////////////////////////////////////////
/// \brief Decide which card to show opponent.
/// \param set<Card>: potential cards to show opponent
/// \return Card: card to show opponent
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const Card*
DetectiveNotebook::decideWhichCardToShowOpponent(
	const std::set<const Card*>* cards) //i - counter-evidence in hand
const
{
	const Card* choice( nullptr ); //no decision yet

	//if only found one piece of counter-evidence
	if( 1 == cards->size() )
	{
		//must show that card
		choice = *(cards->begin());
	}
	else
	{
		/// \todo favor card already shown to this opponent

		/// \todo next, prefer card already shown to greatest number of opponents

		//otherwise, randomly choose
		choice = randomlyChooseCard( cards );
	}

	return choice;

} //end routine decideWhichCardToShowOpponent()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines shortest path from starting point through next step room
///  to a destination room needing questioning.
/// \param 
/// \return size_t: distance (number of moves) to destination
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
size_t
DetectiveNotebook::determineShortestPathForDestinationNeedingQuestion(
	const Location* start_loc, //i - starting point
	Location* next_step) //i - desired next step location
const
{
	size_t shortest_distance( UINT_MAX );

	bool is_next_step_compare_needed( false );
	size_t addition_to_dist( 0 );

	const Room* start_room( nullptr );
	if( start_loc->isRoom() )
	{
		start_room = (const Room*)start_loc;
		is_next_step_compare_needed = true;
	}
	else if( next_step->isRoom() )
	{
		start_room = (const Room*)next_step;
		++addition_to_dist;
	}

	if( start_room )
	{
		size_t distance( UINT_MAX );
		Location* next_from_chart( nullptr );

		//for each room needing questioning
		std::set<clueless::RoomType>::const_iterator ques_iter( _roomsNeedingQuestioning.begin() );
		for(ques_iter  = _roomsNeedingQuestioning.begin();
			ques_iter != _roomsNeedingQuestioning.end();
			++ques_iter)
		{
			//if starting room is room needing questioning
			if( start_room->_type == (*ques_iter) )
			{
				//if currenting in destination room
				if( start_loc->isRoom() )
				{
					//assume need to leave room and reenter
					distance = 2;
				}
				else //in adjacent hallway
				{
					distance = 0;
				}
			}
			else //room needing questioning not current location
			{
				next_from_chart =
					_assocBoard->fetchDistanceToRoom(
						start_room,
						*ques_iter,
						distance);
			} //end else (room needing question not curr location)

			if( is_next_step_compare_needed )
			{
				if( next_step == next_from_chart )
				{
					shortest_distance = std::min(shortest_distance, (distance + addition_to_dist));
				}
			}
			else //next step comparison baked in because queried with next as starting room
			{
				shortest_distance = std::min(shortest_distance, (distance + addition_to_dist));
			}
		} //end for (each room needing questioning)

	} //end if (found starting room)

	return shortest_distance;

} //end routine determineShortestPathForDestinationNeedingQuestion()


size_t
DetectiveNotebook::determineShortestPathForDestinationSuspectedOrInHand(
	const Location* start_loc, //i - starting point
	Location* next_step) //i - desired next step location
const
{
	size_t shortest_distance( UINT_MAX );

	//create combined collection of rooms in hand and suspected
	std::set<clueless::RoomType> roomsToConsider = _roomsInHand;
	roomsToConsider.insert( _suspectedRoom );

	bool is_next_step_compare_needed( false );
	size_t addition_to_dist( 0 );

	const Room* start_room( nullptr );
	if( start_loc->isRoom() )
	{
		start_room = (const Room*)start_loc;
		is_next_step_compare_needed = true;
	}
	else if( next_step->isRoom() )
	{
		start_room = (const Room*)next_step;
		++addition_to_dist;
	}

	if( start_room )
	{
		size_t distance( UINT_MAX );
		Location* next_from_chart( nullptr );

		//for each room needing questioning
		std::set<clueless::RoomType>::const_iterator room_iter( roomsToConsider.begin() );
		for(room_iter  = roomsToConsider.begin();
			room_iter != roomsToConsider.end();
			++room_iter)
		{
			//if starting room is room needing questioning
			if( start_room->_type == (*room_iter) )
			{
				//if currenting in destination room
				if( start_loc->isRoom() )
				{
					//assume need to leave room and reenter
					distance = 2;
				}
				else //in adjacent hallway
				{
					distance = 0;
				}
			}
			else //room needing questioning not current location
			{
				next_from_chart =
					_assocBoard->fetchDistanceToRoom(
						start_room,
						*room_iter,
						distance);
			} //end else (room needing question not curr location)

			if( is_next_step_compare_needed )
			{
				if( next_step == next_from_chart )
				{
					shortest_distance = std::min(shortest_distance, (distance + addition_to_dist));
				}
			}
			else //next step comparison baked in because queried with next as starting room
			{
				shortest_distance = std::min(shortest_distance, (distance + addition_to_dist));
			}
		} //end for (each room needing questioning)

	} //end if (found starting room)

	return shortest_distance;

} //end routine determineShortestPathForDestinationSuspectedOrInHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief Uniform randomly choice amongst specified cards.
/// \param set<Card>: cards to choose amongst
/// \return Card: choice
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const Card*
DetectiveNotebook::randomlyChooseCard(
	const std::set<const Card*>* options) //i - cards from which to choose
const
{
	const Card* choice( nullptr );

	if( 1 == options->size() ) //only one option
	{
		//choose it
		choice = *(options->begin());
	}
	else if( ! options->empty() ) //more than one option
	{
		//create position based on floor of random draw in [0, num choices)
		size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, options->size()) )) );

		std::set<const Card*>::const_iterator opt_iter( options->begin() );
		for(size_t pos_index(0);
			pos_index < zero_based_pos;
			++pos_index)
		{
			++opt_iter;
		}

		choice = *opt_iter;

	} //end if (at least one option)

	return choice;

} //end routine randomlyChooseCard()


////////////////////////////////////////////////////////////////////////////////
/// \brief Retrieves person types in hand.
/// \param None
/// \return set<PersonType>: people in hand
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::set<clueless::PersonType>
DetectiveNotebook::retrievePeopleInHand()
const
{
	std::set<clueless::PersonType> in_hand;

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator page_iter(
		_notebook.find(clueless::PERSON) );

	//have entry for at least one person
	if( _notebook.end() != page_iter )
	{
		std::set<NotebookEntry*>::const_iterator entry_iter( page_iter->second.begin() );
		for(entry_iter  = page_iter->second.begin();
			entry_iter != page_iter->second.end();
			++entry_iter)
		{
			//if owned by notebook owner
			if( _ownerCharacter == (*entry_iter)->_ownerCharacter )
			{
				//person type in hand
				in_hand.insert( ((PersonNotebookEntry*)(*entry_iter))->_person );
			}

		} //end for (each person entry)

	} //end if (at least one person entry)

	return in_hand;

} //end routine retrievePeopleInHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief Retrieves weapon types in hand.
/// \param None
/// \return set<WeaponType>: weapons in hand
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::set<clueless::WeaponType>
DetectiveNotebook::retrieveWeaponsInHand()
const
{
	std::set<clueless::WeaponType> in_hand;

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator page_iter(
		_notebook.find(clueless::WEAPON) );

	//have entry for at least one weapon
	if( _notebook.end() != page_iter )
	{
		std::set<NotebookEntry*>::const_iterator entry_iter( page_iter->second.begin() );
		for(entry_iter  = page_iter->second.begin();
			entry_iter != page_iter->second.end();
			++entry_iter)
		{
			//if owned by notebook owner
			if( _ownerCharacter == (*entry_iter)->_ownerCharacter )
			{
				//person type in hand
				in_hand.insert( ((WeaponNotebookEntry*)(*entry_iter))->_weapon );
			}

		} //end for (each weapon entry)

	} //end if (at least one weapon entry)

	return in_hand;

} //end routine retrieveWeaponsInHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param
/// \return 
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
clueless::PersonType
DetectiveNotebook::determineMissingPerson()
const
{
	clueless::PersonType missing( clueless::UNKNOWN_PERSON );

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator page_iter( _notebook.find(clueless::PERSON) );
	if( _notebook.end() != page_iter )
	{
		std::set<NotebookEntry*>::const_iterator entry_iter( page_iter->second.begin() );
	
		const size_t NUM_PEOPLE( 6 ); // clueless::getNumPersonTypes() );
		bool is_found[NUM_PEOPLE] = {false, false, false, false, false, false};

		//while more entries to consider
		for(entry_iter  = page_iter->second.begin();
			entry_iter != page_iter->second.end();
			++entry_iter)
		{
			clueless::PersonType person( ((PersonCard*)((*entry_iter)->_card))->_person );

			is_found[person - 1] = true;

		} //end while (more entries)

		size_t is_found_index( 0 );
		while( clueless::UNKNOWN_PERSON == missing )
		{
			if( ! is_found[is_found_index] )
			{
				missing = clueless::PersonType( is_found_index + 1 );
			}
			else
			{
				++is_found_index;
			}
		} //end while (more found result to examine)

	}
	else //no person entries
	{
		missing = clueless::PersonType(clueless::UNKNOWN_PERSON + 1);
	}

	return missing;

} //end routine determineMissingPerson()


////////////////////////////////////////////////////////////////////////////////
clueless::WeaponType
DetectiveNotebook::determineMissingWeapon()
const
{
	clueless::WeaponType missing( clueless::UNKNOWN_WEAPON );

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator page_iter( _notebook.find(clueless::WEAPON) );
	if( _notebook.end() != page_iter )
	{
		std::set<NotebookEntry*>::const_iterator entry_iter( page_iter->second.begin() );

		const size_t NUM_WPNS( 6 ); // clueless::getNumPersonTypes() );
		bool is_found[NUM_WPNS] = {false, false, false, false, false, false};

		//while more entries to consider
		for(entry_iter  = page_iter->second.begin();
			entry_iter != page_iter->second.end();
			++entry_iter)
		{
			clueless::WeaponType weapon( ((WeaponCard*)((*entry_iter)->_card))->_weapon );

			is_found[weapon - 1] = true;

		} //end while (more entries)

		size_t is_found_index( 0 );
		while( clueless::UNKNOWN_WEAPON == missing )
		{
			if( ! is_found[is_found_index] )
			{
				missing = clueless::WeaponType( is_found_index + 1 );
			}
			else
			{
				++is_found_index;
			}
		} //end while (more found result to examine)

	}
	else //no weapon entries
	{
		missing = clueless::WeaponType(clueless::UNKNOWN_WEAPON + 1);
	}

	return missing;

} //end routine determineMissingWeapon()


////////////////////////////////////////////////////////////////////////////////
clueless::RoomType
DetectiveNotebook::determineMissingRoom()
const
{
	clueless::RoomType missing( clueless::UNKNOWN_ROOM );

	std::map<clueless::ElementType, std::set<NotebookEntry*>>::const_iterator page_iter( _notebook.find(clueless::PERSON) );
	std::set<NotebookEntry*>::const_iterator entry_iter( page_iter->second.begin() );

	const size_t NUM_ROOMS( 9 ); // clueless::getNumPersonTypes() );
	bool is_found[NUM_ROOMS] = {false, false, false, false, false, false, false, false, false};

	//while more entries to consider
	for(entry_iter  = page_iter->second.begin();
		entry_iter != page_iter->second.end();
		++entry_iter)
	{
		clueless::RoomType weapon( ((RoomCard*)((*entry_iter)->_card))->_room );

		is_found[weapon - 1] = true;

	} //end while (more entries)

	size_t is_found_index( 0 );
	while( clueless::UNKNOWN_ROOM == missing )
	{
		if( ! is_found[is_found_index] )
		{
			missing = clueless::RoomType( is_found_index + 1 );
		}
		else
		{
			++is_found_index;
		}
	} //end while (more found result to examine)

	return missing;

} //end routine determineMissingRoom()

