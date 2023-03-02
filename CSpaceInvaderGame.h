// CSpaceInvadeGame.h
#pragma once

#include "CBase4618.h"
#include "CControl.h"
#include "CInvader.h"
#include "CMissile.h"
#include "CShip.h"


#include <vector>
#include <opencv2/opencv.hpp>
#include <sstream>

class CSpaceInvaderGame : public CBase4618 
{
private:
   CShip ship;

   int resX, resY;
   float xdata;
   int posX;
   int maxShipVelocity;

   int stateBefore1, stateBefore2;

   int score;
   int _lives;
   int numberOfMissiles;
   bool fireMissle;

   int invaderRows;
   int invaderColumns;
   int invaderSpace;
   int invaderPoints;
   int invaderDirection;

public:
	std::vector <CInvader> vecInvaders;
	std::vector <CMissile> vecMissiles;

   CSpaceInvaderGame();
  // ~CSpaceInvaderGame();

   void draw(cv::Mat& canvas) override;

   void update() override;

   void reset();
};