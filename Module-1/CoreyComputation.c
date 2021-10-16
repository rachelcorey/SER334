/**
* This program prompts the user to supply the height and radius of one or more cylinders, and then
* outputs the volume for each cylinder. A total sum volume of all of the cylinders is output at the
* end.
*
* Completion time: 20 minutes
*
* @author Rachel Corey
* @version 10/16/2021
*/

#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265359

float calculateVolume(float height, float radius) {
    return PI * (radius * radius) * height;
}

int main(void)
{
    int numCyl = 0;
    float height = 0;
    float radius = 0;
    float volume = 0;
    float totalVol = 0;

    printf("Enter the number of cylinders to sum: ");
    scanf("%d", &numCyl);

    for (int i = 0; i < numCyl; ++i) {
        printf("\nEnter the height of cylinder #");
        printf("%d", (i+1));
        printf(": ");
        scanf("%f", &height);
        printf("\nNow enter its radius: ");
        scanf("%f", &radius);

        printf("\nThe volume of this cylinder is: ");
        volume = calculateVolume(height, radius);
        printf("%f", volume);
        totalVol += volume;
        printf("\n");
    }

    printf("\nThe total volume sum for all cylinders is: ");
    printf("%f", totalVol);


    return (EXIT_SUCCESS);
}