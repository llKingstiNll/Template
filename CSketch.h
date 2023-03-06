// CSketch.h
#pragma once
#include "CBase4618.h"
#include "CControl.h"
#include <opencv2/opencv.hpp>

/**
* 
* @class CSketch
* 
* @brief Child class of CBase that runs the Etch-A-Sketch 
* 
* @author Bailey Hung
* 
*/
class CSketch : public CBase4618
{
private:
   int stateBefore1, stateBefore2; ///< Stores the initial/previous state of button 1 and button 2.
   float xdata, ydata; ///< Stores the initial/previous state of the analog x value and y value.
   int colour; ///< Stores a number that corresponds to colour that is used in the sketch.

   std::string colourName; ///< Name of the colour respect to the colour number.
   cv::Point p1; ///< Initial starting point of the analog data.

   int resx; ///< Resolution of the canvas width.
   int resy; ///< Resolution of the canvas height.

   int adc; ///< Max value of the joystick adc.
   float scale; ///< Scale of the joystick circle that covers all corners.

public:
   
   /**
   * @brief Contructor call of CSketch used to initialize the class and starting variables.
   * 
   * @param The COMM port used for serial comunication.
   */
   CSketch(int comPort);

   /**
   * @brief Calls the Destructor to delete the instance of the class.
   */
   ~CSketch();

   /**
   * @brief A virtual memeber of CSketch that updates the canvas.
   * 
   * @return Nothing to retrun.
   */
   void update() override;

   /**
   * @brief a virtual memeber of CSketch that draws the buttons and objects on the canvas.
   *
   * @param canvas The cv::Mat canvas on which to draw the object.
   * @return Nothing to retrun.
   */
   void draw(cv::Mat& canvas) override;
};
