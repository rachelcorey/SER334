
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
typedef struct Student Student;
typedef struct Assign Assign;
typedef struct ScoreStruct ScoreStruct;
typedef struct Course Course;

enum AcademicLevel{Freshman, Sophomore, Junior, Senior};

struct Student{
    int id;
    char name[32];
    enum AcademicLevel level;
};

struct Assign{
    int id;
    char name[32];
};

struct ScoreStruct{
    int assignID;
    int studentID;
    double score;
    char comment[64];
};

struct Course{
    int id;
    char courseName[32];
    char instructorName[32];
    int studentsEnrolled;
    int totalAssignments;
    int totalScores;
    struct Assign* assignments;
    struct ScoreStruct* scores;
};

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
void studentMenu(Course course);
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

    // Scan for student data
    fscanf(file, "%d", &numStudents);
    students = malloc(numStudents * sizeof (struct Student));
    for (int i = 0; i < numStudents; ++i) {
        struct Student* student = NULL;
        student = malloc(sizeof (struct Student));
        int sID = 0;
        char sName[32] = "";
        int sLvl = 0;
        fscanf(file, "%d", &sID);
        student->id = sID;
        fscanf(file, "%s", sName);
        strcpy(student->name, sName);
        fscanf(file, "%d", &sLvl);
        student->level = sLvl;
        students[i] = *student;
    }

    // Scan for course data
    fscanf(file, "%d", &numCourses);
    courses = malloc(numCourses * sizeof (struct Course));
    for (int i = 0; i < numCourses; ++i) {
        struct Course* course = NULL;
        course = malloc(sizeof (struct Course));
        course->studentsEnrolled = 0;
        int cID = 0;
        char cName[32] = "";
        char iName[32] = "";
        int assN = 0;
        fscanf(file, "%d", &cID);
        course->id = cID;
        fscanf(file, "%s", cName);
        strcpy(course->courseName, cName);
        fscanf(file,"%s", iName);
        strcpy(course->instructorName, iName);
        fscanf(file, "%d", &assN);
        course->totalAssignments = assN;

        // Scan for assignment data for the course
        course->assignments = malloc(assN * sizeof(Assign));
        for (int j = 0; j < assN; ++j) {
            struct Assign* assignment = NULL;
            assignment = malloc(sizeof(struct Assign));
            int assID = 0;
            char assName[32] = "";

            fscanf(file, "%d", &assID);
            assignment->id = assID;
            fscanf(file, "%s", assName);
            strcpy(assignment->name, assName);
            course->assignments[j] = *assignment;
        }

        // Scan for score data for the course
        int scoresN = numStudents * assN;
        course->totalScores = scoresN;
        course->scores = malloc(scoresN * sizeof (ScoreStruct));
        for (int k = 0; k < scoresN; ++k) {
            struct ScoreStruct* score = NULL;
            score = malloc(sizeof(struct ScoreStruct));
            int assID = 0;
            int sN = 0;
            float scoreP = 0;
            char comm[32] = "";

            fscanf(file, "%d", &assID);
            score->assignID = assID;
            fscanf(file, "%d", &sN);
            score->studentID = sN;
            fscanf(file, "%f", &scoreP);
            score->score = scoreP;
            fscanf(file, "%s", comm);
            strcpy(score->comment, comm);
            course->scores[k] = *score;
        }
        courses[i] = *course;
    }

//    printf("%s \n", courses[0].courseName);
//    printf("%s \n", courses[0].assignments[1].name);
//    printf("%s \n", courses[0].scores[0].comment);


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
            printf("   %d %s\n", courses[i].id, courses[i].courseName);
        }
        printf("   0 REPEAT OPTIONS\n");
        printf("  -1 TERMINATE PROGRAM\n");
        printf("Please enter your choice ---> ");
        scanf(" %d", &input_buffer);
        mainMenuBranch(input_buffer);
    } while (1);
}


void getAssignmentScore(Course course, int assignmentNo) {
    struct Assign *assignment = malloc(sizeof(struct Assign));
    for (int i = 0; i < course.totalAssignments; ++i) {
        if (course.assignments[i].id == assignmentNo) {
            assignment = &course.assignments[i];
        }
    }
    double scoreT = 0;
    double numScores = 0;
    for (int j = 0; j < course.totalScores; ++j) {
        if (course.scores[j].assignID == assignment->id) {
            scoreT += course.scores[j].score;
            ++numScores;
        }
    }
    printf("%s %s %s %g", "The average grade on", assignment->name, "was", scoreT/numScores);
}

void commonOptions(int option) {
    if(option == -2){
        printf("Terminating program...\n");
        terminate();
    } else if (option == -1){
        printf("Returning to previous menu...\n");
    } else if (option == 0){
        printf("Repeating options...\n");
    }
}

/**
* Implements assignment menu functionality, which allows user to select an assignment in the course to interact with
*/
_Noreturn void assignmentMenu(Course course){
    int input_buffer;
    do {
        printf("\n-----------------------------------\n");
        printf("Course Assignments");
        printf("\n-----------------------------------\n");
        printf("Please choose from the following assignments:\n");
        int i;
        for(i = 0; i < course.totalAssignments; i++){
            printf("   %d %s\n", course.assignments[i].id, courses->assignments[i].name);
        }
        printf("   0 REPEAT OPTIONS\n");
        printf("  -1 RETURN TO PREVIOUS MENU\n");
        printf("  -2 TERMINATE PROGRAM\n");
        printf("Please enter your choice ---> ");
        scanf(" %d", &input_buffer);
        if (input_buffer == 0 || input_buffer == -2) {
            commonOptions(input_buffer);
        } else if (input_buffer == -1) {
            commonOptions(input_buffer);
            subMenu(course);
        } else if (input_buffer > 0 && input_buffer <= course.totalAssignments) {
            getAssignmentScore(course, input_buffer);
        } else {
            printf("Please enter a valid option.\n");
        }
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
        printf("Course with name %s selected.\n", course.courseName);
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

    commonOptions(option);

    if (option == 1){
        //FOR YOU TO IMPLEMENT
    } else if (option == 2){
        assignmentMenu(course);
    } else if (option == 3){
        printCourse(course);
    } else if (option == -1) {
        mainMenu();
    }
}



void printCourse(Course course) {
    printf("%s %d \n", "Course ID:" , course.id);
    printf("%s %s \n", "Course Name:" , course.courseName);
    printf("%s %s \n", "Teacher:" , course.instructorName);
    printf("Assigns:\n");
    for (int i = 0; i < course.totalAssignments; ++i) {
        printf("   %d %s \n", course.assignments[i].id , course.assignments[i].name);
    }
    printf("Grade Data:\n");
    for (int i = 0; i < course.totalScores; ++i) {
        printf("   %d %d ", course.scores[i].assignID , course.scores[i].studentID);
        printf("%.0f ", course.scores[i].score);
        printf("%s\n", course.scores[i].comment);
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


void studentsDestructor() {
    for (int i = 0; i < numStudents; ++i) {
        free(&students[i]);
    }
}
void coursesDestructor() {
    for (int i = 0; i < numCourses; ++i) {
        assignsDestructor((Assign **) courses[i].assignments, courses[i].totalAssignments);
    }
}
void assignsDestructor(Assign** assigns, int numAssign) {
    for (int i = 0; i < numAssign; ++i) {
        free(assigns[i]);
    }
}
void scoresDestructor(ScoreStruct*** scores, int numAssigns) {

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






//int main(void) {
//
//    FILE * data;
//    char read [256];
//
//    data = fopen("../Module-2/simple-data.txt","r");
//
//    int base = 0;
//    int numberOf = 0;
//    int id = 0;
//    char name[32];
//
//    fscanf(data, "%d", &numberOf);
//
//    for (int i = 0; i < numberOf; ++i) {
//
//    }
//
//    fclose(data);
//    return 0;
//}