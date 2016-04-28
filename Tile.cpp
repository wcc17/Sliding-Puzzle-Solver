//
// Created by Christian Curry on 4/17/16.
//

#include "Tile.h"

Tile::Tile() {}

Tile::Tile(SDL_Rect sourceRect, SDL_Rect destinationRect, SDL_Rect outlineRect) {
    this->sourceRect = sourceRect;
    this->destinationRect = destinationRect;
    this->outlineRect = outlineRect;

    setDimensions();
}

void Tile::setDimensions() {
    this->sourceRect.w = TILE_WIDTH;
    this->sourceRect.h = TILE_HEIGHT;
    this->destinationRect.w = TILE_WIDTH;
    this->destinationRect.h = TILE_HEIGHT;
    this->outlineRect.w = TILE_WIDTH;
    this->outlineRect.h = TILE_HEIGHT;
}

void Tile::handleEvent(SDL_Event* event) {
    //If mouse event happened
    if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP ) {

        int mouseX, mouseY;
        bool insideCurrentTilePosition = true;
        SDL_GetMouseState(&mouseX, &mouseY);

        //destinationRect is where the tile is actually sitting on the board
        if(mouseX < destinationRect.x) {
            insideCurrentTilePosition = false;
        } else if(mouseX > (destinationRect.x + TILE_WIDTH) ) {
            insideCurrentTilePosition = false;
        } else if(mouseY < destinationRect.y) {
            insideCurrentTilePosition = false;
        } else if(mouseY > (destinationRect.y + TILE_HEIGHT) ) {
            insideCurrentTilePosition = false;
        }

        //if we're clicking inside the current tile
        if(insideCurrentTilePosition) {
            //if we're inside the current tile and this tile isn't being held and no other tile is being held
            if(event->type == SDL_MOUSEBUTTONDOWN && !this->isActive) {
                printf("TILE (%d, %d) PICKED AT POSITION (%d, %d)\n", currentArrayIndex.x, currentArrayIndex.y, destinationRect.x, destinationRect.y);
                this->isActive = true;
            }
        }
    }
}

void Tile::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, &this->sourceRect, &this->destinationRect);
    SDL_RenderDrawRect(renderer, &this->outlineRect);
}