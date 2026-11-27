#ifndef CONVERTINGTOGCODE_H //Guards for compiler so once we include the files it wont get 'reincluded'

#define CONVERTINGTOGCODE_H

#include "Functions/OpeningReadingShapeFiles/OpeningReadingShapesFile.h"


int drawShape(struct shapeData *shape, int row , int col ,float cellSize);

#endif