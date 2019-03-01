////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file GamePiece.cpp
/// \brief
///
/// \date   28 Feb 2019  1430
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "GamePiece.h"

#include "CluelessEnums.h"	//for PersonType, WeaponType use

#include <iostream>			//for std::cout use

//------------------------------------------------------------------------------
// Constructors / Destructors
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Default constructor.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
GamePiece::GamePiece()
	: _location( nullptr )
{
}


////////////////////////////////////////////////////////////////////////////////
/// \brief Extended constructor
/// \param PieceType: type of piece (person, weapon)
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
GamePiece::GamePiece(
	GamePiece::PieceType type) //i - type of piece
	: _location( nullptr )
{
}


//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Builds association between Player and character game token.
/// \param Player: game player
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
PersonPiece::setAssociatedPlayer(
	Player* player) //i - player who will use as game token
{
	if( _assocPlayer )
	{
		std::ostringstream msg;
		msg << "PersonPiece::setAssociatedPlayer()\n"
			<< "  LOGIC_ERROR\n"
			<< "  character piece already in use by " << _assocPlayer->getName();
		throw std::logic_error( msg.str() );
	}

	_assocPlayer = player;

	std::cout << player->getName() << " will use " << getName() << " character token.\n";

} //end routine setAssociatedPlayer()


//------------------------------------------------------------------------------
// Additional Member Functions
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Constructs name based on type.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
PersonPiece::populateName()
{
	std::ostringstream name;
	name << clueless::translatePersonTypeToText( _person );
	_name = name.str();

} //end routine populateName()


////////////////////////////////////////////////////////////////////////////////
/// \brief Constructs name based on type.
/// \param None
/// \return None
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
void
WeaponPiece::populateName()
{
	std::ostringstream name;
	name << clueless::translateWeaponTypeToText( _weapon );
	_name = name.str();

} //end routine populateName()
