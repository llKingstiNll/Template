#pragma once

// Includes
#include <iostream>
#include <stdlib.h>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "CStudent.h"
#include <fstream>

using namespace std;

// Defines

// Classes
class CCourse
{
private: // Objects
   vector <CStudent> vecStudent;

public:  // Methods
   void print_menu();
   void add_student();
   void edit_student(float student);
   void print_grades();

   void delete_student(float numStudent); 
   void save();
   void load();
   float range();

   string validNum(string studentNumber);
   float validInt(string numStudent);
   string validFloat(string inputValue);

   CCourse() {}
};

