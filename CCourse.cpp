// Includes
#include "stdafx.h"
#include "CCourse.h"
#include "CStudent.h"


// Function Definitions
void CCourse::print_menu()
{
   cout << "*************************************\n"
        << "ELEX4618 Grade System, by Bailey Hung\n"
        << "*************************************\n";

   cout << "(A)dd student\n"
        << "(E)dit student\n"
        << "(P)rint student\n"
        << "(D)elete student\n\n"
        << "(S)ave\n"
        << "(L)oad\n"
        << "(Q)uit\n\n";

   cout << "CMD> ";
}

void CCourse::add_student()
{
   // clear input buffer
   cin.clear();
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   string num, lab, quiz, mid, fin, grade;
   vecStudent.push_back(CStudent(num, lab, quiz, mid, fin, grade));

   edit_student(range()-1);
}

void CCourse::edit_student(float numStudent)
{
   string num, lab, quiz, mid, fin, grade;
   double fgrade;

   printf("\nStudent Number: ");
   getline(cin, num);
   num = validNum(num);

   printf("Lab Grade: ");
   getline(cin, lab);
   lab = validFloat(lab);

   printf("Quiz Grade: ");
   getline(cin, quiz);
   quiz = validFloat(quiz);

   printf("Midterm Grade: ");
   getline(cin, mid);
   mid = validFloat(mid);

   printf("Final Grade: ");
   getline(cin, fin);
   fin = validFloat(fin);

   fgrade = (0.4 * stof(lab)) + (0.1 * stof(quiz)) + (0.2 * stof(mid)) + (0.3 * stof(fin));
   grade = to_string(fgrade);

   cout << endl;
   vecStudent[numStudent]=(CStudent(num, lab, quiz, mid, fin, grade));
}

void CCourse::print_grades()
{
   cout << "#\t"
        << "Student Number\t"
        << "Lab Grade\t"
        << "Quiz Grade\t"
        << "Midterm Grade\t"
        << "Final Grade\t"
        << "Overall Grade\n";

   int numStudent = 1;
   for(auto s : vecStudent)
   {
      cout << setprecision(2);
      cout << numStudent++ << "\t" << setprecision(2)
           << s.studentNumber << "\t"
           << s.labGrade << "\t\t"
           << s.quizGrade << "\t\t"
           << s.midtermGrade << "\t\t"
           << s.finalGrade << "\t\t"
           << s.grade << "\n";
   }
   cout << endl;
}

void CCourse::delete_student(float numStudent)
{
   vecStudent.erase(vecStudent.begin() + numStudent);
   cout << "Student " << (numStudent + 1) << " has been deleted\n" << endl;
}

void CCourse::save()
{
   string fileName;
   cout << "Enter a file name to save the data: ";
   cin >> fileName;
   ofstream outFile;
   outFile.open(fileName);
   if(outFile.fail())
   {
      cerr << "Error opening file\n\n";
      return;
   }
   /*
   for(auto s : vecStudent)
   {
      outFile << s.studentNumber << " " << s.labGrade << " " << s.quizGrade
              << " " << s.midtermGrade << " " << s.finalGrade
              << " " << s.grade << "\n";
      outFile.close();
   }
   */
   cout << range();
   for(int i = 0; i < range(); i++)
   {
      outFile << vecStudent[i].studentNumber << " "
              << vecStudent[i].labGrade << " "
              << vecStudent[i].quizGrade << " "
              << vecStudent[i].midtermGrade << " "
              << vecStudent[i].finalGrade << " "
              << vecStudent[i].grade << endl;
   }
   outFile.close();
   cout << "File Saved!\n\n";
}

void CCourse::load()
{
   string fileName;
   cout << "Enter a file name to load the data: ";
   cin >> fileName;
   ifstream inFile;
   inFile.open(fileName);
   if(inFile.fail())
   {
      cerr << "Error opening file\n\n";
      return;
   }
   string studentNum, labG, quizG, midG, finalG, grade;
   while(inFile >> studentNum >> labG >> quizG >> midG >> finalG >> grade)
   {
      vecStudent.clear();
      CStudent student(studentNum, labG, quizG, midG, finalG, grade);
      vecStudent.push_back(student);
      cout << "File Loaded!\n\n";
   }
   if(!inFile.eof())
   {
      cerr << "File format is not correct\n";
   }
   inFile.close();
}

string CCourse::validNum(string studentNumber)
{
   regex regex_stu("^(A00|A01)[0-9]{6}$");
   while(!regex_match(studentNumber, regex_stu))
   {
      cout << "Invalid Input, Try Again: ";
      getline(cin, studentNumber);
   }
   return studentNumber;
}

float CCourse::validInt(string numStudent)
{
   bool valid = false;
   regex int_regex("^[+-]?[0-9]+$");
   while(1)
   {
      if(regex_match(numStudent, int_regex) && (stof(numStudent) > 0) && (stof(numStudent) < (vecStudent.size()+1)))
      {
         return (stof(numStudent));
      }
      cout << "Enter a valid integer: ";
      getline(cin, numStudent);
   }
}

string CCourse::validFloat(string inputValue)
{
   float num;
   bool valid = false;
   while(!valid)
   {
      stringstream stream(inputValue);
      if(stream >> num && !(stream >> inputValue) && num >= 0 && num <= 100)
      {
         return (inputValue);
      }
      else
      {
         cout << "Please enter a number between 0 and 100: ";
         getline(cin, inputValue);
      }
   }
}

float CCourse::range()
{
   return(vecStudent.size());
}

