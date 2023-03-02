// CGameObject.cpp
#include "stdafx.h"
#include "CSpaceInvaderGame.h"
#include "CGameObject.h"
#include <opencv2/imgproc.hpp>

//CGameObject::CGameObject() {}

void CGameObject::move()
{
   _position += _velocity;
}

void CGameObject::changeVelocity()
{
   _velocity = -_velocity;
}

bool CGameObject::collide(CGameObject &obj) 
{
   float distance = cv::norm(_position - obj._position);

   // If the distance is less than or equal to the sum of the radii, they are colliding
   if (distance <= _radius + obj._radius)
   {
      return true;
   }
   else
   {
      return false;
   }
}

bool CGameObject::collide_wall(cv::Size board) 
{
   return true;

   // Implementation
}

void CGameObject::hit() 
{
   _lives--;
}

void CGameObject::draw(cv::Mat& canvas) 
{
   cv::circle(canvas, _position, _radius, cv::Scalar(255, 255, 255), -1);
}
