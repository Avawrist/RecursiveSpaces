// =============================================
// Title: level.cpp
// Description: The source file for level code 
// =============================================

#include "level.hpp"

//////////////////////
// Struct LevelGrid //
//////////////////////

LevelGrid::LevelGrid()
{
    memset(grid, -1, MAX_WIDTH * MAX_HEIGHT * MAX_LENGTH * sizeof(int));
}
