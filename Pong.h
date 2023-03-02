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
   int stateBefore1, stateBefore2; ///< Stores the initial/previous state of button 1 and button 2.
   float xdata, ydata, ydata2; ///< Stores the initial/previous state of the analog x value and y value.

   int resx, resy; ///< x and y resolution of the canvas.

   int ballSpeed, ballSize; ///< Initial ball speed and size.
   int ballPosX, ballPosY; ///< Initial ball x and y position.
   double ballVelocityX, ballVelocityY; ///< Initial ball x and y velocity.

   int paddleWidth, paddleHeight; ///< Setting the paddle width and height.
   int paddle1X, paddle1Y; ///< Initial player paddle position.
   int paddle2X, paddle2Y; ///< Initial opponent paddle position.
   double maxPaddleVelocity; ///< Setting the maximum paddle velocity.

   int playerScore, opponentScore; ///< Player and oppoent score.

   float fps; ///< Float stores the frames per second. 
   float fpsAverage; ///< Averages the last 10 frames per second.

   std::chrono::steady_clock::time_point lastTime; ///< Last time point recorded by the steady clock.
   int targetFPS; ///< The targeted frames per second.

   bool showWinScreen; ///< Boolean value whether or not to show the win screen.

public:

   /**
   * @brief Contructor call of Pong used to initialize the class and starting variables.
   * 
   * @param comPort The comport used for serial communication.
   */
   Pong(int comPort);

   /**
   * @brief Calls the Destructor to delete the instance of the class.
   */
   ~Pong();

   /**
   * @brief A virtual memeber of Pong that updates the canvas.
   * 
   * @return Nothing to retrun.
   */
   void update() override;

   /**
   * @brief A virtual memeber of Pong that draws the buttons and objects on the canvas.
   *
   * @param canvas The cv::Mat canvas on which to draw the object.
   * 
   * @return Nothing to retrun.
   */
   void draw(cv::Mat &canvas) override;

   /**
   * @brief This resets the ball and paddles to their default values.
   * 
   * @return Nothing to retrun.
   */
   void reset();
};