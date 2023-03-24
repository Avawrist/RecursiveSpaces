// =============================================
// Title: level.hpp
// Description: The header file for level code 
// =============================================

#ifndef LEVEL_H
#define LEVEL_H

// C libs
#include <cstring>

// My libs
#include "utility.hpp"

//////////////////////
// Struct LevelGrid //
//////////////////////

typedef enum GridMeasurements
{
    MAX_WIDTH  = 50,
    MAX_LENGTH = 50,
    MAX_HEIGHT = 4
} GridMeasurements;

typedef struct LevelGrid
{
    int grid[MAX_WIDTH][MAX_HEIGHT][MAX_LENGTH];
    float square_dim = 2.5f;
    LevelGrid();
} LevelGrid;

#endif
