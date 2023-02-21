////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated Dec 2, 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

// Add simple GUI elements
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <conio.h>

#include "Client.h"
#include "Server.h"

//other includes
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <string>
#include <conio.h>
#include <thread>

// Must include Windows.h after Winsock2.h, so Serial must be included after Client/Server
#include "Serial.h" 
#define CANVAS_NAME "Display Image"

using namespace std;

// LAB file Includes
// Lab2
#include "CCourse.h"
#include "CStudent.h"

// Lab3
#include "CControl.h"

// Lab4
#include "CSketch.h"

// Lab5
#include "Pong.h"

////////////////////////////////////////////////////////////////
// Can be used as a replacement for cv::waitKey() to display cv::imshow() images, Windows Only
////////////////////////////////////////////////////////////////
void process_msg()
{
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}

////////////////////////////////////////////////////////////////
// Generate ARUCO markers
////////////////////////////////////////////////////////////////
void generate_marks()
{
  std::string str;
  cv::Mat im;
  int mark_size = 250;

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  for (int marker_id = 0; marker_id < 250; marker_id++)
  {
    cv::aruco::drawMarker(dictionary, marker_id, mark_size, im, 1);
    str = "mark" + std::to_string(marker_id) + ".png";
    cv::imwrite(str, im);
  }
}

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
  // Comport class (change port to your MSP device port)
  Serial com;
  com.open("COM4");

  // TX and RX strings
  std::string tx_str = "G 1 2\n";
  std::string rx_str;

  // temporary storage
  char buff[2];
  do
  {
    // Send TX string
		com.write(tx_str.c_str(), tx_str.length());
    Sleep(10); // wait for ADC conversion, etc. May not be needed?
  
    rx_str = "";
    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
      if (com.read(buff, 1) > 0)
      {
        rx_str = rx_str + buff[0];
      }
    }

    printf ("\nRX: %s", rx_str.c_str());
    cv::waitKey(1);
  } 
  while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
  cv::Mat im;

  // initialize GUI system
  cvui::init(CANVAS_NAME);
  cv::Point gui_position;

  // Load test image
  im = cv::imread("BCIT.jpg");

  // Seed random number generator with time
  srand(time(0));

  // Draw 500 circles and dots on the image
  for (int i = 0; i < 500; i++)
  {
    gui_position = cv::Point(10, 10);
    cvui::window(im, gui_position.x, gui_position.y, 200, 40, "Image Test");
    gui_position += cv::Point(5, 25);
    cvui::text(im, gui_position.x, gui_position.y, "Number of Circles: " + std::to_string(i));

    float radius = 50 * rand() / RAND_MAX;
    cv::Point center = cv::Point(im.size().width*rand() / RAND_MAX, im.size().height*rand() / RAND_MAX);
    
    cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
    
    im.at<char>(i,i) = 255;
    
    // Show image and delay (all display image to update)
    cv::imshow(CANVAS_NAME, im);
    cv::waitKey(1);
  }
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  cv::VideoCapture vid;

  vid.open(0, cv::CAP_DSHOW);
    
  bool do_canny = true;
  bool do_aruco = false;
  int canny_thresh = 30;
  bool do_exit = false;

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  // initialize GUI system
  cvui::init(CANVAS_NAME);
  cv::Point gui_position;
  
  std::vector<cv::Scalar> color_vec;
  color_vec.push_back(cv::Scalar(255, 255, 255));
  color_vec.push_back(cv::Scalar(255, 0, 0));
  color_vec.push_back(cv::Scalar(0, 255, 0));
  color_vec.push_back(cv::Scalar(0, 0, 255));
  int color_index = 0;
  
  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame, edges;

      // Capture video frame
      vid >> frame;

      // Make sure video frame exists
      if (frame.empty() == false)
      { 
        // ARUCO marker tracking
        if (do_aruco == true)
        {
          std::vector<int> ids;
          std::vector<std::vector<cv::Point2f> > corners;
          cv::aruco::detectMarkers(frame, dictionary, corners, ids);
          if (ids.size() > 0)
          {
            cv::aruco::drawDetectedMarkers(frame, corners, ids);
          }
        }

        // Canny edge detection
        if (do_canny == true)
        {
          cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
          cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
          cv::Canny(edges, edges, 0, canny_thresh, 3);
          cv::add(frame, color_vec.at(color_index), frame, edges);
        }

        // GUI Menu
        gui_position = cv::Point(10, 10);
        cvui::window(frame, gui_position.x, gui_position.y, 200, 190, "Video Test");
        gui_position += cv::Point(5, 25);
        cvui::checkbox(frame, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
        gui_position += cv::Point(0, 25);
        cvui::checkbox(frame, gui_position.x, gui_position.y, "ArUco", &do_aruco);
        gui_position += cv::Point(0, 25);
        cvui::text(frame, gui_position.x, gui_position.y, "Canny Threshold");
        gui_position += cv::Point(0, 15);
        cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &canny_thresh, 5, 120);
        gui_position += cv::Point(0, 50);
        if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Colour Switch"))
        {
          color_index++;
          if (color_index >= color_vec.size()) { color_index = 0; }
        }
        gui_position += cv::Point(120, 0);
        if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit"))
        {
          do_exit = true;
        }

        cvui::update();
        cv::imshow(CANVAS_NAME, frame);
      }
    }
    while (cv::waitKey(1) != 'q' && do_exit == false);
  }      
}		

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
void serverthread(CServer* server)
{
  // Start server
  server->start(4618);
}

void do_clientserver()
{
  char inputchar = 0;
  std::vector<std::string> cmds;

  cv::VideoCapture vid;
  CServer server;

  // Start server thread
  std::thread t(&serverthread, &server);
  t.detach();

  vid.open(0);

  while (inputchar != 'q')
  {
    inputchar = cv::waitKey(100);

    server.get_cmd(cmds);
    if (cmds.size() > 0)
    {
      // Process different commands received by the server
      for (int i = 0; i < cmds.size(); i++)
      {
        if (cmds.at(i) == "im")
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Hi there from Server";
          server.send_string(reply);
        }
        else
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Got some other message";
          server.send_string(reply);
        }
      }
    }

		// Update server image with the latest camera image
		if (vid.isOpened() == true)
		{
			cv::Mat frame;
			vid >> frame;
			if (frame.empty() == false)
			{
				imshow("Server Image", frame);
				process_msg();
				server.set_txim(frame);
			}
		}
  };

  server.stop();
  
  Sleep(100);
}

////////////////////////////////////////////////////////////////
// Lab 1
////////////////////////////////////////////////////////////////
// Definitions
#define Q 81   //ascii for Q

// Functions
void print_menu1();
void add_student();
void edit_student(int numStudents);
void print_grades();

// Structs
struct Student
{
   char studentNumber[9];
   float labGrade;
   float quizGrade;
   float midtermGrade;
   float finalGrade;
   float grade;
};
struct Student students[100];

// Variables
float class_list[100];
int nubmer_of_students = 0;
int lab1()
{
   cout << "LAB1\n";
   char input;
   int numStudent;

   // menu
   do
   {
      print_menu1();
      //input = getche();
      cin >> input;
      printf("\n\n");

      // Switch cases
      switch(input)
      {
      case 'A':
         add_student();
         break;

      case 'E':
         printf("\nStudent to edit: ");
         cin >> numStudent;
         edit_student(numStudent);
         break;

      case 'P':
         print_grades();
         break;

      default:
         printf("Enter a vaid input\n\n");

      }
   }
   while(input != Q);

   // end
   return 0;
}

//Function Definitions
void print_menu1(void)
{
   // inital print statements
   printf("****************************************\n");
   printf("ELEX4618 Grade System, by Bailey Hung\n");
   printf("****************************************\n\n");

   printf("(A)dd student\n");
   printf("(E)dit student\n");
   printf("(P)int student\n");
   printf("(Q)uit student\n\n");

}

void add_student(void)
{
   nubmer_of_students++;
   printf("Adding Student\n");
   edit_student(nubmer_of_students);
}

void edit_student(int numStudent)
{

   printf("Student Number: ");
   cin >> students[numStudent].studentNumber;
   printf("Lab Grade: ");
   cin >> students[numStudent].labGrade;
   printf("Quiz Grade: ");
   cin >> students[numStudent].quizGrade;
   printf("Midterm Grade: ");
   cin >> students[numStudent].midtermGrade;
   printf("Final Exam Grade: ");
   cin >> students[numStudent].finalGrade;
   cout << endl;

   students[numStudent].grade = (0.4 * students[numStudent].labGrade) + (0.1 * students[numStudent].quizGrade) + (0.2 * students[numStudent].midtermGrade) + (0.3 * students[numStudent].finalGrade);
}

void print_grades(void)
{
   cout << "#" << "\t" << "Student" << "\t\t" << "Lab" << "\t" << "Quiz" << "\t" << "Midterm" << "\t\t" << "Final Exam" << "\t" << "Final Grade" << endl;
   for(int i = 1; i < (nubmer_of_students + 1); i++)
   {
      cout << i << "\t" << students[i].studentNumber << "\t" << students[i].labGrade << "\t" << students[i].quizGrade << "\t" << students[i].midtermGrade << "\t\t" << students[i].finalGrade << "\t\t" << students[i].grade << endl;
   }
   cout << endl;
}

////////////////////////////////////////////////////////////////
// Lab 2
////////////////////////////////////////////////////////////////
int lab2()
{
   cout << "LAB2\n";
   CCourse cpp;  // class variable
   char input;        // option select input
   string input2, numStudent;      // input for editing student
   float number;

   while(1)
   {
      regex chars("^[AaEePpDdSsLlQq]+$");
      cpp.print_menu();
      cin >> input;

      //getline(cin, input2)
      //while(!(regex_match(input2, chars)))
      //{
      //   cout << "Invalid Input\n";
      //   cin >> input2;
      //} 
      //char input = input2;

      switch(input)
      {
      case 'A':
      case 'a':
         cpp.add_student();
         break;

      case 'E':
      case 'e':
         if(cpp.range() == 0)
         {
            cout << "You have no students to edit.\n\n";
         }
         else
         {
            cout << "\nStudent to edit: ";

            // clear input buffer
            fflush(stdin);

            getline(cin, numStudent);
            number = cpp.validInt(numStudent);
            cpp.edit_student(number - 1);
         }
         break;


      case 'P':
      case 'p':
         cpp.print_grades();
         break;

      case 'D':
      case 'd':
         if(cpp.range() == 0)
         {
            cout << "You have no students to Delete.\n\n";
         }
         else
         {
            cout << "\nStudent to Delete: ";

            // clear input buffer
            fflush(stdin);

            getline(cin, numStudent);
            number = cpp.validInt(numStudent);
            cpp.delete_student(number - 1);
         }
         break;

      case 'S':
      case 's':
         cpp.save();
         break;

      case 'L':
      case 'l':
         cpp.load();
         break;

      case 'Q':
      case 'q':
         return 0;

      default:
         cout << "Enter a valid input\n\n";

      }
   }
}

////////////////////////////////////////////////////////////////
// Lab 3
////////////////////////////////////////////////////////////////
void lab3()
{
   CControl control;

   char option;
   int xpos, ypos, val;
   float xpercent, ypercent;
   bool quit = true;

   //main 
   while (quit)
   {
      cout << "\nChoose a funtion\n"
         << "(A)nalog Test\n"
         << "(D)igital Test\n"
         << "(B)utton Test\n"
         << "(S)ervo Test\n"
         << "(Q)uit\n\n"
         << "CMD> ";

      cin >> option;

      if(option == 'A' || option == 'a')
      {
         char c;
         float dataoutx, dataouty;
         int rawx, rawy;
         while(!_kbhit())
         {
            //while code
            dataoutx = control.get_analog(2, xpos);
            dataouty = control.get_analog(26, ypos);
            control.get_data(1, 2, rawx);
            control.get_data(1, 26, rawy);
            cout << fixed << setprecision(2)
               << setw(12) << "X position: "
               << setw(4) << (rawx)
               << setw(1) << "("
               << setw(6) << dataoutx
               << setw(2) << "%)"
               << "\t"
               << setw(12) << "Y position: "
               << setw(4) << (rawy)
               << setw(1) << "("
               << setw(6) << dataouty 
               << setw(2) << "%)" << "\r";

            //wait function
            chrono::milliseconds duration(10);
            this_thread::sleep_for(duration);
         }
         cout << "\n";
      }

      if(option == 'D' || option == 'd')
      {
         int button, readdata, writedata, value, setLED;
         cout << "Button to test: ";
         cin >> button;

         if(button == 1)
         {
            button = 33;
         }
         if(button == 2)
         {
            button = 32;
         }

         while(!_kbhit())
         {
            //while code
            control.get_data(0, button, value);
            cout << value << "\n";
            if(value == 0)
            {
               control.set_data(0, 37, 1);
            }
            if(value == 1)
            {
               control.set_data(0, 37, 0);
            }

            //wait function
            chrono::milliseconds duration(1);
            this_thread::sleep_for(duration);
         }

      }

      if(option == 'B' || option == 'b')
      {
         int button;
         //int stateBefore = 0;
         int countButton = 0;
         int debounce = 0;

         cout << "What button to debounce: ";
         cin >> button;
         if(button == 1)
         {
            button = 33;
         }
         if(button == 2)
         {
            button = 32;
         }

         //control.get_data(0, button, stateBefore1);
         cout << "BUTTON TEST: 0\n";

         while(!_kbhit())
         {
            if(control.get_button(button))
            {
               countButton++;
               cout << "BUTTON TEST: " << countButton << "\n";
            }
            
            //wait function
            chrono::milliseconds duration(10);
            this_thread::sleep_for(duration);
         }
      }

      if(option == 'S' || option == 's')
      {
         while(!_kbhit())
         {
            /*
            int servoset;
            cout << "Servo: ";
            cin >> servoset;
            control.set_data(2, 0, servoset);
            */

            chrono::microseconds duration(7);

            for(int angle = 0; angle < 180; angle++)
            {
               control.set_data(2, 0, angle);
               cout << angle << "\n";
               this_thread::sleep_for(duration);
            }
            
            for(int angle = 180; angle > 0; angle--)
            {
               control.set_data(2, 0, angle);
               cout << angle << "\n";
               this_thread::sleep_for(duration);
            }
         }
      }

      if(option == 'Q' || option == 'q')
      {
         quit = false;
      }
   }
}

////////////////////////////////////////////////////////////////
// Lab 4
////////////////////////////////////////////////////////////////
int lab4()
{
   CSketch sketch(0);
   sketch.run();
   return 0;
}

////////////////////////////////////////////////////////////////
// Lab 5
////////////////////////////////////////////////////////////////
int lab5()
{
   Pong pong(0);
   pong.run();
   return 0;
}

////////////////////////////////////////////////////////////////
// Lab 6
////////////////////////////////////////////////////////////////
void lab6()
{
}

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Template Project";
	std::cout << "\n***********************************";
   std::cout << "\n(1) Lab 1 - User Input";
   std::cout << "\n(2) Lab 2 - Grading";
   std::cout << "\n(3) Lab 3 - Embedded Control";
   std::cout << "\n(4) Lab 4 - Etch-A-Sketch";
   std::cout << "\n(5) Lab 5 - Pong";
   std::cout << "\n(6) Lab 6 - Classic Arcade Game";
   std::cout << "\n(7) Lab 7 - Linux Port";
   std::cout << "\n(8) Lab 8 - Sorting";
   std::cout << "\n(9) Lab 9 - Sockets";
   std::cout << "\n(10) Test serial COM communication";
	std::cout << "\n(11) Show image manipulation";
	std::cout << "\n(12) Show video manipulation";
	std::cout << "\n(13) Test client/server communication";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

int main(int argc, char* argv[])
{
  generate_marks();
	int cmd = -1;

	do
	{
		print_menu();
		std::cin >> cmd;
		switch (cmd)
		{break;
    case 1: lab1(); break;
    case 2: lab2(); break;
    case 3: lab3(); break;
    case 4: lab4(); break;
    case 5: lab5(); break;
    case 6: lab6(); break;
    case 10: test_com(); break;
	 case 11: do_image(); break;
	 case 12: do_video(); break;
    case 13: do_clientserver(); break;
		}
	} while (cmd != 0);
}
