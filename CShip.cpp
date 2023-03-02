// CShip.cpp
#include "stdafx.h"
#include "CShip.h"

CShip::CShip() 
{
   _position = cv::Point2f(500, 730);
   _radius = 15;
   _lives = 3;
}

float CShip::get_posX()
{
   return _position.x;
}

