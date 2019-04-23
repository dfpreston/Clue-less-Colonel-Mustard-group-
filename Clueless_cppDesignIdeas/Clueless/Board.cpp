////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Board.cpp
/// \brief
///
/// \date   25 Feb 2019  1457
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Board.h"

#include "Card.h"			//for RoomCard use
#include "GamePiece.h"
#include "Location.h"
#include "Room.h"

#include "CluelessEnums.h"	//for RoomType use
#include "mersenneTwister.h"

#include <iostream>			//for std::cout use
#include <sstream>			//for std::ostringstream use
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
Board::Board()
	: _study( nullptr )
	, _hall( nullptr )
	, _lounge( nullptr )
	, _library( nullptr )
	, _billiardRoom( nullptr )
	, _diningRoom( nullptr )
	, _conservatory( nullptr )
	, _ballroom( nullptr )
	, _kitchen( nullptr )
	, _missScarlet( nullptr )
	, _colonelMustard( nullptr )
	, _mrsWhite( nullptr )
	, _mrGreen( nullptr )
	, _mrsPeacock( nullptr )
	, _professorPlum( nullptr )
	, _leadPipe( nullptr )
	, _knife( nullptr )
	, _rope( nullptr )
	, _candlestick( nullptr )
	, _revolver( nullptr )
	, _wrench( nullptr )
{
	createRooms();

	createPersonTokens();
	createConnectingHallways();

	createWeaponTokens();
	randomlyDistributeWeapons(); //amongst rooms

} //end routine constructor


////////////////////////////////////////////////////////////////////////////////
/// \brief Destructor
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Board::~Board()
{
	//delete all room objects
	std::set<Location*>::iterator room_iter( _rooms.begin() );
	Location* curr_room( nullptr );

	while( room_iter != _rooms.end() )
	{
		curr_room = *room_iter;
		_rooms.erase( room_iter );
		delete curr_room;

		room_iter = _rooms.begin();
	}

	//delete all person tokens
	std::map<clueless::PersonType, PersonPiece*>::iterator person_iter( _personTokens.begin() );
	PersonPiece* curr_person_token( nullptr );

	while( person_iter != _personTokens.end() )
	{
		curr_person_token = person_iter->second;
		_personTokens.erase( person_iter );
		delete curr_person_token;

		person_iter = _personTokens.begin();

	} //end while (more person tokens)

	//delete all weapon tokens
	std::map<clueless::WeaponType, WeaponPiece*>::iterator wpn_iter( _weaponTokens.begin() );
	WeaponPiece* curr_wpn_token( nullptr );

	while( wpn_iter != _weaponTokens.end() )
	{
		curr_wpn_token = wpn_iter->second;
		_weaponTokens.erase( wpn_iter );
		delete curr_wpn_token;

		wpn_iter = _weaponTokens.begin();

	} //end while (more weapon tokens)

} //end routine destructor


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Lists all rooms on board.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::listRooms()
const
{
	std::cout << _rooms.size() << " Rooms: ";

	std::set<Location*>::const_iterator room_iter( _rooms.begin() );
	if( 0 < _rooms.size() ) //at least one room
	{
		std::cout << (*room_iter)->getName();
		++room_iter;
	}

	while( _rooms.end() != room_iter )
	{
		std::cout << ", " << (*room_iter)->getName();
		++room_iter;

	} //end while (more rooms)

	std::cout << "\n";

} //end routine listRooms()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns person token of type specified.
/// \param PersonType: person type
/// \return PersonPiece: corresponding person token
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
PersonPiece*
Board::fetchPersonToken(
	clueless::PersonType person) //i - person type
const
{
	PersonPiece* token( nullptr );

	std::map<clueless::PersonType, PersonPiece*>::const_iterator token_iter(
		_personTokens.find(person) );

	if( _personTokens.end() != token_iter ) //found token
	{
		token = token_iter->second;
	}

	return token;

} //end routine fetchPersonToken()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns weapon token of type specified.
/// \param WeaponType: weapon type
/// \return WeaponPiece: corresponding weapon token
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
WeaponPiece*
Board::fetchWeaponToken(
	clueless::WeaponType wpn) //i - weapon type
const
{
	WeaponPiece* token( nullptr );

	std::map<clueless::WeaponType, WeaponPiece*>::const_iterator token_iter(
		_weaponTokens.find(wpn) );

	if( _weaponTokens.end() != token_iter ) //found token
	{
		token = token_iter->second;
	}

	return token;

} //end routine fetchWeaponToken()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns room of type specified.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Room*
Board::fetchRoom(
	clueless::RoomType type) //i - room type
const
{
	switch( type )
	{
	case clueless::STUDY:
		return _study;

	case clueless::HALL:
		return _hall;

	case clueless::LOUNGE:
		return _lounge;

	case clueless::LIBRARY:
		return _library;

	case clueless::BILLIARD_ROOM:
		return _billiardRoom;

	case clueless::DINING_ROOM:
		return _diningRoom;

	case clueless::CONSERVATORY:
		return _conservatory;

	case clueless::BALLROOM:
		return _ballroom;

	case clueless::KITCHEN:
		return _kitchen;

	default:
		;
		
	} //end switch (on room type)

	return nullptr;

} //end routine fetchRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Determines distance from location to room with next location on
///  shortest path.
/// \param Location: current location
/// \param Room: destination room
/// \param size_t: distance from location to room (in number of moves)
/// \return Location: next location on shortest path
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
const Location*
Board::getDistanceToRoom(
	const Location* curr_location, //i - current location
	const Room* destination, //i - destination room
	size_t& distance)
const
{
	Location* next_location_on_path( nullptr );

	std::pair<const Location*, const Room*> location_room;
	location_room = std::make_pair(curr_location, destination);

	std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>>::const_iterator
		chart_iter( _roomDistanceChart.find(location_room) );

	if( _roomDistanceChart.end() != chart_iter )
	{
		distance = chart_iter->second.first;
		next_location_on_path = chart_iter->second.second;
	}

	return next_location_on_path;

} //end routine getDistanceToRoom()


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Reports class information.
/// \param None
/// \return ostringstream: report
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
std::ostringstream
Board::report()
const
{
	std::ostringstream report;

	//std::set<Room*>::const_iterator room_iter( _rooms.begin() );
	//while( _rooms.end() != room_iter )
	//{
	//	//report room information
	//	report << (*room_iter)->report().str() << "\n";
	//	++room_iter;

	//} //end while (more rooms)

	/// \note prefer specific order to mimic board schematic
	report
		<< _study->report().str() << "\n"
		<< _hall->report().str() << "\n"
		<< _lounge->report().str() << "\n"
		<< _library->report().str() << "\n"
		<< _billiardRoom->report().str() << "\n"
		<< _diningRoom->report().str() << "\n"
		<< _conservatory->report().str() << "\n"
		<< _ballroom->report().str() << "\n"
		<< _kitchen->report().str() << "\n";

	return report;

} //end routine report()


////////////////////////////////////////////////////////////////////////////////
/// \brief Creates all rooms for board with default types.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::createRooms()
{
	using clueless::RoomType;

	_study = new Room( RoomType::STUDY );
	_rooms.insert( _study );

	_hall = new Room( RoomType::HALL );
	_rooms.insert( _hall );

	_lounge = new Room( RoomType::LOUNGE );
	_rooms.insert( _lounge );

	_library = new Room( RoomType::LIBRARY );
	_rooms.insert( _library );

	_billiardRoom = new Room( RoomType::BILLIARD_ROOM );
	_rooms.insert( _billiardRoom );

	_diningRoom = new Room( RoomType::DINING_ROOM );
	_rooms.insert( _diningRoom );

	_conservatory = new Room( RoomType::CONSERVATORY );
	_rooms.insert( _conservatory );

	_ballroom = new Room( RoomType::BALLROOM );
	_rooms.insert( _ballroom );

	_kitchen = new Room( RoomType::KITCHEN );
	_rooms.insert( _kitchen );

	//provide secret passage access between opposite corners (both directions)
	//study-kitchen
	_study->createSecretPassageTo( _kitchen );
	_kitchen->createSecretPassageTo( _study );

	populateDistanceChart_studyKitchen();

	//lounge-conservatory
	_lounge->createSecretPassageTo( _conservatory );
	_conservatory->createSecretPassageTo( _lounge );

	populateDistanceChart_loungeConservatory();

} //end routine createRooms()


////////////////////////////////////////////////////////////////////////////////
/// \brief Creates hallways, building connections between rooms.
/// \param None
/// \return None
/// \throw None
/// \note
/// - Assumes rooms and person tokens created prior to hallway creation.
////////////////////////////////////////////////////////////////////////////////
void
Board::createConnectingHallways()
{
	using clueless::PersonType;
	std::pair<const Location*, const Room*> location_room;
	std::pair<size_t, Location*> distance_nextLocation;

	// 1: study-hall
	Hallway* hallway = new Hallway(_study, _hall);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayStudyHall( hallway );

	// 2: hall-lounge + Miss Scarlet starting place
	hallway = new Hallway(_hall, _lounge);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_missScarlet, hallway) );

	populateDistanceChart_hallwayHallLounge( hallway );

	// 3: study-library
	hallway = new Hallway(_study, _library);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_professorPlum, hallway) );

	populateDistanceChart_hallwayStudyLibrary( hallway );

	// 4: hall-billiard room
	hallway = new Hallway(_hall, _billiardRoom);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayHallBilliardRoom( hallway );

	// 5: lounge-dining room
	hallway = new Hallway(_lounge, _diningRoom);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_colonelMustard, hallway) );

	populateDistanceChart_hallwayLoungeDiningRoom( hallway );

	// 6: library-billiard room
	hallway = new Hallway(_library, _billiardRoom);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayLibraryBilliardRoom( hallway );

	// 7: billiard room-dining room
	hallway = new Hallway(_billiardRoom, _diningRoom);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayBilliardRoomDiningRoom( hallway );

	// 8: library-conservatory
	hallway = new Hallway(_library, _conservatory);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrsPeacock, hallway) );

	populateDistanceChart_hallwayLibraryConservatory( hallway );

	// 9: billiard room-ballroom
	hallway = new Hallway(_billiardRoom, _ballroom);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayBilliardRoomBallroom( hallway );

	//10: dining room-kitchen
	hallway = new Hallway(_diningRoom, _kitchen);
	_hallways.insert( hallway );

	populateDistanceChart_hallwayDiningRoomKitchen( hallway );

	//11: conservatory-ballroom
	hallway = new Hallway(_conservatory, _ballroom);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrGreen, hallway) );

	populateDistanceChart_hallwayConservatoryBallroom( hallway );

	//12: ballroom-kitchen
	hallway = new Hallway(_ballroom, _kitchen);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrsWhite, hallway) );

	populateDistanceChart_hallwayBallroomKitchen( hallway );

} //end routine createConnectingHallways()


////////////////////////////////////////////////////////////////////////////////
/// \brief Creates all person tokens for board with default types / names.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::createPersonTokens()
{
	using clueless::PersonType;

	_missScarlet = new PersonPiece( PersonType::MISS_SCARLET );
	_personTokens[PersonType::MISS_SCARLET] = _missScarlet;

	_colonelMustard = new PersonPiece( PersonType::COLONEL_MUSTARD );
	_personTokens[PersonType::COLONEL_MUSTARD] = _colonelMustard;

	_mrsWhite = new PersonPiece( PersonType::MRS_WHITE );
	_personTokens[PersonType::MRS_WHITE] = _mrsWhite;

	_mrGreen = new PersonPiece( PersonType::MR_GREEN );
	_personTokens[PersonType::MR_GREEN] = _mrGreen;

	_mrsPeacock = new PersonPiece( PersonType::MRS_PEACOCK );
	_personTokens[PersonType::MRS_PEACOCK] = _mrsPeacock;

	_professorPlum = new PersonPiece( PersonType::PROFESSOR_PLUM );
	_personTokens[PersonType::PROFESSOR_PLUM] = _professorPlum;

} //end routine createPersonTokens()


////////////////////////////////////////////////////////////////////////////////
/// \brief Creates all weapons for board with default types / names.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::createWeaponTokens()
{
	using clueless::WeaponType;

	_leadPipe = new WeaponPiece( WeaponType::LEAD_PIPE );
	_weaponTokens[WeaponType::LEAD_PIPE] = _leadPipe;

	_knife = new WeaponPiece( WeaponType::KNIFE );
	_weaponTokens[WeaponType::KNIFE] = _knife;

	_rope = new WeaponPiece( WeaponType::ROPE );
	_weaponTokens[WeaponType::ROPE] = _rope;

	_candlestick = new WeaponPiece( WeaponType::CANDLESTICK );
	_weaponTokens[WeaponType::CANDLESTICK] = _candlestick;

	_revolver = new WeaponPiece( WeaponType::REVOLVER );
	_weaponTokens[WeaponType::REVOLVER] = _revolver;

	_wrench = new WeaponPiece( WeaponType::WRENCH );
	_weaponTokens[WeaponType::WRENCH] = _wrench;

} //end routine createWeaponTokens()


////////////////////////////////////////////////////////////////////////////////
/// \brief Distributes each weapon token to a unique room.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::randomlyDistributeWeapons()
{
	//local copy of rooms so can remove as place weapons
	std::set<Location*> rooms = _rooms;
	std::set<Location*>::iterator room_iter( rooms.begin() );
	Location* chosen_room( nullptr );
	
	std::map<clueless::WeaponType, WeaponPiece*>::iterator wpn_iter( _weaponTokens.begin() );
	WeaponPiece* wpn_token( nullptr );
	while( _weaponTokens.end() != wpn_iter )
	{
		chosen_room = chooseLocation(&rooms);
		wpn_token = wpn_iter->second;

		//update room occupants and game piece location
		chosen_room->addOccupant( wpn_token );
		wpn_token->_location = chosen_room;

		//remove room from list of options (so unique per weapon)
		room_iter = rooms.find( chosen_room );
		rooms.erase( room_iter );
		
		++wpn_iter; //next weapon
	}

} //end routine randomlyDistributeWeapons()


////////////////////////////////////////////////////////////////////////////////
/// \brief Chooses room from specified container with uniform random draw.
/// \param set<Room>: collection of rooms
/// \return Room: choosen room
/// \throw
/// - INSUFFICIENT_DATA when empty collection of rooms
/// \note  None
////////////////////////////////////////////////////////////////////////////////
Location* const
Board::chooseLocation(
	std::set<Location*>* rooms) //i - rooms to choose amongst
const
{
	Location* choice( nullptr );

	if( rooms->empty() ) //no rooms from which to choose
	{
		std::ostringstream msg;
		msg << "Board::chooseRoom()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  no rooms from which to choose";
		throw std::logic_error( msg.str() );
	}
	else if( 1 == rooms->size() )
	{
		//only one choice
		choice = *(rooms->begin());
	}
	else //more than one room
	{
		//create position based on floor of random draw in [0, num cards)
		size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, (double)(rooms->size())) )) );

		std::set<Location*>::const_iterator room_iter( rooms->begin() );
		for(size_t pos_index(0);
			pos_index < zero_based_pos;
			++pos_index)
		{
			++room_iter;
		}

		choice = *room_iter;
	}

	return choice;

} //end routine chooseRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Build associations between players and board characters.
/// \param set<Player*>: all players
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::recognizePlayerCharacterAssignments(
	std::list<Player*>* players) //i - all players
{
	std::list<Player*>::iterator player_iter( (*players).begin() );
	while( (*players).end() != player_iter )
	{
		//build relationship between board character and player
		assignCharacterToPlayer(*player_iter, (*player_iter)->getCharacter());

		//provide room distance chart reference to player's notebook
		(*player_iter)->_notebook.setBoard( this );

		++player_iter; //next player

	} //end while (more players)

} //end routine recognizePlayerCharacterAssignments()


////////////////////////////////////////////////////////////////////////////////
/// \brief Build associations between rooms and room cards.
/// \param set<Card*>: all room cards
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::buildRoomRelationshipsWithCards(
	const std::set<Card*> cards) //i - room cards
{
	std::set<Card*>::const_iterator card_iter( cards.begin() );
	RoomCard* room_card( nullptr );
	Room* room( nullptr );

	while( cards.end() != card_iter )
	{
		//hold card as room card
		room_card = (RoomCard*)(*card_iter);

		//find room for card's room type
		room = fetchRoom( room_card->_room );

		if( room )
		{
			room->_assocCard = room_card;
		}

		++card_iter; //next card

	} //end while (more cards)
	
} //end routine buildRoomRelationshipsWithCards()


////////////////////////////////////////////////////////////////////////////////
/// \brief Returns collection of possible move destinations based on specified
///  starting point.
/// \param Location: starting point
/// \return set<Location*>: possible destinations
/// \throw
/// - INSUFFICIENT_DATA when starting point object dne
/// \note  None
////////////////////////////////////////////////////////////////////////////////
//std::set<Location*>
//Board::getMoveOptionsFrom(
//	const Location* const startingPoint) //i - starting location
//const
//{
//	if( ! startingPoint )
//	{
//		std::ostringstream msg;
//		msg << "Board::getMoveOptionsFrom()\n"
//			<< "  INSUFFICIENT_DATA\n"
//			<< "  starting point obj dne";
//		throw std::logic_error( msg.str() );
//	}
//
//	std::cout << "STUB: add logic to Board::getMoveOptionsFrom(startingPoint)\n";
//
//} //end routine getMoveOptionsFrom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Moves player to specified location.
/// \param Location: starting point
/// \return set<Location*>: possible destinations
/// \throw
/// - INSUFFICIENT_DATA when player or location object does not exist.
/// - LOGIC_ERROR when destination cannot accept another occupant.
/// \note
/// - Assumes destination is valid.
////////////////////////////////////////////////////////////////////////////////
bool
Board::movePlayerTo(
	Player* const player,
	Location* const destination)
{
	if( ! player )
	{
		std::ostringstream msg;
		msg << "Board::movePlayerTo()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  player object dne";
		throw std::logic_error( msg.str() );
	}
	else if( ! destination )
	{
		std::ostringstream msg;
		msg << "Board::movePlayerTo()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  location object dne";
		throw std::logic_error( msg.str() );
	}

	//retrieve associated character token
	PersonPiece* player_token( player->fetchAssocGameToken() );
	Location* curr_location( player_token->_location );

	//if destination differs from current location  AND
	//   destination cannot accept another occupant
	if( (curr_location != destination) &&
		! destination->canAcceptAnotherOccupant() )
	{
		std::ostringstream msg;
		msg << "Board::movePlayerTo()\n"
			<< "  LOGIC_ERROR\n"
			<< "  location object dne";
		throw std::logic_error( msg.str() );
	}

	//update room occupants and game piece location
	destination->addOccupant( player_token );
	player_token->_location = destination;

	//vacate current location
	curr_location->recognizeOccupantLeft( player_token );

	return true; //successful move

} //end routine movePlayerTo()


////////////////////////////////////////////////////////////////////////////////
/// \brief Moves token to specified room.
/// \param PersonType: character for token of interest
/// \param RoomType: destination
/// \return bool: move success
/// \throw
/// - INSUFFICIENT_DATA when cannot find corresponding token or room object.
/// \note
/// - Assumes destination is valid.
////////////////////////////////////////////////////////////////////////////////
bool
Board::movePersonTokenToRoom(
	clueless::PersonType character, //i - token's character
	clueless::RoomType room) //i - destination
{
	bool has_token_moved( false );

	PersonPiece* token( fetchPersonToken( character ) );
	Room* destination( fetchRoom( room ) );

	if( ! token  ||  ! destination )
	{
		std::ostringstream msg;
		msg << "Board::movePersonTokenToRoom()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  cannot find token or room object";
		throw std::logic_error( msg.str().c_str() );
	}

	Location* curr_location( token->_location );

	if( curr_location != (Location*)destination )
	{
		//update room occupants and game piece location
		destination->addOccupant( token );
		token->_location = destination;

		//vacate current location
		curr_location->recognizeOccupantLeft( token );

		has_token_moved = true;
	}

	return has_token_moved;

} //end routine movePersonTokenToRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Moves token to specified room.
/// \param WeaponType: character for token of interest
/// \param RoomType: destination
/// \return bool: move success
/// \throw
/// - INSUFFICIENT_DATA when cannot find corresponding token or room object.
/// \note
/// - Assumes destination is valid.
////////////////////////////////////////////////////////////////////////////////
bool
Board::moveWeaponTokenToRoom(
	clueless::WeaponType wpn, //i - weapon
	clueless::RoomType room) //i - destination
{
	bool has_token_moved( false );

	WeaponPiece* token( fetchWeaponToken( wpn ) );
	Room* destination( fetchRoom( room ) );

	if( ! token  ||  ! destination )
	{
		std::ostringstream msg;
		msg << "Board::moveWeaponTokenToRoom()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  cannot find token or room object";
		throw std::logic_error( msg.str().c_str() );
	}

	Location* curr_location( token->_location );

	if( curr_location != (Location*)destination )
	{
		//update room occupants and game piece location
		destination->addOccupant( token );
		token->_location = destination;

		//vacate current location
		curr_location->recognizeOccupantLeft( token );

		has_token_moved = true;
	}

	return has_token_moved;

} //end routine moveWeaponTokenToRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief Associates specified character game token with player.
/// \param Player: player
/// \param PersonType: character
/// \return None
/// \throw
/// - INSUFFICIENT_DATA when player object dne
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
Board::assignCharacterToPlayer(
	Player* player, //io- 
	clueless::PersonType character) //i - 
{
	if( ! player )
	{
		std::ostringstream msg;
		msg << "Board::assignCharacterToPlayer()\n"
			<< "  INSUFFICIENT_DATA\n"
			<< "  Player object does not exist";
		throw std::logic_error( msg.str() );
	}

	//associate Player with person token
	_personTokens[character]->setAssociatedPlayer( player );

	//update Player reference to token
	player->acceptAssocCharacterToken( _personTokens[character] );

} //end routine assignCharacterToPlayer()
