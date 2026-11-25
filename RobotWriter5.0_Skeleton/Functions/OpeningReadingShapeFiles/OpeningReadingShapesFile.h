
#ifndef OPENINGREADINGSHAPESFILE_H // Guards for compiler so once we include the files it wont get 'reincluded'
#define OPENINGREADINGSHAPESFILE_H

struct Points //This defines the points 
{
    float x,y;
    int z; 

};

struct shapeData //This will contain the actual shape data 
{
    char shapeName[20]; // Shape data will hold these variables 
    int pointNumber;
    struct Points *pointsList; // This looks and points at the exact shapes points 

};

struct Shapes 
{
    int shapeNumber;
    struct shapeData *shapesList;//This looks and points at the shapes which are held within the array

};

int readShapesFile(struct Shapes *file);// It fills the dynamic array structure 
int allocateShapes(struct Shapes *file,int count); //creates a dynamic array of shapes 
int allocatePoints(struct shapeData *shape,int count); // Makes the dynamic array of the points of the shapes
void freeShapes(struct Shapes *file); // Frees the dynamic memory being used 

#endif 