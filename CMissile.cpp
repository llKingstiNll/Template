// CMissile.cpp
#include "stdafx.h"
#include "CMissile.h"

CMissile::CMissile(cv::Point2f position)
{
   _position = position;
   _velocity = cv::Point2f(0, -50);
   _radius = 5;
}
