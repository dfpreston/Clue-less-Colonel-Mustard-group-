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

#include <string>		//for std::string use


//forward declarations
// - none


class Player
{
	//--------------------------------------------------------------------------
	// Constructors / Destructor
	//--------------------------------------------------------------------------
private:
	/// \brief Default constructor
	Player()
		: _name( "" )
	{
	}

public:
	/// \brief Extended constructor
	Player(std::string name, clueless::PersonType gameCharacter)
		: _name( name )
		, _character( gameCharacter )
	{
	}


	/// \brief Destructor
	virtual ~Player()
	{
	}


	//--------------------------------------------------------------------------
	// Accessors and Mutators
	//--------------------------------------------------------------------------
	std::string getName() const;
	clueless::PersonType getCharacter() const;

	//--------------------------------------------------------------------------
	// Data Members
	//--------------------------------------------------------------------------
public:
	std::string _name;
	clueless::PersonType _character;

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


#endif //Player_h
