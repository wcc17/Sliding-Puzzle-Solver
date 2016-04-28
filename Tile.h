//
// Created by Christian Curry on 4/17/16.
//

#ifndef TILEGAME_TILE_H
#define TILEGAME_TILE_H

#include <SDL_rect.h>
#include <SDL_events.h>
#include <SDL_render.h>


class Tile {

private:

    int SCREEN_WIDTH = 640;
    int SCREEN_HEIGHT = 480;
    int TILE_WIDTH = SCREEN_WIDTH / 4;
    int TILE_HEIGHT = SCREEN_HEIGHT / 4;

    void pickUp(bool& alreadyHoldingTile);
    void putBack(bool& alreadyHoldingTile);

public:

    Tile();
    Tile(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect outlineRect);
    void handleEvent(SDL_Event* event);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    void setDimensions();
    bool isActive = false;
    bool isEmptyTile = false;

    SDL_Point currentArrayIndex;
    SDL_Point originalArrayIndex;

    //the source rect is the portion of the source image that we cut out for this tile
    SDL_Rect sourceRect;

    //the destination rect is the place on the board where the tile currently is
    SDL_Rect destinationRect;

    //the rect that will draw an outline around a tile to further differentiate it from other tiles
    SDL_Rect outlineRect;
};


#endif //TILEGAME_TILE_H
