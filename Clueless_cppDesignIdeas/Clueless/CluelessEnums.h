////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file Card.h
/// \brief Enumerated types intended for use across the Clue-less Game space.
///
/// \date   26 Feb 2019  1215
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CluelessEnums_h
#define CluelessEnums_h

#include <string>


namespace clueless
{

////////////////////////////////////////////////////////////////////////////////
// Cards, Weapons, Rooms
////////////////////////////////////////////////////////////////////////////////
enum PersonType
{
	UNKNOWN_PERSON = 0,

	MISS_SCARLET,
	COLONEL_MUSTARD,
	MRS_WHITE,
	MR_GREEN,
	MRS_PEACOCK,
	PROFESSOR_PLUM

}; //end enum PersonType defn


enum WeaponType
{
	UNKNOWN_WEAPON = 0,

	LEAD_PIPE,
	KNIFE,
	ROPE,
	CANDLESTICK,
	REVOLVER,
	WRENCH

}; //end enum WeaponType defn


enum RoomType
{
	UNKNOWN_ROOM = 0,

	STUDY,
	HALL,
	LOUNGE,
	LIBRARY,
	BILLIARD_ROOM,
	DINING_ROOM,
	CONSERVATORY,
	BALLROOM,
	KITCHEN

}; //end enum RoomType defn


////////////////////////////////////////////////////////////////////////////////
// Turn Options
////////////////////////////////////////////////////////////////////////////////
enum TurnOptionType
{
	DO_NOTHING_ELSE_THIS_TURN = 0,

	MOVE,
	MAKE_SUGGESTION,
	MAKE_ACCUSATION

}; //end enum TurnOptionType defn


//------------------------------------------------------------------------------
// Number of Type
//------------------------------------------------------------------------------
static size_t getNumPersonTypes()
{
	return( size_t(PersonType::PROFESSOR_PLUM) );

} //end routine getNumPersonTypes()


static size_t getNumWeaponTypes()
{
	return( size_t(WeaponType::WRENCH) );

} //end routine getNumWeaponTypes()


static size_t getNumRoomTypes()
{
	return( size_t(RoomType::KITCHEN) );

} //end routine getNumRoomTypes()


//------------------------------------------------------------------------------
// Translation from Type to Text
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Translates specified person type to text.
/// \param PersonType: type to translate
/// \return string: text translation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
static std::string
translatePersonTypeToText(
	PersonType person_type) //i - type to translate
{
	switch( person_type )
	{
	case MISS_SCARLET:
		return "Miss Scarlet";
		break;

	case COLONEL_MUSTARD:
		return "Colonel Mustard";
		break;

	case MRS_WHITE:
		return "Mrs. White";
		break;

	case MR_GREEN:
		return "Mr. Green";
		break;

	case MRS_PEACOCK:
		return "Mrs. Peacock";
		break;

	case PROFESSOR_PLUM:
		return "Professor Plum";
		break;

	case UNKNOWN_PERSON:
	default:
		;

	} //end switch (on person type)

	return "unknown person type";

} //end routine translatePersonTypeToText()


////////////////////////////////////////////////////////////////////////////////
/// \brief Translates specified weapon type to text.
/// \param WeaponType: type to translate
/// \return string: text translation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
static std::string
translateWeaponTypeToText(
	WeaponType wpn_type) //i - type to translate
{
	switch( wpn_type )
	{
	case LEAD_PIPE:
		return "lead pipe";
		break;

	case KNIFE:
		return "knife";
		break;

	case ROPE:
		return "rope";
		break;

	case CANDLESTICK:
		return "candlestick";
		break;

	case REVOLVER:
		return "revolver";
		break;

	case WRENCH:
		return "wrench";
		break;

	case UNKNOWN_WEAPON:
	default:
		;

	} //end switch (on weapon type)

	return "unknown weapon type";

} //end routine translateWeaponTypeToText()
 
 
////////////////////////////////////////////////////////////////////////////////
/// \brief Translates specified room type to text.
/// \param RoomType: type to translate
/// \return string: text translation
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
static std::string
translateRoomTypeToText(
	RoomType room_type) //i - type to translate
{
	switch( room_type )
	{
	case STUDY:
		return "study";
		break;

	case HALL:
		return "hall";
		break;

	case LOUNGE:
		return "lounge";
		break;

	case LIBRARY:
		return "library";
		break;

	case BILLIARD_ROOM:
		return "billiard room";
		break;

	case DINING_ROOM:
		return "dining room";
		break;

	case CONSERVATORY:
		return "conservatory";
		break;

	case BALLROOM:
		return "ballroom";
		break;

	case KITCHEN:
		return "kitchen";
		break;

	case UNKNOWN_ROOM:
	default:
		;

	} //end switch (on room type)

	return "unknown room type";

} //end routine translateRoomTypeToText()


  ////////////////////////////////////////////////////////////////////////////////
  /// \brief Translates specified room type to text.
  /// \param TurnOptionType: type to translate
  /// \return string: text translation
  /// \throw None
  /// \note  None
  ////////////////////////////////////////////////////////////////////////////////
static std::string
translateTurnOptionTypeToText(
	TurnOptionType option_type) //i - type to translate
{
	switch( option_type )
	{
	case DO_NOTHING_ELSE_THIS_TURN:
		return "do nothing else this turn";
		break;

	case MOVE:
		return "move";
		break;

	case MAKE_SUGGESTION:
		return "make suggestion";
		break;

	case MAKE_ACCUSATION:
		return "make accusation";
		break;

	default:
		;

	} //end switch (on room type)

	return "unknown turn option";

} //end routine translateTurnOptionTypeToText()


} //end namespace clueless scope

#endif //CluelessEnums_h
