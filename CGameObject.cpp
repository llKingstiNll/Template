// CGameObject.cpp
#include "stdafx.h"
#include "CSpaceInvaderGame.h"
#include "CGameObject.h"
#include <opencv2/imgproc.hpp>

//CGameObject::CGameObject() {}

void CGameObject::move()
{
   //static auto prevTime = std::chrono::high_resolution_clock::now(); 
   //auto currentTime = std::chrono::high_resolution_clock::now();
   //float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - prevTime).count();
   //prevTime = currentTime; 
   _position += _velocity;
}

bool CGameObject::collide(CGameObject &obj) 
{
   return true;
}

bool CGameObject::collide_wall(cv::Size board) 
{
   return true;

   // Implementation
}

void CGameObject::hit() 
{
   // Implementation
}

void CGameObject::draw(cv::Mat& canvas) 
{
   cv::circle(canvas, _position, _radius, cv::Scalar(255, 255, 255), -1);
}
