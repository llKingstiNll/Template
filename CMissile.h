// CMissile.h
#pragma once

#include "CGameObject.h"

/**
* @class CMissile
* 
* @brief This class stores the values of each missile and inherits from CGameObject.
* The missiles contain a position vector, and a velocity
* 
* @author Bailey Hung
*/
class CMissile : public CGameObject
{
public:
   /**
   * @brief Contructor call for the missiles.
   * 
   * @peram position The current position of the missile.
   * 
   * @return Nothing to return.
   */
   CMissile(cv::Point2f position);
};