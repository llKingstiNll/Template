// CControl.cpp
#include "stdafx.h"
#include "CControl.h"
#include "Serial.h"

using namespace std;

CControl::CControl() 
{
   // COMM port
   int comport;
   cout << "\nEnter the COMM port: ";
   cin >> comport;
   init_com(comport);

   //autoDetectCOMPort();

   // Initial Button State
   get_data(0, 33, stateBefore1);
   get_data(0, 32, stateBefore2);
}
CControl::~CControl() {}

bool CControl::autoDetectCOMPort()
{
   int result;
   for(int i = 1; i <= 10; i++)
   {
      if(_com.open("COM" + to_string(i), CBR_9600))
      {
         cout << "tying COM " << i << "\n";
         set_data(0, 38, 1);
         Sleep(10);

         get_data(0, 38, result);
         cout << result;
         if(result == 1)
         {
            cout << "connected\n";
            return i;
         }
         else
         {
            // Close the wrong port and try the next one
            cout << "failed on COM " << i << "\n";
            //_com.~Serial();
         }
      }
   }
   return 0;
}

// initializing the COM port
void CControl::init_com(int comport)
{
   std::string comsel = "COM" + to_string(comport);
   _com.open(comsel);
}

// getting data
bool CControl::get_data(int type, int channel, int &result)
{
   _com.flush();
   std::string data = "g " + to_string(type) + " " + to_string(channel) + "\n";
   std::string rx_str = "";

   char buff[2];
   
   _com.write(data.c_str(), data.length());
   Sleep(10);

   float start_time = GetTickCount();

   buff[0] = 0;

   while(buff[0] != '\n' && GetTickCount() - start_time < 1000)
   {
      if(_com.read(buff, 1) > 0)
      {
         rx_str = rx_str + buff[0];
      }
   }
   int i = rx_str.find_last_of(' ');
   if((rx_str[0] == 'A' && (cv::getTickCount() - start_time) / cv::getTickCount() < 1.0))
   {
      result = stoi(rx_str.substr(i + 1, rx_str.length() - 1));
      return 1;
   }
   else
   {
      return 0;
   }
}

// setting data
void CControl::set_data(int type, int channel, int val)
{
   _com.flush();
   std::string data = "s " + to_string(type) + " " + to_string(channel) + " " + to_string(val) + "\n";
   _com.write(data.c_str(), data.length());
}

// Getting the percnet value of the analog input
float CControl::get_analog(int channel, int &result)
{
   float percent;
   get_data(1, channel, result);
   percent = (static_cast<float>(result) / 4096)*100;
   return percent;
}

// deboucing the button
bool CControl::get_button(int button)
{
   int currentState;
   bool debounce = false;

   get_data(0, button, currentState);
   if(button == 32)
   {
      if((currentState != stateBefore1))
      {
         if((currentState == 0) && (stateBefore1 == 1))
         {
            debounce = true;
         }
      }
      stateBefore1 = currentState;
   }
   if(button == 33)
   {
      if((currentState != stateBefore2))
      {
         if((currentState == 0) && (stateBefore2 == 1))
         {
            debounce = true;
         }
      }
      stateBefore2 = currentState;
   }
   return debounce;
}