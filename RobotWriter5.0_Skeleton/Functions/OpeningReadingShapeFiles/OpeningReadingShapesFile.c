#include <stdio.h>
#include <stdlib.h>
#include "OpeningReadingShapesFile.h"



int readShapesFile(struct Shapes *file) // readShapesFile is given the address of allshapes so anything written through the file will change main
{
    FILE *fInput;  //File pointer keeps track of where in the file the code is 
    char header[20]; // Static array to place the header 
    int shapeNumber; // An int to hold the number of shapes 
    int i,j; // These will be used for the loops
    int pointNumber; // Number of points/strokes of the current shape

    file->shapeNumber = 0; // This makes sure the location where the shapes will go is empty  
    file->shapesList= NULL; // A shapes array has not been created yet sop the pointer does not point anywhere yet

    fInput = fopen("ShapeStrokeData.txt","r"); // Opening the shapes file 
    //fInput = fopen("testCases/AddtionalShapes.txt","r"); //These are all test cases 
    //fInput = fopen("testCases/IncorrectHeader.txt","r");
    //fInput = fopen("testCases/IncorrectOneShape.txt","r");
    //fInput = fopen("testCases/OneShape.txt","r");
    //fInput = fopen("testCases/IncorrectPoints.txt","r")





    if (fInput == NULL) //Null is returned if the file couldnt open 
        {
            printf("The file was not opened\n");
            return -1; // For the file not opening the error will be -1

        }

    if (fscanf(fInput,"%s %d",header, &shapeNumber) !=2) // The number of shape gets binded to the header however the number of shapes goes directly into the local shape number 
        {                                                //!=2 makes sure that the correct 2 things were read and if not then an error occurs 
            printf("The first line was not read\n");
            fclose(fInput);   //Closes the file 
            return -2;        //The error if we cant read the header and number of shapes will be -2

        }
    
    //printf("The file was succesfully opened and the first line was read\nThe %s is %d\n",header,shapeNumber);

    if (allocateShapes(file,shapeNumber) !=0) // Number of shapes is passed to allocate shapes, this is where the dynamic array is made and where the address is stored
    {
        printf("The shapes can not be placed in the memory\n");
        fclose(fInput);
        return -5; //If memory cannot be made for the shape number the error code -5 will come up
    }
    

//Whats happeining bellow this line: 
//Theres an embedded for loop in another for loop 
//The outer for loop is scanning the shape name and the number of points it has 
//Then if that passes, we go to the next line 
//This is where the embedded for loop reads the points (x,y,z) for every shape
//This loop then reads "pointNumber" lines down and finishes 
//If read correctly we go back to the outer loop and it reads the next shape name and number of points 
//There is also if statement in these loops which check for errors and print them to the screen

    for (i=0; i<file->shapeNumber; i++) // This uses the number of shapes read previously (with the header) and cycles through this many shapes 
        {                               
         if(fscanf(fInput,"%s %d",file->shapesList[i].shapeName,&pointNumber) !=2) //Here the shape name and number of points is read.Where the name goes into the dynamic array and the points go into the local pointNumber 
         {
            printf("Cant read the shape name and number of points\n");
            fclose(fInput);
            freeShapes(file); // clears the dynamic memory if a failure occurs 
            return -3; // A failure in reading any the names or point numbers will generate this error 
         }
        //printf("The shape is a %s with %d points\n",file->shapesList[i].shapeName,pointNumber);// This will print out the shape name and the number of points it has 

        if (allocatePoints(&file->shapesList[i],pointNumber) !=0) //This makes the shapes dynamic array of points.The pointNumber and address is passed of shape i to allocatePoints 
        {                                                         //Making the shapes own point array and stores the count inside the shape 
            printf("Cant allocate the points of the shape %s\n",file->shapesList[i].shapeName);
            fclose(fInput);
            freeShapes(file);// clears dynamic memory if failure occurs 
            return -6;  //Theres not enough memory to allocate the shape points so -6 is the error given back 



        }
    

            for (j=0; j<file->shapesList[i].pointNumber;j++) //This uses the point number read previously to read through x amount of lines to get all of the points in that shape structure 
            {                                                //These points go into the pointList dynamic array of point j of shape i 
                if(fscanf(fInput,"%f %f %d", &file->shapesList[i].pointsList[j].x,&file->shapesList[i].pointsList[j].y,&file->shapesList[i].pointsList[j].z) != 3)// This part reads the points to x,y,z into point j of shape i and makes sure 3 are read by !=3, if more or less are read there will be an error 
                {
                    printf("%s point %d wasnt read correctly\n",file->shapesList[i].shapeName,j);
                    fclose(fInput);
                    freeShapes(file);
                    return -4; //Creates this error if points cannot be read 
                 }
            //printf("Point:%d x:%f y:%f z:%d\n",j,file->shapesList[i].pointsList[j].x,file->shapesList[i].pointsList[j].y,file->shapesList[i].pointsList[j].z); //This will print out the point co-ordinate of each one of the points in the current shape 
            }
        
        }

    fclose(fInput); // The loops are finished and the file is complete 
    return 0;  //If the program gets to this point, every shape and point has been sucesfully stored and read 
}

int allocateShapes(struct Shapes *file,int count) // creates the dynamic array that holds every shape from the file 
{                                                 //shapesList is set to a new array and its shapeNumber is set to count 
    file->shapesList = calloc((size_t)count,sizeof(struct shapeData)); //Creates memory spaces for count shapes where they are all set to zero at the start 
    if (file->shapesList == NULL)                 //callock returns NULL if there isnt enough memory allocated so -7 will be returned 
        return -7;                                 
    file->shapeNumber = count; //This stores how many shapes the array holds 
    return 0; // If the program gets to this point the memeory for the shaopes has been created successfully 
}

int allocatePoints(struct shapeData *shape,int count) //Creates the dynamic array that holds the points/ stokes of the shapes 
{
    shape->pointsList = calloc((size_t)count,sizeof(struct Points)); // Creates memory for count points for the specific shape 
    if (shape->pointsList ==NULL)   //Null will be returned if enough memory was generated 
        return -8;
    shape->pointNumber = count; //Stores the number of point the specific shape has 
    return 0;
}

void freeShapes(struct Shapes *file) // releases all the dynamic memory held by the file 
{
    int i;
    if (file->shapesList ==NULL)
        return;
    for (i = 0; i < file->shapeNumber; i++) //This cycles through each shapes points/ strokes and frees them
        free(file->shapesList[i].pointsList); //The points of the shapes are released 
   free(file->shapesList); //The names of the shapes and the number of strokes is released 
   file->shapesList = NULL ; // Stopping the pointer from pointing at the memory that is no longer binded 
    file->shapeNumber = 0; // setting the number of shapes back to 0 


}

/*

int main()   //Main to test the function 
    {
        struct Shapes allShapes; //Once every shape has been succesfully read by the function, it holds this data in the struct 
        int outcome = readShapesFile(&allShapes); // Binds what error comes out of the function to outcome 
        int k;
            if (outcome != 0) // if output is not 0 i.e fail then we will get this error code in main 
            {
            printf("Failure %d\n",outcome);
            }
            else              // If 0 comes out which occurs when no failures happen the code works 
            {
                printf("Success yay\n");
                for (k = 0 ; k < allShapes.shapeNumber;k++)
                    printf("Starting shape is in memory %s\n",allShapes.shapesList[k].shapeName); //Checking if the dynamic memory data remained after the function ended by reading from main 

                freeShapes(&allShapes); //Free all of the dynamic memory used before the program finsishes 
            }
            return 0;
    }
*/

