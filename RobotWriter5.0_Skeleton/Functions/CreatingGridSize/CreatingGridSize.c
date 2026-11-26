#include <stdio.h>
#include <stdlib.h> 
#include "CreatingGridSize.h"

int getCreatingGridSize(float *Height, float *Width)
{
    
        printf("Please enter the grid size you would like to play on. Input the width and then the height of the grid\n");

        if (scanf("%f %f", Width,Height) != 2) // Ensures that the user has only entered 2 numbers 
        {
            printf("You must enter 2 values\n");

            return -8;
        }
        
        else if (*Width< MIN || *Width>Max || *Height<MIN || *Height>Max ) // Checks if the user inputs are within the range of the breif 
        {
            printf("The height and width must be within the range of 30-100, please enter your value again\n");
            return -9;

        }

        else
        {

            return 0; // If nothing goes wrong 0 will be returned 
        } 
}

int main(void)
{
    float Width, Height; 

    int correct = getCreatingGridSize(&Height,&Width); // handing over the address and calling the function 

    printf("Return Type: %d\nThe Width of the grid will be %f\nThe Height of the grid will be %f\n",correct,Width,Height);

    return 0;

}