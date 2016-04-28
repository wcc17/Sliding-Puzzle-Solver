//
// Created by Christian Curry on 4/26/16.
//

#ifndef TILEGAME_MULTIDIMENSIONALTILEARRAY_H
#define TILEGAME_MULTIDIMENSIONALTILEARRAY_H

#include "Tile.h"

//so i can use a vector of 2d arrays in my solve puzzle algorithm
struct MultiDimensionalTileArray {
    Tile array[4][4];
};

#endif //TILEGAME_MULTIDIMENSIONALTILEARRAY_H
