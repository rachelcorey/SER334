#ifndef _HW2_H
#define _HW2_H

/** 
* (A brief description of what the program does)
*
* Completion time: (the time it took you to complete the assignment)
* 
* @author (Your Name), (anyone elses name who's code you used... ie Acuna)
* @version (a version number or date)
*
*/

////////////////////////////////////////////////////////////////////////////////
// INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// MACROS: CONSTANTS
#define MAX_STUDENT_NAME_LENGTH 20
#define MAX_COURSE_NAME_LENGTH 10
#define MAX_TEACHER_NAME_LENGTH 20
#define MAX_COMMENT_LENGTH 20
#define MAX_ASSIGN_NAME_LENGTH 20
#define MAX_FILENAME_LENGTH 30

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef enum AcademicLevel AcademicLevel;
typedef struct ScoreStruct {} ScoreStruct;
typedef struct Assign {} Assign;
typedef struct Student {} Student;
typedef struct Course {} Course;

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
//place to store student information
Student* students = NULL;
//place to store course information
Course* courses = NULL; 
int numCourses, numStudents;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS

// the following should be used to read student/course data in from the file 
void readFile(char* filename);
void readString(FILE* file, char** dst, int max_length);
Student* readStudents(FILE* file);
Course* readCourses(FILE* file);
Assign* readAssigns(FILE* file, int numAssigns);
ScoreStruct** readScores(FILE* file, int numAssigns);

// the following should be used to free all heap data allocated during the programs runtime
// and handle dangling pointers
void terminate();
void studentsDestructor();
void coursesDestructor();
void assignsDestructor(Assign** assigns, int numAssign);
void scoresDestructor(ScoreStruct*** scores, int numAssigns);

// the following should be used to cleanly print the data used in the program 
void printStudents();
void printAssigns(Assign* assigns, int numAssigns);
void printGrades(ScoreStruct** scores, int numAssigns);
void printCourse(Course course);

// the following are mostly complete functions that define and 
// control the CLI menu associated with the program
_Noreturn void mainMenu();
void mainMenuBranch(int option);
void subMenu(Course course);
void subMenuBranch(int option, Course course);

// these are the 'special' functions that you are being asked to implement
_Noreturn void studentMenu(Course course);
void getStudentScores(Course course, int studentNo);

_Noreturn void assignmentMenu(Course course);
void getAssignmentScore(Course course, int assignmentNo);

// this is an optional function that parses and executes instructions defined in an input file
void performInstructions(char* iFile);

// this is a utility function to be called when input filenames are invalid
void printUsage();

/////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATION

/**
* Loads data from student/course data file
* @param filename is the name of the file 
*/
void readFile(char* filename){
  FILE* file = fopen(filename, "r");
  //FOR YOU TO IMPLEMENT
  fclose(file);
}

/**
* Implements main menu functionality, which allows user to select a course to interact with
*/
_Noreturn void mainMenu(){
  int input_buffer;
  printf("Course Searcher");  
  do {
	printf("\n-----------------------------------\n");
    printf("Course Options");
	printf("\n-----------------------------------\n");
	int i;
	for(i = 0; i < numCourses; i++){
      //FOR YOU TO IMPLEMENT
      //printf("   %d %s\n", courses[i].no, courses[i].name);
    }
	printf("   0 REPEAT OPTIONS\n");
    printf("  -1 TERMINATE PROGRAM\n");
	printf("Please enter your choice ---> ");
	scanf(" %d", &input_buffer);
    mainMenuBranch(input_buffer);
  } while (1);
}

/**
* Handles menu options of main menu
* @param option is the chosen operation's option #
*/
void mainMenuBranch(int option){
  if (option < -1 || option > numCourses){
    printf("Invalid input. Please try again...\n");; 
    while(option < -1 || option > numCourses){
  	  printf("Please enter a valid option ---> ");
      scanf(" %d", &option);
    }
  }
  if(option == -1){
	printf("Terminating program...\n");
    terminate();
  } else if (option == 0) {
	printf("Repeating options...\n");
  } else {
    Course course = courses[option - 1];
	//FOR YOU TO IMPLEMENT
    //printf("Course with name %s selected.\n", course.name);
	subMenu(course);
  }
}

/**
* Implements sub menu functionality, which allows users to select how they want to interact with course
* @course is the course to be queried
*/ 

void subMenu(Course course){
  int input_buffer;
  do {
    printf("\n-----------------------------------\n");
    printf("Menu Options");
	printf("\n-----------------------------------\n");
    printf("   1 Get a student's final grades in the course\n");
	printf("   2 Get the average grade of an assignment in the course\n");
    printf("   3 Print all course data\n");
	printf("   0 REPEAT OPTIONS\n");
	printf("  -1 RETURN TO PREVIOUS MENU\n");
    printf("  -2 TERMINATE PROGRAM\n");	
	printf("Please enter your choice ---> ");
	scanf(" %d", &input_buffer);
    subMenuBranch(input_buffer, course);
  } while (input_buffer != -1);
}

/**
* Handles menu options of submenu
* @param option is the chosen operation's option #
* @param course is the course struct to be queried
*/
void subMenuBranch(int option, Course course){
  if (option < -2 || option > 3){
    printf("Invalid input. Please try again...\n");; 
    while(option < -2 || option > 3){
  	  printf("Please enter a valid option ---> ");
      scanf(" %d", &option);
    }
  }
  if(option == -2){
    printf("Terminating program...\n");
	terminate();
  } else if (option == -1){
	printf("Returning to previous menu...\n");
  } else if (option == 0){
    printf("Repeating options...\n");
  } else if (option == 1){
    //FOR YOU TO IMPLEMENT 	
  } else if (option == 2){
    //FOR YOU TO IMPLEMENT
  } else if (option == 3){
    //FOR YOU TO IMPLEMENT
  } 
}


/**
* Prints basic usage instructions for the program to the command line
*/
void print_usage(){
  printf("USAGE:\n./LastNameCourseReader -d <data_file_name(char*)> -c <instruction_file_name(char*)>\n");
  printf("-d refers to the required input data file containing student & course information; this must be a valid .txt file\n");
  printf("-i refers to the optionally supported 'instruction file' that provides directions for how the program should execute without CLI input; \n\t must be a valid .txt.file\n");
}

void terminate(){
  //FREE EVERYTHING HERE
  exit(1);
}

int main(int argc, char* argv[]){
  char* datafile;
  char* instructionfile;
  int opt;
  while((opt = getopt(argc, argv, ":d:i:")) != -1){
    switch(opt){
      case 'd':
        datafile = optarg;
        break;  
      case 'i':
        instructionfile = optarg;
        break;
      case ':':
		printf("option needs a value\n");
		break;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
    }
  }
  for (; optind < argc; optind++){
    printf("Given extra arguments: %s\n", argv[optind]);
  }  

  int dflen;
  if(datafile != NULL){
    dflen = strlen(datafile);
    if(dflen >= 5
		&& (strcmp(&datafile[dflen-4], ".txt") == 0)
        && (access(datafile, F_OK) != -1)){
      printf("Importing data from %s\n\n", datafile);
	  readFile(datafile);
    } else {
	  printf("Data file has an invalid name or does not exist.\n");
      print_usage();
	  exit(1);   
    }
  } else {
    printf("No data file name provided. This is a required field.\n");
    print_usage();
	exit(1);
  }

  int iflen;
  int ifval;
  if(instructionfile != NULL){
    iflen = strlen(instructionfile);
	if(iflen >= 5 
		&& (strcmp(&instructionfile[iflen-4], ".txt") == 0)
		&& (access(instructionfile, F_OK) != -1)){
	  printf("Performing instructions defined in %s:\n", instructionfile);
	  //uncomment below if doing this optional part of the assignment
	  //performInstructions(instructionfile);  	  
	} else {
      printf("Instruction file has an invalid name or does not exist.\n");
	  print_usage();
	  exit(1);
    }
  } else {
	printf("No instruction file provided. Using CLI:\n");
    mainMenu();
  }
  return 0;
}

#endif
