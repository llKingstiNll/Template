// Pong.h
#pragma once
#include "CBase4618.h"
#include "CControl.h"
#include <opencv2/opencv.hpp>

/**
*
* @file.Pong.h
* @brief Child class of CBase that runs Pong
*
* @author Bailey Hung
*
*/
class Pong : public CBase4618
{
private:
   int stateBefore1, stateBefore2; ///< Stores the initial/previous state of button 1 and button 2
   float xdata, ydata, ydata2; ///< Stores the initial/previous state of the analog x value and y value

   int resx, resy; ///< resolution of the canvas

   int ballSpeed, ballSize;
   int ballPosX, ballPosY;
   double ballVelocityX, ballVelocityY;

   int paddleWidth, paddleHeight;
   int paddle1X, paddle1Y;
   int paddle2X, paddle2Y;
   double maxPaddleVelocity;

   int playerScore, opponentScore;

   float fps;
   float fpsAverage;

   std::chrono::steady_clock::time_point lastTime;
   int targetFPS;

public:

   /**
   * @brief Contructor call of Pong used to initialize the class and starting variables
   * 
   * @param The comport used for serial communication
   */
   Pong(int comPort);

   /**
   * @brief Calls the Destructor to delete the instance of the class
   */
   ~Pong();

   /**
   * @brief a virtual memeber of Pong that updates the canvas
   */
   void update() override;

   /**
   * @brief a virtual memeber of Pong that draws the buttons and objects on the canvas
   *
   * @peram canvas
   */
   void draw(cv::Mat &canvas) override;

   void reset();
};