///**
//* This program prompts the user to supply an integer, and outputs the number of
//* Collatz conjecture algorithm iterations for the integer.
//*
//* Completion time: 10 minutes
//*
//* @author Rachel Corey
//* @version 10/16/2021
//*/
//
//
//#include <stdio.h>
//
//int termination(int n) {
//    int iter = 0;
//    while (n != 1) {
//        iter++;
//        printf("\nIteration #");
//        printf("%d", iter);
//        printf(": ");
//        if (n % 2 == 0) {
//            n = n/2;
//        } else {
//            n = 3*n + 1;
//        }
//        printf("%d", n);
//    }
//    printf("\nTotal Iterations: ");
//    printf("%d", iter);
//    return n;
//}
//
//int main(void) {
//
//    int num = 0;
//
//    printf("\nEnter an integer: ");
//    scanf("%d", &num);
//
//    termination(num);
//
//    return 0;
//}
//
