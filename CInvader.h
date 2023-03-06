// CInvader.h
#pragma once
#include "CGameObject.h"

/**
* 
* @class CInvader
* 
* @brief This class contains the invader values and it inherits from CGameObject
* It contains a position attribute and can perform actions such as moving and shooting.
* 
* @auhor Bailey Hung
*/
class CInvader : public CGameObject
{
public:
   /**
   * @brief Constucto for CInvader.
   * 
   * @param position The initial position of the invader.
   */
   CInvader(cv::Point2f position);
};
