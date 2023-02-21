// CBase4618.cpp
#include "stdafx.h"
#include "CBase4618.h"
#include <opencv2/opencv.hpp>
#include "cvui.h"

CBase4618::CBase4618()
{
   exitButton = false;
}

CBase4618::~CBase4618() {}

void CBase4618::run()
{
   cvui::init("Canvas");

   while(!exitButton)
   {
      update();
      draw(canvas);
      cvui::imshow("Canvas", canvas);

      int inputKey = cv::waitKey(1);
      if(inputKey == 'q' || inputKey == 'Q')
      {
         exitButton = true;
         cv::destroyAllWindows();
         break;
      }
   }
   cv::destroyAllWindows();
}

