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
#include "Player.h"

#include <sstream>			//for std::ostringstream use
#include <stdexcept>		//for std::logic_error use


//------------------------------------------------------------------------------
// Constructors / Destructor
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief default constructor
////////////////////////////////////////////////////////////////////////////////
DetectiveNotebook::DetectiveNotebook()
	: _owner( nullptr )
{
} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
////////////////////////////////////////////////////////////////////////////////
//DetectiveNotebook( Player* owner )
//	: _owner( owner )
//{
//} //end routine extended constructor


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


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
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
		entry_iter = book_iter->second.begin();

		clueless::PersonType entry_person( clueless::UNKNOWN_PERSON );
		clueless::PersonType card_person( clueless::UNKNOWN_PERSON );

		//while have not found entry  AND
		//      more entries of elmt type in book
		while( entry_iter != book_iter->second.end() )
		{
			entry_person = ((PersonEntry*)(*entry_iter))->_person;
			card_person = ((const PersonCard*)card)->_person;

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
	}
		break;

	case clueless::WEAPON:
	{
		book_iter = _notebook.find( clueless::WEAPON );
		entry_iter = book_iter->second.begin();

		clueless::WeaponType entry_wpn( clueless::UNKNOWN_WEAPON );
		clueless::WeaponType card_wpn( clueless::UNKNOWN_WEAPON );

		//while have not found entry  AND
		//      more entries of elmt type in book
		while( entry_iter != book_iter->second.end() )
		{
			entry_wpn = ((WeaponEntry*)(*entry_iter))->_weapon;
			card_wpn = ((const WeaponCard*)card)->_weapon;

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
	}
	break;

	case clueless::ROOM:
	{
		book_iter = _notebook.find( clueless::ROOM );
		entry_iter = book_iter->second.begin();

		clueless::RoomType entry_room( clueless::UNKNOWN_ROOM );
		clueless::RoomType card_room( clueless::UNKNOWN_ROOM );

		//while have not found entry  AND
		//      more entries of elmt type in book
		while( entry_iter != book_iter->second.end() )
		{
			entry_room = ((RoomEntry*)(*entry_iter))->_room;
			card_room = ((const RoomCard*)card)->_room;

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
/// \brief Determines whether have previously shown specified card to player.
/// \param string: name
/// \param Card: card of interest
/// \param Player: player of interest
/// \return bool: whether have shown card to player
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// - INSUFFICIENT_DATA when player does not exist.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
DetectiveNotebook::haveShownCardToPlayer(
	const Card* const card,     //i - card of interest
	const Player* const player) //i - player
const
{
	//if objects dne
	if( ! card || ! player )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::haveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card and player objects must exist";
		throw std::logic_error( msg.str().c_str() );
	}

	bool have_shown_card( false );
	const NotebookEntry* entry( fetchNotebookEntry(card) );

	if( entry )
	{
		have_shown_card = entry->haveShownToPlayer( player );
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
/// \param Player: player to whom card shown
/// \return None
/// \throw
/// - INSUFFICIENT_DATA when card does not exist.
/// - INSUFFICIENT_DATA when player does not exist.
/// - INCONSISTENT_DATA when card not owned by notebook owner.
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::recordHaveShownCardToPlayer(
	const Card* const card, //i - card
	const Player* player)   //i - player to whom card shown
{
	//if objects dne
	if( ! card || ! player )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::recordHaveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  card and player objects must exist";
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
	else if( entry->_owner != _owner )
	{
		std::ostringstream msg;
		msg << "DetectiveNotebook::recordHaveShownCardToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  cannot have shown card not in hand (" << card->getName() << ")";
		throw std::logic_error( msg.str().c_str() );
	}

	//indicate card shown to player
	entry->_playersShown.insert( player );

} //end routine recordHaveShownCardToPlayer()

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
	addEntryForCard(card, _owner);

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
//		addEntryForCard(*card_iter, _owner);
//	}
//
//} //end routine recordHand()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
/// \param Card: card
/// \param Player: player showing card
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::recordCardShownByPlayer(
	const Card* const card,
	const Player* player)
{
	//retrieve notebook entry
	NotebookEntry* const entry( fetchNotebookEntry(card) );

	if( ! entry ) //no previous entry
	{
		addEntryForCard(card, player); 
	}

} //end routine recordCardShownByPlayer()


////////////////////////////////////////////////////////////////////////////////
/// \brief Adds entry for card to notebook.
/// \param Card: card
/// \param Player: card owner
/// \return None
/// \throw None
/// \note
/// - Assumes upstream check for existance of entry.
////////////////////////////////////////////////////////////////////////////////
void
DetectiveNotebook::addEntryForCard(
	const Card* const card,   //i - card
	const Player* card_owner) //i - card's owner
{
	//build entry
	NotebookEntry* new_entry = new NotebookEntry(card, card_owner);

	//add entry to page for element type
	_notebook[card->_type].insert( new_entry );

} //end routine addEntryForCard()
