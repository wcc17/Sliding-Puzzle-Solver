//
// Created by Christian Curry on 4/20/16.
//

#ifndef TILEGAME_NODE_H
#define TILEGAME_NODE_H


#include <vector>
#include "Tile.h"

class Node {
public:
    Node(int f, int g, int h);

    int f;
    int g;
    int h;
    Tile attemptArray[4][4];
    Node* parent;
    int move;
};


#endif //TILEGAME_NODE_H
