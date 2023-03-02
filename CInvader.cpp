// CInvader.cpp
#include "stdafx.h"
#include "CInvader.h"

CInvader::CInvader(cv::Point2f position) // Modified constructor definition
{
   _position = position;
   _radius = 15;
   _lives = 1;
   _velocity = cv::Point2f(5, 0);
}

