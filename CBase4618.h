//CBase4618.h
#pragma once
#include <opencv2/opencv.hpp>
#include "CControl.h"

/**
*
* @brief CBase4618 Class is used as the parent class that creates the canvas and runs games
*
* This class uses the CControl class to to interact with the microcontroller to get 
* player inputs.
*
* @author Bailey Hung
*
*/
class CBase4618
{
protected:
   cv::Mat canvas; ///< Creates the canvas matrix used to display games
    
   CControl control; ///< Uses the CControl class to use the control functions to send and recieve data from the microcontroller

   /**
   * @brief A flag used to end the run fucntion
   * determins if q or Q was pushed
   * 
   * @return Returns a boolean value whether to stop the run function
   */
   bool exitButton;

public:
   /**
   * @brief The Constructor call initilizes the class and its starting variables
   */
   CBase4618();

   /**
   * @brief The Destructor call destroy the instance the class
   */
   ~CBase4618();

   /**
   * @brief A virtual memeber of CSketch that updates the canvas
   */
   virtual void update() = 0;

   /**
   * @brief A virtual memeber of CSketch that draws the buttons and objects on the canvas
   * 
   * @param canvas The canvas instance used to create visuals 
   */
   virtual void draw(cv::Mat &canvas) = 0;

   /**
   * @brief Runs a loop until the exitButton has been triggered
   */
   void run();
};