#pragma once
#include "Serial.h"

/**
* 
* @brief Control Class is used for communication with the embedded system
* 
* This class contains the variables and methods to get or set information to the microcontroller
* 
* @author Bailey Hung
* 
*/
class CControl
{
private:
   Serial _com; ///< Serial Com port instance
   int stateBefore1, stateBefore2; ///< Getting the initial states of button 1 and button 2

public:
   /**
   * @brief The Constructor call initilizes the class
   */ 
   CControl();

   /**
   * @brief The Destructor call that removes the class
   */
   ~CControl();

   /**
   * @brief Automatically Tests and Connects to the COM port of the device 
   * 
   * @return Returns a boolean value whether it was able to connect to the COM port
   */
   bool autoDetectCOMPort();

   /** 
   * @brief Initializes the serial port to the com port using an input parameter.
   * The input parameter selects com port to be used.
   * 
   * @param comport The COMM port number selected
   */
   void init_com(int comport);
   
   /**
   * @brief Reads the serial communication protocol andreturns a boolean value.
   * It takes 3 perameters, type, channel, and result.
   * 
   * @param type Selects either digital, analog or servo to be read
   * @param channel Selects channel number
   * @param result The value that is read
   * 
   * @return bool Returns whether get_data ran successfully or not
   */
   bool get_data(int type, int channel, int &result);

   /**
   * @ brief Writes to the serial communication protocol and returns a boolean value.
   * It takes 3 perameters, type, channel, and value.
   * 
   * @param type Selects the digital write
   * @param channel Selects channel number
   * @param result The value that is written23
   * 
   * @return bool Returns whether set_data ran successfully or not
   */
   void set_data(int type, int channel, int result);

   /**
   * @brief Returns a percentage of the analog input using the get_data method
   * 
   * @param channel Selects the channel that is used on the microcontroller
   * @param result Stores analog value from the board
   */
   float get_analog(int channel, int &result);

   /**
   * @brief Reads a digital input anddebounces it for 1 second
   * 
   * @param button Selects the button to debounce
   * @param stateBefore A pointer to the previous state of the button
   * 
   * @return Returns a bool value whether the button has been debounced
   */
   bool get_button(int button);
};