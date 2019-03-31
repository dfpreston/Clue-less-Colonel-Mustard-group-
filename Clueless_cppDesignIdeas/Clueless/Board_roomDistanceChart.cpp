////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Board_populateDistanceChart.cpp
/// \brief
///
/// \date   31 Mar 2019  0339
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "Board.h"

#include "Room.h"

//------------------------------------------------------------------------------
// populate location-room distance chart
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief 
////////////////////////////////////////////////////////////////////////////////
void
Board::addDataToDistanceChart(
	const Location* start_loc, //i - starting location
	const Room* destination,   //i - destination room
	size_t distance,           //i - distance (number of moves)
	Location* next_loc)        //i - next step along shortest path
{
	std::pair<const Location*, const Room*> location_room;
	std::pair<size_t, Location*> distance_nextLocation;

	location_room = std::make_pair(start_loc, destination);
	distance_nextLocation = std::make_pair(distance, next_loc);
	_roomDistanceChart[location_room] = distance_nextLocation;

} //end routine addDataToDistanceChart()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
////////////////////////////////////////////////////////////////////////////////
Location*
Board::fetchDistanceToRoom(
	const Location* start_loc, //i - starting point
	clueless::RoomType destination, //i - destination
	size_t& distance) // o- distance (number of moves)
{
	Room* destination_room( fetchRoom(destination) );

	return(
		fetchDistanceToRoom(
			start_loc,
			destination_room,
			distance) );

} //end routine fetchDistanceToRoom()


Location*
Board::fetchDistanceToRoom(
	const Location* start_loc, //i - starting point
	const Room* destination, //i - destination
	size_t& distance) // o- distance (number of moves)
{
	Location* next_loc( nullptr );

	distance = UINT_MAX;

	std::pair<const Location*, const Room*> location_room;
	//std::pair<size_t, Location*> distance_nextLocation;

	location_room = std::make_pair(start_loc, destination);
	std::map<std::pair<const Location*, const Room*>, std::pair<size_t, Location*>>::const_iterator
		pair_iter( _roomDistanceChart.find(location_room) );

	//if found start-destination pair
	if( _roomDistanceChart.end() != pair_iter )
	{
		distance = pair_iter->second.first;
		next_loc = pair_iter->second.second;
	}

	return next_loc;

} //end routine fetchDistanceToRoom()


////////////////////////////////////////////////////////////////////////////////
/// \brief 
////////////////////////////////////////////////////////////////////////////////
void
Board::populateDistanceChart_studyKitchen()
{
	//study --> kitchen
	addDataToDistanceChart(_study, _kitchen, 1, _kitchen);

	//study --> ballroom
	addDataToDistanceChart(_study, _ballroom, 3, _kitchen);

	//study --> dining room
	addDataToDistanceChart(_study, _diningRoom, 3, _kitchen);

	//kitchen --> study
	addDataToDistanceChart(_kitchen, _study, 1, _study);

	//kitchen --> hall
	addDataToDistanceChart(_kitchen, _hall, 3, _study);

	//kitchen --> library
	addDataToDistanceChart(_kitchen, _library, 3, _study);

} //end routine populateDistanceChart_studyKitchen()


void
Board::populateDistanceChart_loungeConservatory()
{
	//lounge --> conservatory
	addDataToDistanceChart(_lounge, _conservatory, 1, _conservatory);

	//lounge --> library
	addDataToDistanceChart(_lounge, _library, 3, _conservatory);

	//lounge --> ballroom
	addDataToDistanceChart(_lounge, _ballroom, 3, _conservatory);

	//conservatory --> lounge
	addDataToDistanceChart(_conservatory, _lounge, 1, _lounge);

	//conservatory --> dining room
	addDataToDistanceChart(_conservatory, _diningRoom, 3, _lounge);

	//conservatory --> hall
	addDataToDistanceChart(_conservatory, _hall, 3, _lounge);

} //end routine populateDistanceChart_loungeConservatory()


void
Board::populateDistanceChart_hallwayStudyHall(
	Hallway* hallway) //i - study-hall
{
	//study --> hall
	addDataToDistanceChart(_study, _hall, 2, hallway);

	//study --> lounge
	addDataToDistanceChart(_study, _lounge, 4, hallway);

	//hall --> study
	addDataToDistanceChart(_hall, _study, 2, hallway);

	//hall --> kitchen
	addDataToDistanceChart(_hall, _kitchen, 3, hallway);

	//hall --> library
	addDataToDistanceChart(_hall, _library, 4, hallway);

	//study-hall --> study

	//study-hall --> hall


} //end routine populateDistanceChart_hallwayStudyHall()


void
Board::populateDistanceChart_hallwayHallLounge(
	Hallway* hallway) //i - hall-lounge
{
	//hall --> lounge
	addDataToDistanceChart(_hall, _lounge, 2, hallway);

	//hall --> conservatory
	addDataToDistanceChart(_hall, _conservatory, 3, hallway);

	//lounge --> hall
	addDataToDistanceChart(_lounge, _hall, 2, hallway);

	//lounge --> study
	addDataToDistanceChart(_lounge, _study, 4, hallway);

	//lounge --> billiard room
	addDataToDistanceChart(_lounge, _billiardRoom, 4, hallway);

	//hall-lounge --> hall

	//hall-lounge --> lounge


} //end routine populateDistanceChart_hallwayHallLounge()

void
Board::populateDistanceChart_hallwayStudyLibrary(
	Hallway* hallway) //i - study-library
{
	//study --> library
	addDataToDistanceChart(_study, _library, 2, hallway);

	//study --> conservatory
	addDataToDistanceChart(_study, _conservatory, 4, hallway);

	//study --> billiard room
	addDataToDistanceChart(_study, _billiardRoom, 4, hallway);

	//library --> study
	addDataToDistanceChart(_library, _study, 2, hallway);

	//library --> kitchen
	addDataToDistanceChart(_library, _kitchen, 3, hallway);

	//study-library --> study

	//study-library --> library


} //end routine populateDistanceChart_hallwayStudyLibrary()

void
Board::populateDistanceChart_hallwayHallBilliardRoom(
	Hallway* hallway) //i - hall-billiard room
{
	//hall --> billiard room
	addDataToDistanceChart(_hall, _billiardRoom, 2, hallway);

	//hall --> ballroom
	addDataToDistanceChart(_hall, _ballroom, 4, hallway);

	//hall --> dining room
	addDataToDistanceChart(_hall, _diningRoom, 4, hallway);

	//billiard room --> hall
	addDataToDistanceChart(_billiardRoom, _hall, 2, hallway);

	//billiard room --> study
	addDataToDistanceChart(_billiardRoom, _study, 4, hallway);

	//hall-billiard room --> hall

	//hall-billiard room --> billiard room


} //end routine populateDistanceChart_hallwayHallBilliardRoom()


void
Board::populateDistanceChart_hallwayLoungeDiningRoom(
	Hallway* hallway) //i - lounge-dining room
{
	//lounge --> dining room
	addDataToDistanceChart(_lounge, _diningRoom, 2, hallway);

	//lounge --> kitchen
	addDataToDistanceChart(_lounge, _kitchen, 4, hallway);

	//dining room --> lounge
	addDataToDistanceChart(_diningRoom, _lounge, 2, hallway);

	//dining room --> conservatory
	addDataToDistanceChart(_diningRoom, _conservatory, 3, hallway);

	//dining room --> hall
	addDataToDistanceChart(_diningRoom, _hall, 4, hallway);

	//lounge-dining room --> lounge

	//lounge-dining room --> dining room


} //end routine populateDistanceChart_hallwayLoungeDiningRoom()

void
Board::populateDistanceChart_hallwayLibraryBilliardRoom(
	Hallway* hallway) //i - library-billiard room
{
	//library --> billiard room
	addDataToDistanceChart(_library, _billiardRoom, 2, hallway);

	//library --> dining room
	addDataToDistanceChart(_library, _diningRoom, 4, hallway);

	//library --> hall
	addDataToDistanceChart(_library, _hall, 4, hallway);

	//billiard room --> library
	addDataToDistanceChart(_billiardRoom, _library, 2, hallway);

	//billiard room --> conservatory
	addDataToDistanceChart(_billiardRoom, _conservatory, 4, hallway);

	//library-billiard room --> library

	//library-billiard room --> billiard room


} //end routine populateDistanceChart_hallwayLibraryBilliardRoom()


void
Board::populateDistanceChart_hallwayBilliardRoomDiningRoom(
	Hallway* hallway) //i - billiard room-dining room
{
	//billiard room --> dining room
	addDataToDistanceChart(_billiardRoom, _diningRoom, 2, hallway);

	//billiard room --> lounge
	addDataToDistanceChart(_billiardRoom, _lounge, 4, hallway);

	//dining room --> billiard room
	addDataToDistanceChart(_diningRoom, _billiardRoom, 2, hallway);

	//dining room --> library
	addDataToDistanceChart(_diningRoom, _library, 4, hallway);

	//dining room --> ballroom
	addDataToDistanceChart(_diningRoom, _ballroom, 4, hallway);

	//billiard room-dining room --> billiard room

	//billiard room-dining room --> dining room


} //end routine populateDistanceChart_hallwayBilliardRoomDiningRoom()


void
Board::populateDistanceChart_hallwayLibraryConservatory(
	Hallway* hallway) //i - library-conservatory
{
	//library --> conservatory
	addDataToDistanceChart(_library, _conservatory, 2, hallway);

	//library --> lounge
	addDataToDistanceChart(_library, _lounge, 3, hallway);

	//library --> ballroom
	addDataToDistanceChart(_library, _ballroom, 4, hallway);

	//conservatory --> library
	addDataToDistanceChart(_conservatory, _library, 2, hallway);

	//conservatory --> study
	addDataToDistanceChart(_conservatory, _study, 4, hallway);

	//library-conservatory --> library

	//library-conservatory --> conservatory


} //end routine populateDistanceChart_hallwayLibraryConservatory()


void
Board::populateDistanceChart_hallwayBilliardRoomBallroom(
	Hallway* hallway) //i - billiard room-ballroom
{
	//billiard room --> ballroom
	addDataToDistanceChart(_billiardRoom, _ballroom, 2, hallway);

	//billiard room --> kitchen
	addDataToDistanceChart(_billiardRoom, _kitchen, 4, hallway);

	//ballroom --> billiard room
	addDataToDistanceChart(_ballroom, _billiardRoom, 2, hallway);

	//ballroom --> hall
	addDataToDistanceChart(_ballroom, _hall, 4, hallway);

	//ballroom --> library
	addDataToDistanceChart(_ballroom, _library, 4, hallway);

	//billiard room-ballroom --> billiard room

	//billiard room-ballroom --> ballroom


} //end routine populateDistanceChart_hallwayBilliardRoomBallroom()


void
Board::populateDistanceChart_hallwayDiningRoomKitchen(
	Hallway* hallway) //i - dining room-kitchen
{
	//dining room --> kitchen
	addDataToDistanceChart(_diningRoom, _kitchen, 2, hallway);

	//dining room --> study
	addDataToDistanceChart(_diningRoom, _study, 3, hallway);

	//kitchen --> dining room
	addDataToDistanceChart(_kitchen, _diningRoom, 2, hallway);

	//kitchen --> lounge
	addDataToDistanceChart(_kitchen, _lounge, 4, hallway);

	//kitchen --> billiard room
	addDataToDistanceChart(_kitchen, _billiardRoom, 4, hallway);

	//dining room-kitchen --> dining room

	//dining room-kitchen --> kitchen


} //end routine populateDistanceChart_hallwayDiningRoomKitchen()


void
Board::populateDistanceChart_hallwayConservatoryBallroom(
	Hallway* hallway) //i - conservatory-ballroom
{
	//conservatory --> ballroom
	addDataToDistanceChart(_conservatory, _ballroom, 2, hallway);

	//conservatory --> kitchen
	addDataToDistanceChart(_conservatory, _kitchen, 4, hallway);

	//conservatory --> billiard room
	addDataToDistanceChart(_conservatory, _billiardRoom, 4, hallway);

	//ballroom --> conservatory
	addDataToDistanceChart(_ballroom, _conservatory, 2, hallway);

	//ballroom --> lounge
	addDataToDistanceChart(_ballroom, _lounge, 3, hallway);

	//conservatory-ballroom --> conservatory

	//conservatory-ballroom --> ballroom


} //end routine populateDistanceChart_hallwayConservatoryBallroom()


void
Board::populateDistanceChart_hallwayBallroomKitchen(
	Hallway* hallway) //i - ballroom-kitchen
{
	//ballroom --> kitchen
	addDataToDistanceChart(_ballroom, _kitchen, 3, hallway);

	//ballroom --> study
	addDataToDistanceChart(_ballroom, _study, 3, hallway);

	//ballroom --> dining room
	addDataToDistanceChart(_ballroom, _diningRoom, 4, hallway);

	//kitchen --> ballroom
	addDataToDistanceChart(_kitchen, _ballroom, 2, hallway);

	//kitchen --> conservatory
	addDataToDistanceChart(_kitchen, _conservatory, 4, hallway);

	//ballroom-kitchen --> ballroom

	//ballroom-kitchen --> kitchen


} //end routine populateDistanceChart_hallwayBallroomKitchen()

