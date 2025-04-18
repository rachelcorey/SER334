/**
* This program reads and parses a data file of students, courses, assignments and scores.
* The data can be viewed through the console menu options.
* An optional function is to load a set of instructions that defines which data to print,
* without having to navigate the menus. See readme.txt for more information.
*
* Completion time: 6.5 hours
*
* @author Rachel Corey, Whoever Wrote BaseReader.c on Canvas
* @version 1.0, completed on 10/19/2021
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

enum AcademicLevel {
    None, Freshman, Sophomore, Junior, Senior
};

struct Student {
    int id;
    char name[MAX_STUDENT_NAME_LENGTH];
    enum AcademicLevel level;
};

struct Assign {
    int id;
    char name[MAX_ASSIGN_NAME_LENGTH];
};

struct ScoreStruct {
    int assignID;
    int studentID;
    double score;
    char comment[MAX_COMMENT_LENGTH];
};

struct Course {
    int id;
    char courseName[MAX_COURSE_NAME_LENGTH];
    char instructorName[MAX_TEACHER_NAME_LENGTH];
    int studentsEnrolled;
    int totalAssignments;
    int totalScores;
    struct Student *studentRoster;
    struct Assign *assignments;
    struct ScoreStruct *scores;
};

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES
//place to store student information
Student *students = NULL;
//place to store course information
Course *courses = NULL;
int numCourses, numStudents;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS

// the following should be used to read student/course data in from the file
void readFile(char *filename);

void readString(FILE *file, char **dst, int max_length);

Student *readStudents(FILE *file);

Course *readCourses(FILE *file);

Assign *readAssigns(FILE *file, int numAssigns);

ScoreStruct **readScores(FILE *file, int numAssigns);

// the following should be used to free all heap data allocated during the programs runtime
// and handle dangling pointers
void terminate();

void studentsDestructor();

void coursesDestructor();

void assignsDestructor(Assign **assigns, int numAssign);

void scoresDestructor(ScoreStruct ***scores, int numAssigns);

// the following should be used to cleanly print the data used in the program
void printStudents();

void printAssigns(Assign *assigns, int numAssigns);

void printGrades(ScoreStruct **scores, int numAssigns);

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

void assignmentMenu(Course course);

void getAssignmentScore(Course course, int assignmentNo);

// this is an optional function that parses and executes instructions defined in an input file
void performInstructions(char *iFile);

// this is a utility function to be called when input filenames are invalid
void printUsage();

/////////////////////////////////////////////////////////////////////////////////
//IMPLEMENTATION

/**
 * Helper function that gets a specified assignment by its ID
 * @param course that the assignment is part of
 * @param id the assignmentID
 * @return the Assignment struct object
 */
Assign *getAssignmentByID(Course course, int id) {
    for (int i = 0; i < course.totalAssignments; ++i) {
        if (course.assignments[i].id == id) {
            return &course.assignments[i];
        }
    }
    struct Assign *a = NULL;
    return a;
}

/**
 * Helper function that gets a specified student by their ID
 * @param id the studentID
 * @return the Student struct object
 */
Student *getStudentByID(int id) {
    for (int i = 0; i < numStudents; ++i) {
        if (students[i].id == id) {
            return &students[i];
        }
    }
    struct Student *s = NULL;
    return s;
}

/**
 * Helper function that gets a course based on its ID
 * @param id the courseID
 * @return the Course struct object
 */
Course *getCourseByID(int id) {
    for (int i = 0; i < numCourses; ++i) {
        if (courses[i].id == id) {
            return &courses[i];
        }
    }
    struct Course *c = NULL;
    return c;
}

/**
 * Helper function that retrieves a grade on a specified assignment for the specified student and course
 * @param course the course for which the assignment belongs to
 * @param aID the assignmentID
 * @param sID the studentID
 * @return the Student's grade on the specified assignment
 */
double getStudentsAssnGrade(Course course, int aID, int sID) {
    for (int i = 0; i < course.totalScores; ++i) {
        if (course.scores[i].assignID == aID && course.scores[i].studentID == sID) {
            return course.scores[i].score;
        }
    }
    return -1;
}

/**
 * This function opens and parses an instruction file that tells the program what kind of information
 * to print out, without needing to navigate the console menus to access it
 * @param filename the name of the instruction file
 */
void performInstructions(char *filename) {
    FILE *file = fopen(filename, "r");
    int iNum = 0;

    // number of instructions
    fscanf(file, "%d", &iNum);
    for (int i = 0; i < iNum; ++i) {
        int courseID = 0;
        fscanf(file, "%d", &courseID);
        int studentID = 0;
        fscanf(file, "%d", &studentID);
        int assignmentID = 0;
        fscanf(file, "%d", &assignmentID);

        printf("\n%s %d, %s %d, %s %d\n\n", "Processing instruction with courseNum", courseID, "studentNum", studentID,
               "and assignmentNum", assignmentID);

        if (courseID > 0) {
            if (studentID != 0 || assignmentID != 0) {
                if (studentID != 0 && assignmentID != 0) {
                    Student *s = getStudentByID(studentID);
                    Course *c = getCourseByID(courseID);
                    Assign *a = getAssignmentByID(*c, assignmentID);
                    printf("%s %s %s %1.2f %s %s\n\n", "Student with name", s->name, "received a",
                           getStudentsAssnGrade(*c, assignmentID, studentID), "on assignment",
                           a->name);
                } else if (studentID != 0) {
                    getStudentScores(*getCourseByID(courseID), studentID);
                } else if (assignmentID != 0) {
                    getAssignmentScore(*getCourseByID(courseID), assignmentID);
                }
            } else {
                printCourse(*getCourseByID(courseID));
            }
        } else {
            printf("Please define a valid course to read.\n");
        }
    }
    fclose(file);
    terminate();
}

/**
* Loads data from student/course data file
* @param filename is the name of the file
*/
void readFile(char *filename) {
    FILE *file = fopen(filename, "r");

    fscanf(file, "%d", &numStudents);
    students = malloc(numStudents * sizeof(struct Student));

    for (int i = 0; i < numStudents; ++i) {
        // Scan for student data
        struct Student *student = NULL;
        student = malloc(sizeof(struct Student));
        int sID = 0;
        char sName[MAX_STUDENT_NAME_LENGTH] = "";
        int sLvl = 0;
        fscanf(file, "%d", &sID);
        student->id = sID;
        fscanf(file, "%s", sName);
        strcpy(student->name, sName);
        fscanf(file, "%d", &sLvl);
        student->level = sLvl;
        students[i] = *student;
        free(student);
    }

    // Scan for course data
    fscanf(file, "%d", &numCourses);
    courses = malloc(numCourses * sizeof(struct Course));
    for (int i = 0; i < numCourses; ++i) {
        struct Course *course = NULL;
        course = malloc(sizeof(struct Course));
        course->studentsEnrolled = 0;
        int cID = 0;
        char cName[MAX_COURSE_NAME_LENGTH] = "";
        char iName[MAX_TEACHER_NAME_LENGTH] = "";
        int assN = 0;
        fscanf(file, "%d", &cID);
        course->id = cID;
        fscanf(file, "%s", cName);
        strcpy(course->courseName, cName);
        fscanf(file, "%s", iName);
        strcpy(course->instructorName, iName);
        fscanf(file, "%d", &assN);
        course->totalAssignments = assN;

        // Scan for assignment data for the course
        course->assignments = malloc(assN * sizeof(Assign));
        for (int j = 0; j < assN; ++j) {
            struct Assign *assignment = NULL;
            assignment = malloc(sizeof(struct Assign));
            int assID = 0;
            char assName[MAX_ASSIGN_NAME_LENGTH] = "";

            fscanf(file, "%d", &assID);
            assignment->id = assID;
            fscanf(file, "%s", assName);
            strcpy(assignment->name, assName);
            course->assignments[j] = *assignment;
            free(assignment);
        }

        // Scan for score data for the course
        int scoresN = numStudents * assN;
        course->totalScores = scoresN;
        course->scores = malloc(scoresN * sizeof(ScoreStruct));
        for (int k = 0; k < scoresN; ++k) {
            struct ScoreStruct *score = NULL;
            score = malloc(sizeof(struct ScoreStruct));
            int assID = 0;
            int sN = 0;
            float scoreP = 0;
            char comm[MAX_COMMENT_LENGTH] = "";

            fscanf(file, "%d", &assID);
            score->assignID = assID;
            fscanf(file, "%d", &sN);
            score->studentID = sN;
            fscanf(file, "%f", &scoreP);
            score->score = scoreP;
            fscanf(file, "%s", comm);
            strcpy(score->comment, comm);
            course->scores[k] = *score;
            free(score);
        }

        course->studentRoster = NULL;
        course->studentRoster = malloc(numStudents * sizeof(struct Student));
        int idx = 0;
        for (int j = 0; j < course->totalScores; ++j) {
            for (int k = 0; k < course->totalAssignments; ++k) {
                if (course->scores[j].assignID == course->assignments[k].id) {
                    int stExists = 0;
                    for (int l = 0; l < idx; ++l) {
                        if (course->scores[j].studentID == course->studentRoster[l].id) {
                            stExists = 1;
                        }
                    }
                    if (stExists == 0) {
                        course->studentRoster[idx] = *getStudentByID(course->scores[j].studentID);
                        ++idx;
                    }
                }
            }
        }
        course->studentsEnrolled = idx;
        courses[i] = *course;
        free(course);
    }

    fclose(file);
}

/**
 * Destructor to free the Students struct collection
 */
void studentsDestructor() {
    free(&students[0]);
}

/**
 * Destructor to free the Students struct collection in each course
 * @param st the address of the Students struct collection
 */
void rosterDestructor(Student **st) {
    free(&st[0]);
}

/**
 * Destructor to free the Course structs collection, as well as all the collections
 * contained within each one: its Roster, Assignments, and Scores.
 */
void coursesDestructor() {
    for (int i = 0; i < numCourses; ++i) {
        rosterDestructor((Student **) courses[i].studentRoster);
        assignsDestructor((Assign **) courses[i].assignments, courses[i].totalAssignments);
        scoresDestructor((ScoreStruct ***) courses[i].scores, courses[i].totalScores);
    }
    free(&courses[0]);
}

/**
 * Destructor to free the Assignment struct collection within a course
 * @param assigns the address to the Assignment struct collection
 * @param numAssign the number of assignments (? I didn't need this....)
 */
void assignsDestructor(Assign **assigns, int numAssign) {
    free(&assigns[0]);
}

/**
 * Destructor to free the Scores struct collection within a course
 * @param scores the address to the Score struct collection
 * @param numAssigns the number of assignments (? I didn't need this to free the memory...)
 */
void scoresDestructor(ScoreStruct ***scores, int numAssigns) {
    free(&scores[0]);
}

/**
 * Function to call the destructors to free the allocated memory and exit the program safely
 */
void terminate() {
    printf("\n");
    studentsDestructor();
    coursesDestructor();
    exit(1);
}

/**
* Implements main menu functionality, which allows user to select a course to interact with
*/
_Noreturn void mainMenu() {
    int input_buffer;
    printf("Course Searcher");
    do {
        printf("\n-----------------------------------\n");
        printf("Course Options");
        printf("\n-----------------------------------\n");
        int i;
        for (i = 0; i < numCourses; i++) {
            printf("   %d %s\n", courses[i].id, courses[i].courseName);
        }
        printf("   0 REPEAT OPTIONS\n");
        printf("  -1 TERMINATE PROGRAM\n");
        printf("Please enter your choice ---> ");
        scanf(" %d", &input_buffer);
        mainMenuBranch(input_buffer);
    } while (1);
}

/**
 * This function prints the average score for a chosen assignment in a specified course
 * @param course the course that the assignment is in
 * @param assignmentNo the assignmentID
 */
void getAssignmentScore(Course course, int assignmentNo) {
    int assnID = 0;
    char name[32] = "";
    for (int i = 0; i < course.totalAssignments; ++i) {
        if (course.assignments[i].id == assignmentNo) {
            assnID = course.assignments[i].id;
            strcpy(name, course.assignments[i].name);
        }
    }
    double scoreT = 0;
    double numScores = 0;
    for (int j = 0; j < course.totalScores; ++j) {
        if (course.scores[j].assignID == assnID) {
            scoreT += course.scores[j].score;
            ++numScores;
        }
    }
    printf("\n%s %s %s %1.2f\n", "The average grade on", name, "was", scoreT / numScores);
}

/**
 * Refactored helper function that allows several functions to call it if the user's option was a common
 * function to many menus
 * @param option the user's option input
 */
void commonOptions(int option) {
    if (option == -2) {
        printf("Terminating program...\n");
        terminate();
    } else if (option == -1) {
        printf("Returning to previous menu...\n");
    } else if (option == 0) {
        printf("Repeating options...\n");
    }
}

/**
 * Helper function that returns a String value based on the enum index
 * @param lvl the level of the student
 * @return a string representation of the student's level
 */
char *getStudentLevel(enum AcademicLevel lvl) {
    switch (lvl) {
        case Freshman:
            return "(Freshman)";
        case Sophomore:
            return "(Sophomore)";
        case Junior:
            return "(Junior)";
        case Senior:
            return "(Senior)";
        default:
            return "";
    }
}

/**
 * Helper function for sortScores_SID_AID to swap two scores in the scores collection for the course
 * @param score1 the score to be swapped into score2's address
 * @param score2 the score that's swapped into score1's address
 */
void swapScores(struct ScoreStruct *score1, struct ScoreStruct *score2) {
    ScoreStruct t = *score1;
    *score1 = *score2;
    *score2 = t;
}

/**
 * Helper function for sorting the course's scores, by Student ID
 * @param scores the collection of scores for the course
 * @param numScores the number of total scores for the course
 */
void sortScores_SID(struct ScoreStruct *scores, int numScores) {
    for (int i = 0; i < numScores; ++i) {
        for (int j = 0; j < numScores; ++j) {
            if (scores[i].studentID < scores[j].studentID) {
                swapScores(&scores[i], &scores[j]);
            }
        }
    }
}

/**
 * Helper function for sorting the course's scores, by Assignment ID
 * @param scores the collection of scores for the course
 * @param numScores the number of total scores for the course
 */
void sortScores_AID(struct ScoreStruct *scores, int numScores) {
    for (int i = 0; i < numScores; ++i) {
        for (int j = 0; j < numScores; ++j) {
            if (scores[i].assignID < scores[j].assignID) {
                swapScores(&scores[i], &scores[j]);
            }
        }
    }
}

/**
 * This menu displays the scores for the selected student in the selected course
 * @param course the selected course the student is enrolled in
 * @param studentNum the studentID
 */
void getStudentScores(Course course, int studentNum) {

    char sName[32] = "";
    strcpy(sName, getStudentByID(studentNum)->name);
    printf("\n%s%s", sName, "'s assignment specific grades were:\n");
    printf("   %s      %s        %s\n", "Assign Name", "Score", "Comment");
    printf("---------------------------------------");
    double allScores = 0;
    sortScores_AID(course.scores, course.totalScores);
    for (int i = 0; i < course.totalScores; ++i) {
        if (course.scores[i].studentID == studentNum) {
            allScores += course.scores[i].score;
            printf("\n   %s            %1.2f        %s", getAssignmentByID(course, course.scores[i].assignID)->name,
                   course.scores[i].score, course.scores[i].comment);
        }
    }
    printf("\n\n%s%s %1.2f\n", sName, "'s final grade was", allScores / course.totalAssignments);
}

/**
 * A student that presents a list of students enrolled in a course where the user can choose a student to
 * view their course grades and final average score
 * @param course containing the displayed students
 */
void studentMenu(Course course) {
    int option;

    printf("\n-----------------------------------\n");
    printf("Student Menu");
    printf("\n-----------------------------------\n");
    printf("Please choose from the following students:\n");

    for (int i = 0; i < course.studentsEnrolled; i++) {
        printf("   %d %s %s \n", course.studentRoster[i].id, course.studentRoster[i].name,
               getStudentLevel(course.studentRoster[i].level));
    }
    printf("   0 REPEAT OPTIONS\n");
    printf("  -1 RETURN TO PREVIOUS MENU\n");
    printf("  -2 TERMINATE PROGRAM\n");
    printf("Please enter your choice ---> ");
    scanf(" %d", &option);
    if (option == 0 || option == -2) {
        commonOptions(option);
    } else if (option == -1) {
        commonOptions(option);
        subMenu(course);
    } else if (option > 0 && option <= course.studentsEnrolled) {
        getStudentScores(course, option);
    } else {
        printf("Please enter a valid option.\n");
    }
}

/**
* Implements assignment menu functionality, which allows user to select an assignment in the course to interact with
*/
void assignmentMenu(Course course) {
    int option;

    printf("\n-----------------------------------\n");
    printf("Course Assignments");
    printf("\n-----------------------------------\n");
    printf("Please choose from the following assignments:\n");
    int i;
    for (i = 0; i < course.totalAssignments; i++) {
        printf("   %d %s\n", course.assignments[i].id, courses->assignments[i].name);
    }
    printf("   0 REPEAT OPTIONS\n");
    printf("  -1 RETURN TO PREVIOUS MENU\n");
    printf("  -2 TERMINATE PROGRAM\n");
    printf("Please enter your choice ---> ");
    scanf(" %d", &option);
    if (option == 0 || option == -2) {
        commonOptions(option);
    } else if (option == -1) {
        commonOptions(option);
        subMenu(course);
    } else if (option > 0 && option <= course.totalAssignments) {
        getAssignmentScore(course, option);
    } else {
        printf("Please enter a valid option.\n");
    }
}

/**
* Handles menu options of main menu
* @param option is the chosen operation's option #
*/
void mainMenuBranch(int option) {
    if (option < -1 || option > numCourses) {
        printf("Invalid input. Please try again...\n");;
        while (option < -1 || option > numCourses) {
            printf("Please enter a valid option ---> ");
            scanf(" %d", &option);
        }
    }
    if (option == -1) {
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

void subMenu(Course course) {
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
void subMenuBranch(int option, Course course) {
    if (option < -2 || option > 3) {
        printf("Invalid input. Please try again...\n");;
        while (option < -2 || option > 3) {
            printf("Please enter a valid option ---> ");
            scanf(" %d", &option);
        }
    }

    commonOptions(option);

    if (option == 1) {
        studentMenu(course);
    } else if (option == 2) {
        assignmentMenu(course);
    } else if (option == 3) {
        printCourse(course);
    } else if (option == -1) {
        mainMenu();
    }
}

/**
 * This function prints all of the course's details, including name, instructor, assignments and grades
 * @param course that the information is displayed for
 */
void printCourse(Course course) {
    printf("\n%s %d \n", "Course ID:", course.id);
    printf("%s %s \n", "Course Name:", course.courseName);
    printf("%s %s \n", "Teacher:", course.instructorName);
    printf("Assigns:\n");
    for (int i = 0; i < course.totalAssignments; ++i) {
        printf("   %d %s \n", course.assignments[i].id, course.assignments[i].name);
    }
    sortScores_AID(course.scores, course.totalScores);
    sortScores_SID(course.scores, course.totalScores);
    printf("Grade Data:\n");
    for (int i = 0; i < course.totalScores; ++i) {
        printf("   %d %d ", course.scores[i].studentID, course.scores[i].assignID);
        printf("%.0f ", course.scores[i].score);
        printf("%s\n", course.scores[i].comment);
    }
}

/**
* Prints basic usage instructions for the program to the command line
*/
void print_usage() {
    printf("USAGE:\n./LastNameCourseReader -d <data_file_name(char*)> -c <instruction_file_name(char*)>\n");
    printf("-d refers to the required input data file containing student & course information; this must be a valid .txt file\n");
    printf("-i refers to the optionally supported 'instruction file' that provides directions for how the program should execute without CLI input; \n\t must be a valid .txt.file\n");
}

int main(int argc, char *argv[]) {
    char *datafile;
    char *instructionfile;
    int opt;
    while ((opt = getopt(argc, argv, ":d:i:")) != -1) {
        switch (opt) {
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
    for (; optind < argc; optind++) {
        printf("Given extra arguments: %s\n", argv[optind]);
    }

    int dflen;
    if (datafile != NULL) {
        dflen = strlen(datafile);
        if (dflen >= 5
            && (strcmp(&datafile[dflen - 4], ".txt") == 0)
            && (access(datafile, F_OK) != -1)) {
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
    if (instructionfile != NULL) {
        iflen = strlen(instructionfile);
        if (iflen >= 5
            && (strcmp(&instructionfile[iflen - 4], ".txt") == 0)
            && (access(instructionfile, F_OK) != -1)) {
            printf("Performing instructions defined in %s:\n", instructionfile);
            performInstructions(instructionfile);
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