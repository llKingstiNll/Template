// CShip.h
#pragma once

#include "CGameObject.h"
#include "CControl.h"

/**
* @class CShip
* 
* @brief This class stores the values of the ship which inherit from CGameObject.
* It stores the position, the radius, and the number of lives of the player.
* 
* @author Bailey Hung
*/
class CShip : public CGameObject
{
public:
   /**
   * @brief Contructor call for the ship. 
   * Initializes the ship values at the start of the game.
   * 
   * @return Nothing to return.
   */
   CShip();
};