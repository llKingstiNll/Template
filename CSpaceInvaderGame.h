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

/**
* @class CSpaceInvaderGame
* 
* @brief This class runs the Space Invaders game which inherits from CBase4618.
* it contains the objects of other classes to run the game and compute interactions.
* 
* @author Bailey Hung
*/
class CSpaceInvaderGame : public CBase4618 
{
private:
   CShip ship; ///< Instance of the player ship.

   int resX, resY;	///< X and Y resolutions of the canvas
   float xdata;	///< Stores the analog data of the joystick if the x position.
   int posX;	///< Pointer to get the x value of the joystick.
   int maxShipVelocity;	///< The maximum shop velocity.

   int stateBefore1, stateBefore2;	///< Initial button 1 and 2 states.

   int score;	///< Score of the player.
   int numberOfMissiles;	///< Number of missiles the player has fired.

   int invaderRows;	///< Number of Invader Rows.
   int invaderColumns;	///< Number of Invader Columns.
   int invaderSpace;	///< Space in between each invader.
   int invaderDirection;	///< Direction that the invaders travel, 0 for left and 1 for right.

public:
	std::vector <CInvader> vecInvaders; ///< A vector that stores each instance of the invaders.
	std::vector <CMissile> vecMissiles;	///< A vector that stores each instance of the missiles the player has fried.
	std::vector <CMissile> vecInvaderMissiles; ///< A vector that stores each instance of missiles the invaders have fired.

   CSpaceInvaderGame();
  // ~CSpaceInvaderGame();

   /**
   * @brief This function draws to the canvas.
   * It draws the UI for the player score, missiles on screen and player lives.
   * 
   * @peram canvas The pointer to the canvas that it is drawing to.
   * 
   * @return Nothing to return.
   */
   void draw(cv::Mat& canvas) override;

   /**
   * @brief Function that updates the values when the program executes.
   * It tests for collisions, buttons pressed and moves the invaders, missiles, and player. 
   * 
   * @retrun Nothing to return.
   */
   void update() override;

   /**
   * @brief This function resets the game to the default values.
   * It re-creates the invaders, resets the score and player lives.
   * 
   * @return Nothing to return.
   */
   void reset();
};