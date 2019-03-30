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
/// \param Player: player of interest
/// \return bool: whether have shown to player
/// \throw None
/// \note  None
////////////////////////////////////////////////////////////////////////////////
bool
NotebookEntry::haveShownToPlayer(
	const Player* player) //i - player
const
{
	std::set<const Player*>::const_iterator player_iter(
		_playersShown.find(player) );

	//whether found player
	return( _playersShown.end() != player_iter );

} //end routine haveShownToPlayer()
