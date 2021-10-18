//
// Created by kirry on 10/16/21.
//

#include <stdio.h>
#include <string.h>

int mystrlen(char *p)
{
    int c=0;
    while(*p)
    {
        c++;
        *p++;
    }
    return(c);
}

void toUpper(char* str) {
    int len = 0;
//    while (*(++str)) {
//        ++len;
//    }
//    str -= len + 1;

    while (*str) {
        if (str[0] != '\0') {
            if (str[0] != ' ') {
                str[0] -= 32;
            }
            len++;
            *str++;
        } else {
            str -= len + 1;
        }
    }
}

int main() {
    char text[] = "some sample text";
    toUpper(text);
    printf("%s", text);

    return 0;
}

