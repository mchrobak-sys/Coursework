#include <stdio.h>
#include "ConvertingToGcode.h"

int drawShape(struct shapeData*shape,int row, int col,float gridSize)

{
    float newOriginX; //This is the starting x co ordiate of the cell that is picked by the user
    float newOriginY; //This is the starting y co ordiate of the cell that is picked by the user
    float Scale; // this will be how much the shape will be muliplied by to get it to the correct size
    float cellSize; // This is the individual cell size 
    float NewWidth; // This will be the new cell size considering the 2mm clearance 
    float clearance = 2; 
    float sizeOfShapes = 16;
    float y; //y's true position 
    float x; // x's true position 
    int i; //This is the loop counter 

    cellSize = gridSize/3;     //Calculation to find the cell size
    NewWidth = cellSize - 2*clearance; // Calculation to ensure that there is a clearance of 2mm of either side 
    Scale = NewWidth/ sizeOfShapes;  //This is the multiplier that will scale the shape to the correct size 

    newOriginX = (float)(col -1) * cellSize; // This caluclates the left edge of a seclected cell 

    newOriginY = -gridSize + (float)(row-1) * cellSize; //This caluclates the bottom edge of a seclected cell


    for (i = 0; i< shape->pointNumber;i++) //loop cycles through all of the points in the struct 
    {
        y = newOriginY + ((shape->pointsList[i].y)*(Scale)) + clearance; //maps the new origin to the bottom of the cell with clearance 

        x = newOriginX + ((shape->pointsList[i].x)*(Scale)) +clearance; //maps the new origin to the left of the cell with clearance 

        printf(" X = %f  Y = %f  point = %d\n",x,y,i);



    }
    

    return 0;
}

int main(void)
{

    struct Points trianglePoints[4] = {{8,0,0}, {16,16,1}, {0,16,1}, {8,0,1}}; //Creating an array for the test taken from the lecture slides 

    struct shapeData test; //Empty shape creation 

    test.pointNumber =4; // setting the number of points in the shape to 4 

    test.pointsList = trianglePoints; // setting the points list to triangle points for testing 

    drawShape(&test,1,1,90.0f); //This should draw the shape in cell (1,1) with a 90mm grid.
   
    return 0; 

}
