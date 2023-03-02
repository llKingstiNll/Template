// Pong.cpp
#include "stdafx.h"
#include "cvui.h"
#include <opencv2/opencv.hpp>

#include "Pong.h"
#include "CControl.h"
#include <conio.h>
#include <chrono>
#include <thread>

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Pong::Pong(int comPort)
{
   resx = 1000; //1000
   resy = 800; //800
   CBase4618::canvas = cv::Mat::zeros(resy, resx, CV_8UC3);
   CBase4618::canvas = cv::Scalar(0, 0, 0);
   CBase4618::exitButton = false;

   showWinScreen = false;

   // Analog Start State
   int ypos;
   ydata = control.get_analog(26, ypos);
   ydata = (1 - (ydata / 100));
   ydata2 = 0;
   
   // Max paddle velocity
   maxPaddleVelocity = 60;

   // Getting Starting Button State
   control.get_data(0, 33, stateBefore1);
   control.get_data(0, 32, stateBefore2);

   // Turn off LED
   control.set_data(0, 37, 0);
   control.set_data(0, 38, 0);
   control.set_data(0, 39, 0);

   // Score default
   playerScore = 0;
   opponentScore = 0;

   // Reset
   ballSize = 5;
   ballSpeed = 100;

   // Resetting ball and paddle values
   reset();
}

Pong::~Pong() {}

void Pong::draw(cv::Mat &canvas)
{
   // White line in centre
   cv::line(canvas, cv::Point(canvas.cols / 2, 0), cv::Point(canvas.cols / 2, canvas.rows), cv::Scalar(255, 255, 255), 1);

   // Score
   cvui::printf(canvas, 300, 25, 1.2, 0xFFFFFF, "%d", (playerScore));
   cvui::printf(canvas, canvas.cols - 300, 25, 1.2, 0xFFFFFF, "%d", (opponentScore));

   if (playerScore >= 5)
   {
      showWinScreen = true;
   }
   if (opponentScore >= 5)
   {
      showWinScreen = true;
   }
   if ((playerScore | opponentScore) < 5) 
   {
      showWinScreen = false;
   }

   if (showWinScreen) 
   {
      cv::rectangle(canvas, cv::Rect(0, 0, resx, resy), cv::Scalar(20, 20, 20), -1);
       
      if (playerScore >= 5)
      {
         cvui::text(canvas, resx / 2 - 200, resy / 2, "You Win", 3, 0xFFFFFF);
      }
      else
      {
         cvui::text(canvas, resx / 2 - 200, resy / 2, "You Lose", 3, 0xFFFFFF);
      }
      ballVelocityX = 0;
      ballVelocityY = 0;
   }
   // MENU FRAME and TEXT
   // Button menu background 
   cv::rectangle(canvas, cv::Rect(10, 10, 220, 210), cv::Scalar(70, 70, 70), -1);

   // BUTTONS
   // Reset Button
   if (cvui::button(canvas, 15, 180, 100, 30, "Reset"))
   {
      // Resetting the Canvis
      reset();
      playerScore = 0;
      opponentScore = 0;
      ballSize = 5;
      ballSpeed = 100;
   }

   // Exit button
   if (cvui::button(canvas, 125, 180, 100, 30, "Exit"))
   {
      exit(0);
   }

   // TRACKBAR
   // Ball size
   cvui::text(canvas, 80, 60, "Ball Size", 0.5, 0xFFFFFF);
   cvui::trackbar(canvas, 15, 60, 200, &ballSize, (int)5, (int)100);

   // Ball speed
   cvui::text(canvas, 75, 120, "Ball Speed", 0.5, 0xFFFFFF);
   cvui::trackbar(canvas, 15, 120, 200, &ballSpeed, (int)100, (int)400);

   // LETTERS AND TEXT
   // Title background
   cv::rectangle(canvas, cv::Rect(10, 10, 220, 30), cv::Scalar(50, 50, 50), -1);

   // Pong title
   cvui::text(canvas, 15, 15, "Pong", 0.8, 0xFFFFFF);

   // FPS
   cvui::printf(canvas, 120, 18, 0.6, 0xFFFFFF, "FPS: %.2f", (fpsAverage));
}

void Pong::update()
{
   // Start timer
   auto startTime = std::chrono::system_clock::now();
   auto endTime = std::chrono::system_clock::now() + std::chrono::milliseconds(27);

   // Clear canvas
   canvas = cv::Scalar(0, 0, 0);

   // PADDLE CONTROLES AND DYNAMICS
   int posy;
   float ydata;
   // Player paddle
   ydata = control.get_analog(26, posy);
   if(ydata >= 60.0)
   {
      ydata = maxPaddleVelocity * (-(ydata-50) / 100);
   }
   else if(ydata <= 40)
   {
      ydata = maxPaddleVelocity * (((100 - ydata)-50) / 100);
   }
   else ydata = 0.0;

   std::cout << ydata << "\r";

   paddle1Y += ydata;

   // Opponent paddle
   int distance = ballPosY - (paddle2Y + paddleHeight / 2);
   int maxSpeed = min(10, abs(distance) / 10 + 2);
   if (((paddle2Y + paddleHeight / 2) >= ballPosY))
   {
      if (abs(distance) > 20) {
         ydata2 -= maxSpeed * 1.5;
      }
      else {
         ydata2 -= maxSpeed;
      }
   }
   else if ((paddle2Y + paddleHeight) <= ballPosY)
   {
      if (abs(distance) > 20) {
         ydata2 += maxSpeed * 1.5;
      }
      else {
         ydata2 += maxSpeed;
      }
   }
   else
   {
      ydata2 = 0;
   }
   paddle2Y += ydata2;

   // Paddle boundaries
   if(paddle1Y < 0)
   {
      paddle1Y = 0;
   }
   else if(paddle1Y > (resy - paddleHeight))
   {
      paddle1Y = resy - paddleHeight;
   }
   if(paddle2Y < 0)
   {
      paddle2Y = 0;
   }
   else if(paddle1Y > (resy - paddleHeight))
   {
      paddle2Y = resy - paddleHeight;
   }

   // MOVING THE BALL
   ballPosX += ballVelocityX * ballSpeed / 30;
   ballPosY += ballVelocityY * ballSpeed / 30;

   // BOUNCE OFF WALLS
   if(ballPosX <= ballSize || ballPosX >= canvas.cols - ballSize)
   {
      ballVelocityX = -ballVelocityX;
   }
   if(ballPosY <= ballSize || ballPosY >= canvas.rows - ballSize)
   {
      ballVelocityY = -ballVelocityY;
   }

   // BOUNCE OFF PADDLES
   // Paddle 1
   if(((paddle1X + paddleWidth) >= (ballPosX - ballSize)) && (paddle1X <= ballPosX + ballSize) && (paddle1Y <= ballPosY) && ((paddle1Y + paddleHeight) >= ballPosY))
   {
      ballVelocityX = -ballVelocityX + rand() % 5 - 2;
      ballVelocityY += ydata/15;
   }
   // Paddle 2
   if(((paddle2X - paddleWidth) <= (ballPosX + ballSize)) && ((paddle2X + paddleWidth) >= (ballPosX - ballSize)) && (paddle2Y <= ballPosY) && ((paddle2Y + paddleHeight) >= ballPosY))
   {
      ballVelocityX = -ballVelocityX + rand() % 5 - 2;
      ballVelocityY += rand() % 5 - 2;
   }

   // WIN DETECTION
   if (ballPosX < 25) 
   {
      opponentScore++;
      reset();
   }
   if (ballPosX > (canvas.cols - 25)) {
      playerScore++;
      reset();
   }

   // Drawing the objects
   cv::rectangle(CBase4618::canvas, cv::Rect(paddle1X, paddle1Y, paddleWidth, paddleHeight), cv::Scalar(255, 255, 255), -1);
   cv::rectangle(CBase4618::canvas, cv::Rect(paddle2X - paddleWidth, paddle2Y, paddleWidth, paddleHeight), cv::Scalar(255, 255, 255), -1);
   
   cv::circle(CBase4618::canvas, cv::Point(ballPosX, ballPosY), ballSize, cv::Scalar(255, 255, 255), -1);

   // Clearing Canvas with Button 2
   if(control.get_button(32))
   {
      // Default ball values
      reset();
      playerScore = 0;
      opponentScore = 0;
      ballSize = 5;
      ballSpeed = 100;
   }

   // WAIT FUNCTION
   std::this_thread::sleep_until(endTime);

   // FPS
   auto timeFinished = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = timeFinished - startTime;
   fps = 1.0 / elapsed_seconds.count();

   // Average FPS
   const int n = 10;
   static double fpsValues[n] = {};

   for(int i = 0; i < n - 1; i++)
   {
      fpsValues[i] = fpsValues[i + 1];
   }
   fpsValues[n - 1] = fps;

   // Calculate the rolling average of the fps values
   double fps_sum = 0;
   for(int i = 0; i < n; i++)
   {
      fps_sum += fpsValues[i];
   }
   fpsAverage = fps_sum / n;
}

void Pong::reset()
{
   canvas = cv::Scalar(0, 0, 0);

   // Default ball values

   ballPosX = resx / 2;
   ballPosY = resy / 2;

   // Radom Values
   int leftOrRight = (std::rand() % 2 == 0) ? 1 : -1;
   ballVelocityX = (leftOrRight) * (ballSpeed / 30.0);
   ballVelocityY = rand() % 5 - 2;

   // Default paddle values
   paddleWidth = 10;
   paddleHeight = 100;

   paddle1X = 30;
   paddle1Y = (resy / 2) - (paddleHeight / 2);

   paddle2X = resx - paddle1X;
   paddle2Y = (resy / 2) - (paddleHeight / 2);
}