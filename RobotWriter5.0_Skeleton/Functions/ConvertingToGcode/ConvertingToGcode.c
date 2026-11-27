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

    cellSize = gridSize/3;
    NewWidth = cellSize - 2*clearance;
    Scale = NewWidth/ sizeOfShapes;  
    
    newOriginX = (float)(col -1) * cellSize; // This caluclates the left edge of a seclected cell 
    newOriginY = -gridSize + (float)(row-1) * cellSize; //This caluclates the bottom edge of a seclected cell

    printf("Scale:%f\nX:%f\nY:%f\n",Scale,newOriginX,newOriginY);

    return 0;
}

int main(void)
{

    struct shapeData test; 

    drawShape (&test,1,1,90.0f); //This is the cell in the bottom left corner 

    drawShape (&test,3,3,90.0f); //This is the cell in the top right corner 

    drawShape (&test,1,1,30.0f); //This is the smallest grid 

    drawShape (&test,3,3,100.0f); //This is the largest grid  

    return 0; 

}
