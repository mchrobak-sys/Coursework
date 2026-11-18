#include <stdio.h>
#include <stdlib.h>
#include "OpeningReadingShapesFile.h"



int shapeFileReader(struct Shapes *file)
{
    FILE *fInput; 
    char header[20]; // Static array to place the header 
    int shapeNumber; // An int to hold the number of shapes 
    int i,j; // These will be used for the loops
    int pointNumber; 

    file->shapeNumber = 0; // This innitialises the array to 0 so values can then be put in 
    file->shapesList= NULL;

    fInput = fopen("ShapeStrokeData.txt","r"); // Opening the shapes file 

    if (fInput == NULL) //If nothing is found in the shapes file i.e cant open it 
        {
            printf("The file was not opened\n");
            return -1; // For the file not opening the error will be -1

        }

    if (fscanf(fInput,"%s %d",header, &shapeNumber) !=2)
        {
            printf("The first line was not read\n");
            fclose(fInput);   //Closes the file 
            return -2;        //The error if we cant read the header and number of shapes will be -2

        }
    
    printf("The file was succesfully opened and the first line was read\nThe %s is %d\n",header,shapeNumber);

    if (allocateShapes(file,shapeNumber) !=0) // If everything goes correctly we should receive 0 when the shape number is allocated.
    {
        printf("The shapes can not be placed in the memory\n");
        fclose(fInput);
        return -5;
    }
    

//Whats happeining bellow this line: 
//Theres an embedded for loop in another for loop 
//The outer for loop is scanning the shape name and the number of points it has 
//Then if that passes, we go to the next line 
//This is where the embedded for loop reads the point numbers that we read in the previous loop 
//This loop then reads "pointNumber" lines down and finishes 
//If read correctly we go back to the outer loop and it reads the next shape name and number of points 
//There is also if statement in these loops which check for errors and print them to the screen

    for (i=0; i<file->shapeNumber; i++) // This uses the number of shapes read previously (with the header) and cycles through this many shapes 
        {
         if(fscanf(fInput,"%s %d",file->shapesList[i].shapeName,&pointNumber) !=2) //This reads the shape name and the number of points it will have storing them for this run.The not 2 makes sure that 2 things are read. If theres 3 it fails if theres 1 it fails 
         {
            printf("Cant read the shape name and number of points\n");
            fclose(fInput);
            freeShapes(file); // clears the dynamic memory if a failure occurs 
            return -3; // A failure in reading any the names or point numbers will generate this error 
         }
        printf("The shape is a %s with %d points\n",file->shapesList[i].shapeName,pointNumber);// This will print out the shape name and the number of points it has 

        if (allocatePoints(&file->shapesList[i],pointNumber) !=0)
        {
            printf("Cant allocate the points of the shape %s\n",file->shapesList[i].shapeName);
            fclose(fInput);
            freeShapes(file);// clears dynamic memory if failure occurs 
            return -6;



        }
    

            for (j=0; j<file->shapesList[i].pointNumber;j++) //This uses the point number read previously to read through x amount of lines to get all of the points in that shape structure 
            {
                if(fscanf(fInput,"%f %f %d", &file->shapesList[i].pointsList[j].x,&file->shapesList[i].pointsList[j].y,&file->shapesList[i].pointsList[j].z) != 3)// This part binds the points to x,y,z and makes sure 3 are read, if more or less are read there will be an error 
                {
                    printf("%s point %d wasnt read correctly\n",file->shapesList[i].shapeName,j);
                    fclose(fInput);
                    freeShapes(file);
                    return -4; //Creates this error if points cannot be read 
                 }
            printf("Point:%d x:%f y:%f z:%d\n",j,file->shapesList[i].pointsList[j].x,file->shapesList[i].pointsList[j].y,file->shapesList[i].pointsList[j].z); //This will print out the point co-ordinate of each one of the points in the current shape 
            }
        
        }

    fclose(fInput);
    return 0;
}

int allocateShapes(struct Shapes *file,int count)
{
    file->shapesList = calloc(count,sizeof(struct shapeData));
    if (file->shapesList == NULL)
        return -7;
    file->shapeNumber = count;
    return 0; 
}

int allocatePoints(struct shapeData *shape,int count)
{
    shape->pointsList = calloc(count,sizeof(struct Points));
    if (shape->pointsList ==NULL)
    return -8;
shape->pointNumber = count;
return 0;
}

void freeShapes(struct Shapes *file)
{
    int i;
    if (file->shapesList ==NULL)
        return;
    for (i = 0; i < file->shapeNumber; i++)
        free(file->shapesList[i].pointsList);
   free(file->shapesList);
   file->shapesList = NULL ;
    file->shapeNumber = 0;


}

int main()   //Main to test the function 
    {
        struct Shapes allShapes;
        int outcome = shapeFileReader(&allShapes); // Binds what error comes out of the function to outcome 

            if (outcome != 0) // if output is not 0 i.e fail then we will get this error code in main 
            {
            printf("Failure %d\n",outcome);
            }
            else              // If 0 comes out which occurs when no failures happen the code works 
            {
                printf("Success yay\n");
                printf("Starting shape is in memory %s\n",allShapes.shapesList[0].shapeName);
                freeShapes(&allShapes);
            }
            return 0;
    }


