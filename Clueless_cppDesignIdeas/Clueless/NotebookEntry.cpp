////////////////////////////////////////////////////////////////////////////////
///
/// Clue-Less
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file NotebookEntry.cpp
/// \brief
///
/// \date   30 Mar 2019  0947
///
/// \note None
///
////////////////////////////////////////////////////////////////////////////////

#include "DetectiveNotebook.h"

//------------------------------------------------------------------------------
// Accessors and Mutators
//------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// \brief Determines whether have previously shown entry to specified player.
/// \param PersonType: player character of interest
/// \return bool: whether have shown to player
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
NotebookEntry::haveShownToPlayer(
	clueless::PersonType player_character) //i - player
const
{
	std::set<clueless::PersonType>::const_iterator player_iter(
		_playersShown.find(player_character) );

	//whether found player
	return( _playersShown.end() != player_iter );

} //end routine haveShownToPlayer()
