//
// Created by Christian Curry on 4/17/16.
//

#include "TileGame.h"

TileGame::TileGame() {
    isRunning = true;
}

int TileGame::onExecute() {
    if(onInit() == false) {
        return -1;
    }

    SDL_Event event;

    while(isRunning) {
        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }

        onLoop();
        onRender();
    }

    onCleanup();

    return 0;
}

bool TileGame::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize. SDL_ERROR: %s\n", SDL_GetError());
        return false;
    }

    //adding 200 to the width of the window to fit the buttons
    window = SDL_CreateWindow("Tile Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH + 200, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
    }
    else {

        renderer = SDL_CreateRenderer(window, -1,  SDL_RENDERER_ACCELERATED);
        if(renderer == NULL) {
            printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
            return false;
        } else {
            //Initialize renderer background color
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //initialize JPG loading
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not be initialized. SDL_image Error: %s\n", IMG_GetError());
                return false;
            }
        }

        tileImageTexture = loadTexture("assets/Kentucky_uni_logo.png");
    }

    initializeTiles();
    initializeButtons();

    return true;
}

void TileGame::onLoop() {
}

void TileGame::onRender() {
    //clear screen
    SDL_RenderClear(renderer);

    //set the color that the renderer will draw shapes with (our tile outlines specifically)
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    if(movingTilesFromAlgorithmResults) {
        renderTilesWithDelay();
    } else {
        //render normally
        renderTiles();
    }

    solvePuzzleButton.render(renderer);

    //update screen
    SDL_RenderPresent(renderer);
}

void TileGame::onCleanup() {
    SDL_DestroyTexture(tileImageTexture);
    tileImageTexture = NULL;

    //destory the window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* TileGame::loadTexture(std::string path) {
    //final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specfied path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL) {
        printf("Unable to load image %s. SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == NULL) {
            printf("Unable to create texture from %s. SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void TileGame::initializeButtons() {
    solvePuzzleButton.buttonTexture = loadTexture("assets/solve_puzzle_button.jpg");
    solvePuzzleButton.setPosition(SCREEN_WIDTH + 25, 160);
}

void TileGame::initializeTiles() {

    //the SDL_Rect object that represents the position of the original square that was cut from the larger picture
    SDL_Rect sourceRect;

    //this rect represents the outline around the tiles, follows around the destination rect
    SDL_Rect outlineRect;

    //this rect represents the actual position on the board that the tile will go to
    SDL_Rect destinationRect;

    //here we will go through the full picture and split it into 4x4 tile board. each tile has
    //a source rect, a destination rect, and an outline rect
    //at the beginning all three will match, but destination and outline will be changed when the board is shuffled
    std::vector<Tile> temp;
    for(int i = 0; i < SCREEN_WIDTH; i += TILE_WIDTH) {
        for(int j = 0; j < SCREEN_HEIGHT; j += TILE_HEIGHT) {
            sourceRect.x = i;
            sourceRect.y = j;
            destinationRect.x = i;
            destinationRect.y = j;
            outlineRect.x = i;
            outlineRect.y = j;

            //destinationRect and sourceRect don't need to be set yet
            Tile tile(sourceRect, destinationRect, outlineRect);
            temp.push_back(tile);
        }
    }

    //remove the 16th tile
    temp.pop_back();

    //now throw these tiles in a 2d array
    //for the sake of this assignment, I'm assuming that there will always be 15 elements in the temp vector
    int tempVectorIndex = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            tileArray[i][j] = temp[tempVectorIndex++];

            SDL_Point point;
            point.x = i;
            point.y = j;
            tileArray[i][j].currentArrayIndex = point;
            tileArray[i][j].originalArrayIndex = point;

            if(tempVectorIndex == 15) {
                SDL_Rect sourceRect;
                SDL_Rect destinationRect;
                SDL_Rect outlineRect;

                sourceRect.h = TILE_HEIGHT;
                sourceRect.w = TILE_WIDTH;
                sourceRect.x -1000;
                sourceRect.y = -1000;
                destinationRect.h = TILE_HEIGHT;
                destinationRect.w = TILE_WIDTH;
                destinationRect.x = 480;
                destinationRect.y = 360;

                Tile emptyTile(sourceRect, destinationRect, outlineRect);
                emptyTile.currentArrayIndex.x = 3;
                emptyTile.currentArrayIndex.y = 3;
                emptyTile.originalArrayIndex.x = 3;
                emptyTile.originalArrayIndex.y = 3;
                emptyTile.isEmptyTile = true;

                tileArray[3][3] = emptyTile;
                break;
            }
        }
    }
}

void TileGame::renderTilesWithDelay() {
    if(!movesToSolve.empty()) {
        int move = movesToSolve.top();
        movesToSolve.pop();

        moveTileInDirection(move);

        SDL_Delay(350);
        renderTiles();
    } else {
        movingTilesFromAlgorithmResults = false;
        finalNode = NULL;
    }
}

void TileGame::renderTiles() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            tileArray[i][j].render(renderer, tileImageTexture);
        }
    }
}

void TileGame::onEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT) {
        isRunning = false;
    }

    if(!solvingPuzzle) {
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                if(!delayClick) {
                    tileArray[i][j].handleEvent(event);
                }

                if(tileArray[i][j].isActive) {
                    Tile* emptyTile = getEmptyTile(tileArray);

                    if(checkIfLegalMove(tileArray[i][j], *emptyTile)) {
                        moveTile(tileArray[i][j], *emptyTile);
                    } else {
                        //display error message?
                    }

                    tileArray[i][j].isActive = false;
                }
            }
        }

        if(delayClickTime < 50) {
            delayClickTime += SDL_GetTicks();
        } else {
            delayClickTime = 0;
            delayClick = false;
        }
    }

    if(solvePuzzleButton.handleEvent(event)) {
        printf("Solve puzzle button clicked\n");
        solvingPuzzle = true;

        solvePuzzleInit();
        prepareMovesFromAlgorithm();

        solvingPuzzle = false;
    }
}

bool TileGame::checkIfLegalMove(Tile tile, Tile emptyTile) {
    /**
     *  (0,0)   (160,0)   (320,0)   (480,0)
     *
     *  (0,120) (160,120) (320,120) (480,120)
     *
     *  (0,240) (160,240) (320,240) (480,240)
     *
     *  (0,360) (160,360) (320,360) (480,360)
     */

    /**
     *  (x,y)
     *
     *  (0,0)   (1,0)   (2,0)   (3,0)
     *
     *  (0,1)   (1,1)   (2,1)   (3,1)
     *
     *  (0,2)   (1,2)   (2,2)   (3,2)
     *
     *  (0,3)   (1,3)   (2,3)   (3,3)
     */

    int activeX = tile.currentArrayIndex.x;
    int activeY = tile.currentArrayIndex.y;

    int emptyX = emptyTile.currentArrayIndex.x;
    int emptyY = emptyTile.currentArrayIndex.y;

//    printf("Empty tile is at (%d, %d) with position (%d, %d)\n", emptyX, emptyY, emptyTile.destinationRect.x, emptyTile.destinationRect.y);

    if(activeX >= 0 && activeY >= 0 && activeX < 4 && activeY < 4) {
        //check to the left
        if (((emptyX - 1) == activeX) && emptyY == activeY) {
//            printf("This is a legal move for the tile on the left of the empty tile\n");
            return true;
        }

        //check to the right
        if (((emptyX + 1) == activeX) && emptyY == activeY) {
//            printf("This is a legal move for the tile on the right of the empty tile\n");
            return true;
        }

        //check the tile above
        if (((emptyY - 1) == activeY) && emptyX == activeX) {
//            printf("This is a legal move for the tile above the empty tile\n");
            return true;
        }

        //check the tile below
        if (((emptyY + 1) == activeY) && emptyX == activeX) {
//            printf("This is a legal move for the tile below the empty tile\n");
            return true;
        }
    }

//    printf("This is not a legal move\n");

    return false;
}

void TileGame::moveTileInDirection(int direction) {
    const int UP = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int RIGHT = 3;

    Tile* emptyTile = getEmptyTile(tileArray);
    SDL_Point indexToMoveTo;
    switch(direction) {
        case UP:
            indexToMoveTo.x = emptyTile->currentArrayIndex.x;
            indexToMoveTo.y = emptyTile->currentArrayIndex.y - 1;
            break;
        case DOWN:
            indexToMoveTo.x = emptyTile->currentArrayIndex.x;
            indexToMoveTo.y = emptyTile->currentArrayIndex.y + 1;
            break;
        case LEFT:
            indexToMoveTo.x = emptyTile->currentArrayIndex.x - 1;
            indexToMoveTo.y = emptyTile->currentArrayIndex.y;
            break;
        case RIGHT:
            indexToMoveTo.x = emptyTile->currentArrayIndex.x + 1;
            indexToMoveTo.y = emptyTile->currentArrayIndex.y;
            break;
    }
    //get the tile we want to move with the empty tile
    Tile* tile = getTileAtIndex(indexToMoveTo.x, indexToMoveTo.y, tileArray);

    moveTile(*tile, *emptyTile);
}

//essentially swapping the positions on the board of tile and emptyTile and updating their current indexes
void TileGame::moveTile(Tile& tile, Tile& emptyTile) {
    //assign tiles destination rect to a temporary rect
    SDL_Rect temp;
    temp.x = tile.destinationRect.x;
    temp.y = tile.destinationRect.y;

    //assign the empty tiles destination rect to the active tiles destination rect
    tile.destinationRect.x = emptyTile.destinationRect.x;
    tile.destinationRect.y = emptyTile.destinationRect.y;
    tile.outlineRect.x = tile.destinationRect.x;
    tile.outlineRect.y = tile.destinationRect.y;

    //assign the temp rects info to the empty tile destination rect
    emptyTile.destinationRect.x = temp.x;
    emptyTile.destinationRect.y = temp.y;

    //assign the active tile index to a temporary point
    SDL_Point tempPoint;
    tempPoint.x = tile.currentArrayIndex.x;
    tempPoint.y = tile.currentArrayIndex.y;

    //assign the empty tile index to the active tile's current index
    tile.currentArrayIndex.x = emptyTile.currentArrayIndex.x;
    tile.currentArrayIndex.y = emptyTile.currentArrayIndex.y;

    //assign the temp index to the empty tile index
    emptyTile.currentArrayIndex.x = tempPoint.x;
    emptyTile.currentArrayIndex.y = tempPoint.y;
}

void TileGame::prepareMovesFromAlgorithm() {
    movesToSolve = std::stack<int>();

    Node* node = finalNode;
    while(node != NULL) {
        int move = node->move;

        //the root node's move is initialized with -1, so we throw that out here
        //we wouldn't want any other illegal moves anyway
        if(move >= 0 && move < 4) {
            movesToSolve.push(node->move);
        }

        node = node->parent;
    }

    movingTilesFromAlgorithmResults = true;

    printf("Number of moves to make: %lu\n", movesToSolve.size());
}

void TileGame::solvePuzzleInit() {
    solved = false;
    //no clear method for std::priority_queue?
    queue = std::priority_queue<Node*, std::vector<Node*>, costCompareStruct>();
    alreadyVisitedNodes.clear();

    int numberOfIncorrectPositions = checkIncorrectPositions(tileArray);
    if(numberOfIncorrectPositions > 0) {
        rootNode = new Node(numberOfIncorrectPositions, 0, numberOfIncorrectPositions);
        copyTileArrays(rootNode->attemptArray, tileArray);
        rootNode->move = -1;
        rootNode->parent = NULL;

        queue.push(rootNode);

        Node* node = NULL;
        while(!solved) {
            bool exit = false;

            while(!exit) {

                //get the next node from the priority queue
                node = queue.top();
                queue.pop();

                //if alreadyVisistedNodes contains the popped off node, this loop will run again
                bool c = contains(alreadyVisitedNodes, node);
                if(!c) {

                    if(node->h == 0) {
                        solved = true;
                        finalNode = node;
//                        copyTileArrays(tileArray, node->attemptArray);
                        return;
                    }

                    //otherwise exit the loop
                    exit = true;
                }
            }

            //get the next step in solving the puzzle with the current node
            solvePuzzle(*node);

            printf("f: %d, g: %d, h: %d\n", node->f, node->g, node->h);

            alreadyVisitedNodes.push_back(node);
        }
    }
}

void TileGame::solvePuzzle(Node& node) {
    const int UP = 0;
    const int DOWN = 1;
    const int LEFT = 2;
    const int RIGHT = 3;

    std::vector<MultiDimensionalArray> tileArrayVector;
    MultiDimensionalArray upArray;
    MultiDimensionalArray downArray;
    MultiDimensionalArray leftArray;
    MultiDimensionalArray rightArray;

    //copy the last attempt into all the different attempt arrays for this pass
    copyTileArrays(upArray.array, node.attemptArray);
    copyTileArrays(downArray.array, node.attemptArray);
    copyTileArrays(leftArray.array, node.attemptArray);
    copyTileArrays(rightArray.array, node.attemptArray);

    tileArrayVector.push_back(upArray);
    tileArrayVector.push_back(downArray);
    tileArrayVector.push_back(leftArray);
    tileArrayVector.push_back(rightArray);

    //will contain the number of incorrect tiles for each move attempt
    for(int i = UP; i < tileArrayVector.size(); i++) {
        //now check if the attempts are even possible
        Tile* emptyTile = getEmptyTile(tileArrayVector[i].array);

        //find a place to move the empty tile to
        SDL_Point indexToMoveTo;
        switch(i) {
            case UP:
                indexToMoveTo.x = emptyTile->currentArrayIndex.x;
                indexToMoveTo.y = emptyTile->currentArrayIndex.y - 1;
                break;
            case DOWN:
                indexToMoveTo.x = emptyTile->currentArrayIndex.x;
                indexToMoveTo.y = emptyTile->currentArrayIndex.y + 1;
                break;
            case LEFT:
                indexToMoveTo.x = emptyTile->currentArrayIndex.x - 1;
                indexToMoveTo.y = emptyTile->currentArrayIndex.y;
                break;
            case RIGHT:
                indexToMoveTo.x = emptyTile->currentArrayIndex.x + 1;
                indexToMoveTo.y = emptyTile->currentArrayIndex.y;
                break;
        }
        //get the tile we want to move with the empty tile
        Tile* tile = getTileAtIndex(indexToMoveTo.x, indexToMoveTo.y, tileArrayVector[i].array);

        //check if legal move, if so, push into priority queue
        int depth = node.g + 1;
        Node* newNode;
        if(checkIfLegalMove(*emptyTile, *tile)) {
            moveTile(*emptyTile, *tile);
            int incorrect = checkIncorrectPositions(tileArrayVector[i].array);

            newNode = new Node(depth + incorrect, depth, incorrect);
            copyTileArrays(newNode->attemptArray, tileArrayVector[i].array);
            newNode->move = i;
            newNode->parent = &node;

            queue.push(newNode);
        }
    }
}

bool TileGame::contains(std::vector<Node*> alreadyVisitedNodes, Node* node) {
    for(int i = 0; i < alreadyVisitedNodes.size(); i++) {
        if(checkArrayEquality(node->attemptArray, alreadyVisitedNodes[i]->attemptArray)) {
//            if(node->f == alreadyVisitedNodes[i]->f
//               && node->g == alreadyVisitedNodes[i]->g
//               && node->h == alreadyVisitedNodes[i]->h) {
                return true;
//            }
        }
    }

    return false;
}

//returns a pointer to the empty tile in the array
Tile* TileGame::getEmptyTile(Tile (&array)[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(array[i][j].isEmptyTile) {
                return &array[i][j];
            }
        }
    }

    return NULL;
}

bool TileGame::checkArrayEquality(Tile (&a)[4][4], Tile (&b)[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(a[i][j].currentArrayIndex.x != b[i][j].currentArrayIndex.x
               || a[i][j].currentArrayIndex.y != b[i][j].currentArrayIndex.y) {
                return false;
            }
        }
    }
}

//check the number of correct positions in a given board (represented by array) after an attempted move
int TileGame::checkIncorrectPositions(Tile (&array)[4][4]) {
    int goal = 15;

    int numberCorrect = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(array[i][j].currentArrayIndex.x == array[i][j].originalArrayIndex.x
               && array[i][j].currentArrayIndex.y == array[i][j].originalArrayIndex.y) {
                //we're only worried about the non-empty tiles.
                if(!array[i][j].isEmptyTile) {
                    numberCorrect++;
                }
            }
        }
    }

    goal -= numberCorrect;

    return goal;
}

void TileGame::copyTileArrays(Tile (&destination)[4][4], Tile (&source)[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

Tile* TileGame::getTileAtIndex(int x, int y, Tile(&array)[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(array[i][j].currentArrayIndex.x == x && array[i][j].currentArrayIndex.y == y) {
                return &array[i][j];
            }
        }
    }
}