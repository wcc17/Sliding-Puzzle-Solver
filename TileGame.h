//
// Created by Christian Curry on 4/17/16.
//

#ifndef TILEGAME_TILEGAME_H
#define TILEGAME_TILEGAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "Tile.h"
#include "Button.h"
#include "Tree.h"
#include <queue>
#include <stack>


class TileGame {

private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* tileImageTexture = NULL;

    //will be called after tiles are swapped so that the user doesn't pick up the swapped tile directly after swapping
    bool delayClick = false;
    int delayClickTime = 0;

    void renderTiles();
    void renderTilesWithDelay();
    SDL_Texture* loadTexture(std::string path);
    void initializeTiles();
    void initializeButtons();

    bool checkIfLegalMove(Tile tile, Tile emptyTile);
    void moveTileInDirection(int direction);
    void moveTile(Tile& tile, Tile& emptyTile);
    Tile* getEmptyTile(Tile (&array)[4][4]);

    //algorithm stuff
    //so i can use a vector of 2d arrays in my solve puzzle algorithm
    struct MultiDimensionalArray {
        Tile array[4][4];
    };

    struct costCompareStruct
    {
        bool operator()(const Node* a, const Node* b) const
        {
            if(a->f == b->f) {
                return a->g < b->g;
            } else {
                return a->f > b->f;
            }
        }
    };

    bool solvingPuzzle = false;
    bool movingTilesFromAlgorithmResults = false;
    Node* rootNode;
    Node* finalNode; //this will be the final node that solves the puzzle. will use to backtrack and make moves one by one
    bool solved = false;
    void solvePuzzleInit();
    void solvePuzzle(Node& node);
    std::stack<int> movesToSolve;

    int checkIncorrectPositions(Tile (&array)[4][4]);
    void copyTileArrays(Tile (&destination)[4][4], Tile (&source)[4][4]);
    Tile* getTileAtIndex(int x, int y, Tile(&array)[4][4]);

    std::priority_queue<Node*, std::vector<Node*>, costCompareStruct> queue;
    std::vector<Node*> alreadyVisitedNodes;
    bool contains(std::vector<Node*> alreadyVisitedNodes, Node* node);
    bool checkArrayEquality(Tile (&a)[4][4], Tile (&b)[4][4]);

    void prepareMovesFromAlgorithm();

public:
    TileGame();

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    const int TILE_WIDTH = SCREEN_WIDTH / 4;
    const int TILE_HEIGHT = SCREEN_HEIGHT / 4;

    const int UP = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int RIGHT = 3;

    Tile tileArray[4][4];
    Button solvePuzzleButton;

    int onExecute();
    bool onInit();
    void onEvent(SDL_Event* event);
    void onLoop();
    void onRender();
    void onCleanup();
};


#endif //TILEGAME_TILEGAME_H
