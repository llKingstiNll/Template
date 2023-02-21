#pragma once

// Includes
#include <string>
using namespace std;

class CStudent
{
public:
   string studentNumber;
   string labGrade;
   string quizGrade;
   string midtermGrade;
   string finalGrade;
   string grade;

   CStudent(string studentNum, string labGr, string quizGr, string midtermGr, string finalGr, string gr) :
      studentNumber(studentNum), labGrade(labGr), quizGrade(quizGr), midtermGrade(midtermGr), finalGrade(finalGr), grade(gr) {}
};
