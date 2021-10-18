///**
//* This program decrypts and prints a hard-coded array of 4 strings.
//*
//* Completion time: 2 hours 30 minutes
//*
//* @author Rachel Corey
//* @version 10/16/2021
//*/


#include <stdio.h>
#include <string.h>

//macros: constants
#define CHUNK_LENGTH (20+1)  //each chunk has twenty characters, we add 1 so
                           //there is space for the null terminator.
#define NUMBER_OF_CHUNKS 4 //the message is spread across 4 chunks.
#define DECRYPTION_SHIFT 5 //this is the ASCII table shift used for decryption.
#define STRING_LENGTH 20

//forward declarations
void sort_chunks();
void decrypt_chunks();
void display_chunks();

char chunks[NUMBER_OF_CHUNKS][CHUNK_LENGTH];


// This method can be called to print each given chunk, starting at index 1
void print_array(char* array, int length)
{
    for (int i = 0; i < length - 1; i++) {
        printf("%c", array[i + 1]);
    }
}


// This method can be called to shift each character's ASCII code in a char array, starting at index 1
void shift_array(char* array, int length)
{
    for (int i = 1; i < (length); i++) {
        array[i] -= DECRYPTION_SHIFT;
    }
}


int main() {
	//copy message into memory.
    strcpy(chunks[0], "2i1%fsi%fs%jstwrtzx%");
    strcpy(chunks[1], "1'H%nx%vznwp~1%kqf|j");
    strcpy(chunks[2], "4R3%Wnyhmnj%%%%%%%%%");
    strcpy(chunks[3], "3xzhhjxx3'%2%Ijssnx%");

	//the format of a chunk is a single number indicating its order in overall
	//message followed by twenty encrypted characters.


	//reorder chunks in message by sorting them based on the first digital
	//they contain. looking above, one can see they are currently in the order
	//1, 1, 4, 3 but should be ordered 1, 2, 3, 4.
	sort_chunks();

	//shift the characters in the message to produce the original characters.
	decrypt_chunks();

	//display the decrypted message.
	display_chunks();

	return 0;
}

//given two strings, swaps their contents in memory.
void swap_strings(char* x, char* y) {
	//create a temporary holding place for the data so we don't lose it.
	char temp[CHUNK_LENGTH];

	strcpy(temp, x);
	strcpy(x, y);
	strcpy(y, temp);
}

//sorts the strings the global chunks variable by the first character they contain.
// uses Insertion Sort
void sort_chunks() {
    for (int i = 0; i < NUMBER_OF_CHUNKS; i++) {
        char key = chunks[i][0];
        char* keyPos = chunks[i];
        int position = i;
        char t = chunks[position - 1][0];

        while (position > 0 && t > key) {
            swap_strings(chunks[position], chunks[position - 1]);
            position--;
            t = chunks[position - 1][0];
        }
    }
}

//for each string in the global chunks variable, shifts the characters in it by
//DECRYPTION_SHIFT.
void decrypt_chunks() {
    for (int i = 0; i < NUMBER_OF_CHUNKS; ++i) {
        shift_array(chunks[i], STRING_LENGTH);
    }
}


//displays the strings in the global chunks variable
void display_chunks() {
    for (int i = 0; i < NUMBER_OF_CHUNKS; ++i) {
        print_array(chunks[i], STRING_LENGTH);
    }
}
