// CSpaceInvaderGame.cpp
#include "stdafx.h"
#include "CSpaceInvaderGame.h"

CSpaceInvaderGame::CSpaceInvaderGame()
{
   // Canvas Resolution
   resX = 1000;
   resY = 800;

   // Initiate Canvas
   CBase4618::canvas = cv::Mat::zeros(resY, resX, CV_8UC3);
  // CBase4618::canvas = cv::Scalar(0, 0, 0);

   // Analog Start State
   xdata = control.get_analog(2, posX);
   xdata = (1 - (xdata / 100));

   // Getting Starting Button State
   control.get_data(0, 33, stateBefore1);
   control.get_data(0, 32, stateBefore2);

   //
   maxShipVelocity = 60;

   reset();
}

//CSpaceInvaderGame::~CSpaceInvaderGame() {}

void CSpaceInvaderGame::draw(cv::Mat& canvas)
{
   //CBase4618::canvas = cv::Scalar(0, 0, 0);

   // Drawing Invaders
   for (int i = 0; i < vecInvaders.size(); i++)
   {
      vecInvaders[i].draw(canvas);
   }

   // Drawing Ship
   ship.draw(canvas);

   // Draw Missiles
   for (int i = 0; i < vecMissiles.size(); i++)
   {
      vecMissiles[i].draw(canvas);
   }

   // HUD Background
   cv::rectangle(canvas, cv::Point(0, 0), cv::Point(1000, 40), cv::Scalar(50, 50, 50), -1);
 
   // Draw text on top of background rectangles
   putText(canvas, "Score: " + std::to_string(score), cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
   putText(canvas, "Number Of Missiles: " + std::to_string(numberOfMissiles), cv::Point(350, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
   putText(canvas, "Lives: " + std::to_string(_lives), cv::Point(850, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
}

void CSpaceInvaderGame::update()
{
   canvas = cv::Scalar(0, 0, 0);
   // Ship Movement
   float xdata;
   int posX;
   xdata = control.get_analog(2, posX);
   if (xdata >= 55.0)
   {
      xdata = maxShipVelocity * (-(xdata - 50) / 100);
   }
   else if (xdata <= 45.0)
   {
      xdata = maxShipVelocity * (((100 - xdata) - 50) / 100);
   }
   else xdata = 0.0;

   std::cout << xdata << "\r";

   int newPos;
   newPos = ship.get_posX() - xdata;

   if (newPos < 0) 
   {
      newPos = 1000;
   }
   if (newPos > 1000)
   {
      newPos = 0;
   }

   ship.set_pos(cv::Point2f(newPos, 750));

   // Firing Missiles
   if (control.get_button(33))
   {
      CMissile missile(ship.get_pos()); 
 
      vecMissiles.push_back(missile);
      std::cout << "SHOT\n";

      // Wait Time
      std::chrono::milliseconds duration(20);
      std::this_thread::sleep_for(duration);
   }

   // Moving Missiles
   for (int i = 0; i < vecMissiles.size(); i++) 
   {
      vecMissiles[i].move();
      std::cout << vecMissiles[0].get_pos().y;
      if (vecMissiles[i].get_pos().y <= 0)
      {
          vecMissiles.erase(vecMissiles.begin() + i);
      }
   }
   numberOfMissiles = vecMissiles.size();

   // Moving Invaders
   if (invaderDirection <= 100) //invader going left
   {
       for (int i = 0; i < vecInvaders.size(); i++)
       {
           vecInvaders[i].move();
       }
       invaderDirection++;
   }
   if (invaderDirection >= 100) //invader going right
   {
       
       for (int i = 0; i < vecInvaders.size(); i++)
       {
           vecInvaders[i].move();
       }
       invaderDirection = 0;
   }


   // Reset 
   if (control.get_button(32))
   {
      std::cout << "Reset\n";
      reset();
   }
}

void CSpaceInvaderGame::reset()
{
   // Default Values
   score = 0;
   _lives = 3;
   numberOfMissiles = 0;
   fireMissle = false;

   // Invader Values
   invaderRows = 3;
   invaderColumns = 10;
   invaderSpace = 60;
   invaderPoints = 10;
   invaderDirection = 0;

   // Initializing Invaders
   for (int row = 0; row < invaderRows; row++)
   {
      for (int col = 0; col < invaderColumns; col++)
      {
         cv::Point2f invaderPosition(invaderSpace * col + 250.0, invaderSpace * row + 80.0);
         CInvader invader(invaderPosition);
         vecInvaders.push_back(invader);
      }
   }
}