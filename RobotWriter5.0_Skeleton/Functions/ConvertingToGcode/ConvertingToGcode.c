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
    int drawing = 0; // When the gcode is at zero the pen will be up 
    char gCodeHolder[50]; // keeps hold of a g code co-ordinate before sending to the robot   

    cellSize = gridSize/3;     //Calculation to find the cell size
    NewWidth = cellSize - 2*clearance; // Calculation to ensure that there is a clearance of 2mm of either side 
    Scale = NewWidth/ sizeOfShapes;  //This is the multiplier that will scale the shape to the correct size 

    newOriginX = (float)(col -1) * cellSize; // This caluclates the left edge of a seclected cell 

    newOriginY = -gridSize + (float)(row-1) * cellSize; //This caluclates the bottom edge of a seclected cell
    
    x = newOriginX + clearance; // calibrating the starting x position of pen 

    y = newOriginY + clearance; //calibrating starting positon of y  

    sprintf(gCodeHolder,"G0 X%f y%f\n",x,y); // writing the command into the gcode holder 

    SendCommands(gCodeHolder); // Makes the robot move to this point if it is not at this point already 

    for (i = 0; i< shape->pointNumber;i++) //loop cycles through all of the points in the struct 
    {
        y = newOriginY + ((shape->pointsList[i].y)*(Scale)) + clearance; //maps the new origin to the bottom of the cell with clearance (origin of the chosen cell ) 

        x = newOriginX + ((shape->pointsList[i].x)*(Scale)) +clearance; //maps the new origin to the left of the cell with clearance (origin of the chosen cell)

        //This section of code up to line 64, firstly checks if the pen should be up or down for this point. If it is meant to be up 
        //as we want the robot to move without writing to a new point or cell. There is an if loop that checks for this condition 
        //if the pen is meant to be up, then another if loop is used to check if the previous pen position is down.
        //If the pen is down the code will lift it up and rewrite the variable of 'drawing' so it knows the pen is now up.
        //In the case where the pen was already up, then the robot will just go to the next co-ordinate
        //In the case we want the line to be drawn rather than moving to a position without drawing, the else catches this case 
        //We then check where the pen position is with the if condition. If the pen is up, then the code will put the pen down 
        //send the move to the robot and then set the new drawing position to 1 to remember where it is at.

            if(shape->pointsList[i].z == 0) //Checks for the move and keeps the pen up to move to that position 
            {
                if(drawing == 1) // This is a loop that occurs when the pen is down from the previous point 
                {
                    sprintf(gCodeHolder , "S0\n"); // Tells the pen to lift up since we are moving to a new position 

                    SendCommands(gCodeHolder); //Sending the command to the robot

                    drawing = 0; // change the drawing variable to 0 so that in the next pass the robot knows what position the pen is in 

                }
                sprintf(gCodeHolder, "G0 X%f Y%f\n",x,y); //writes the Gcode command into the gCode holder to be applied 
            }
            else
            {
                if (drawing == 0) //This is a loop that occurs when the pen is up from the previous point
                {
                    sprintf(gCodeHolder,"S1000\n"); // Puts the pen position to down / 1 

                    SendCommands(gCodeHolder); //This actually sends the command to the robot 

                    drawing = 1; //Overwrites pen position 

                }
                sprintf(gCodeHolder,"G1 X%f Y%f\n",x,y); //writes the Gcode command into the gCode holder to be applied 


            }

            SendCommands(gCodeHolder); // This sends the final command on the point G0, G1 


        }

        sprintf(gCodeHolder,"S0\n"); //writes the Gcode command into the gCode holder to be applied and since its outside the for loop, this occurs 
                                     //when the shape is complete. So the pen is lifted ready for the next user 
        
        SendCommands(gCodeHolder); //This actually sends the command to the robot


    return 0;
}

#include "../OpeningReadingShapeFiles/OpeningReadingShapesFile.c"

void SendCommands(char *buffer) //A replacement for the 'main main' for testing the code in the terminal
{

    printf("%s",buffer);

}


int main(void)
{

    struct Shapes allShapes; // These are all the shapes from the read file 

    int i; //Simple loop counter 

    int result; 

    result = readShapesFile(&allShapes); //Holds what shapes files will return could be 0 or any error from readShapesFile
    
    if(result != 0) //Basically if there is an error it returns the cause of the error 
    {
        printf("The shape file was not read %d\n",result);
        
        return result;


    }

    for (i = 0; i< allShapes.shapeNumber; i ++) //Goes through all of the shapes 
    {
        printf("Shape read %s\n",allShapes.shapesList[i].shapeName);

        drawShape(&allShapes.shapesList[i],1,1,90);//gcode conversion so that the shape is made in cell 1,1 for a 90mm grid 

    }
    
    freeShapes(&allShapes); //frees the dynamic memory that was taken by the shapes file 
   
    return 0; 

}
