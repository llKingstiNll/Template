// CSketch.cpp
#include "stdafx.h"
#include "cvui.h"
#include <opencv2/opencv.hpp>

#include "CSketch.h"
#include "CControl.h"
#include <conio.h>


CSketch::CSketch(int comPort)
{
   // Canvas Size
   resx = 400; //1080
   resy = 400; //1080
   CBase4618::canvas = cv::Mat::zeros(resy, resx, CV_8UC3);
   CBase4618::canvas = cv::Scalar(255, 255, 255);
   CBase4618::exitButton = false;

   scale = 1.4;
   colour = 0;

   // Getting the initial point
   int xpos, ypos;
   xdata = control.get_analog(2, xpos);
   ydata = control.get_analog(26, ypos);

   // Scaling Of Initial Point 
   xdata = ((xdata / 100 * resx) * scale) - ((resx * scale - resx) / 2);
   ydata = (((1 - (ydata / 100)) * resy) * scale) - (((resy * scale) - resy) / 2);
   p1 = cv::Point(xdata, ydata);

   // Getting Starting Button State
   control.get_data(0, 33, stateBefore1);
   control.get_data(0, 32, stateBefore2);

   // Led Default to blue
   control.set_data(0, 37, 1);
   control.set_data(0, 38, 0);
   control.set_data(0, 39, 0);
}

CSketch::~CSketch() {}

void CSketch::draw(cv::Mat &canvas)
{
   // MENU FRAME and TEXT
   // Button menu background 
   cv::rectangle(canvas, cv::Rect(10, 10, 220, 110), cv::Scalar(70, 70, 70), -1);
   
   // Reset Button
   if(cvui::button(canvas, 15, 80, 100, 30, "Reset"))
   {
      // Resetting the Canvis
      canvas = cv::Scalar(255, 255, 255);
   }

   // Exit button
   if(cvui::button(canvas, 125, 80, 100, 30, "Exit"))
   {
      exit(0);
   }

   // Title background
   cv::rectangle(canvas, cv::Rect(10, 10, 220, 30), cv::Scalar(50, 50, 50), -1);

   // Etch-A-Sketch title
   cvui::text(canvas, 15, 15, "Etch-A-Sketch", 0.8, 0xFFFFFF);
   cvui::printf(canvas, 15, 50, 0.6, 0xFFFFFF, "Colour: %s", colourName.c_str());
}

void CSketch::update()
{
   int xpos, ypos;
   float xdata2, ydata2;

   // Drawing the line
   xdata2 = control.get_analog(2, xpos);
   ydata2 = control.get_analog(26, ypos);
   
   // Scaling 
   xdata2 = ((xdata2 / 100 * resx) * scale) - ((resx * scale - resx) / 2);
   ydata2 = (((1 - (ydata2/100)) * resy) * scale) - (((resy * scale) - resy) / 2);

   // Restric to Boundary

   if(xdata2 < 0)
   {
      xdata2 = 0;
   }
   else if(xdata2 > resx)
   {
      xdata2 = resx;
   }
   if(ydata2 < 0)
   {
      ydata2 = 0;
   }
   else if(ydata2 > resy)
   {
      ydata2 = resy;
   }

   // Changing Colours with Button 1
   if(control.get_button(33))
   {
      colour++;
      if(colour > 3)
      {
         colour = 0;
      }
   }

   // Clearing Canvas with Button 2
   if(control.get_button(32))
   {
      canvas = cv::Scalar(255, 255, 255);
   }

   //wait function
   std::chrono::milliseconds duration(10);
   std::this_thread::sleep_for(duration);

   // Changing Colours
   cv::Point p2(xdata2, ydata2);
   std::cout << p2 << "\r";

   if(colour == 0)
   {
      line(canvas, p1, p2, cv::Scalar(255, 0, 0), 10, cv::LINE_AA);
      colourName = "Blue";
      control.set_data(0, 38, 0);
      control.set_data(0, 39, 0);
   }
   else if(colour == 1)
   {
      line(canvas, p1, p2, cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
      colourName = "Green";
      control.set_data(0, 37, 0);
      control.set_data(0, 38, 1);
   }
   else if(colour == 2)
   {
      line(canvas, p1, p2, cv::Scalar(0, 0, 255), 10, cv::LINE_AA);
      colourName = "Red";
      control.set_data(0, 38, 0);
      control.set_data(0, 39, 1);
   }
   else if(colour == 3)
   {
      line(canvas, p1, p2, cv::Scalar(255, 255, 255), 10, cv::LINE_AA);
      colourName = "White";
      control.set_data(0, 37, 1);
      control.set_data(0, 38, 1);
   }
   p1 = p2;
}