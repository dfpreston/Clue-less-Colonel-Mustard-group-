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
	std::cout << "Creating Clue-less game board...\n";

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
	std::set<Room*>::iterator room_iter( _rooms.begin() );
	Room* curr_room( nullptr );

	while( room_iter != _rooms.end() )
	{
		curr_room = *room_iter;
		_rooms.erase( room_iter );
		delete curr_room;

		room_iter = _rooms.begin();
	}

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

	std::set<Room*>::const_iterator room_iter( _rooms.begin() );
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

	//lounge-conservatory
	_lounge->createSecretPassageTo( _conservatory );
	_conservatory->createSecretPassageTo( _lounge );

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

	// 1: study-hall
	Hallway* hallway = new Hallway(_study, _hall);
	_hallways.insert( hallway );

	// 2: hall-lounge + Miss Scarlet starting place
	hallway = new Hallway(_hall, _lounge);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_missScarlet, hallway) );

	// 3: study-library
	hallway = new Hallway(_study, _library);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_professorPlum, hallway) );

	// 4: hall-billiard room
	hallway = new Hallway(_hall, _billiardRoom);
	_hallways.insert( hallway );

	// 5: lounge-dining room
	hallway = new Hallway(_lounge, _diningRoom);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_colonelMustard, hallway) );

	// 6: library-billiard room
	hallway = new Hallway(_library, _billiardRoom);
	_hallways.insert( hallway );

	// 7: billiard room-dining room
	hallway = new Hallway(_billiardRoom, _diningRoom);
	_hallways.insert( hallway );

	// 8: library-conservatory
	hallway = new Hallway(_library, _conservatory);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrsPeacock, hallway) );

	// 9: billiard room-ballroom
	hallway = new Hallway(_billiardRoom, _ballroom);
	_hallways.insert( hallway );

	//10: dining room-kitchen
	hallway = new Hallway(_diningRoom, _kitchen);
	_hallways.insert( hallway );

	//11: conservatory-ballroom
	hallway = new Hallway(_conservatory, _ballroom);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrGreen, hallway) );

	//12: ballroom-kitchen
	hallway = new Hallway(_ballroom, _kitchen);
	_hallways.insert( hallway );

	_personHomes.insert( new HomeLocation(_mrsWhite, hallway) );

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
	std::set<Room*> rooms = _rooms;
	std::set<Room*>::iterator room_iter( rooms.begin() );
	Room* chosen_room( nullptr );
	
	std::map<clueless::WeaponType, GamePiece*>::iterator wpn_iter( _weaponTokens.begin() );
	GamePiece* wpn_token( nullptr );
	while( _weaponTokens.end() != wpn_iter )
	{
		chosen_room = chooseRoom(&rooms);
		wpn_token = wpn_iter->second;

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
Room*
Board::chooseRoom(
	std::set<Room*>* rooms) //i - rooms to choose amongst
const
{
	Room* choice( nullptr );

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
		size_t zero_based_pos( size_t(std::floor( MersenneTwister::drawReal2(0, rooms->size()) )) );

		std::set<Room*>::const_iterator room_iter( rooms->begin() );
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
/// \brief Associates specified character game token with player.
/// \param 
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

} //end routine assignCharacterToPlayer()
