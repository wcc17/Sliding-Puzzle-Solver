//
// Created by Christian Curry on 4/20/16.
//

#include "Button.h"

Button::Button() {
}

//returns true if the button was clicked
bool Button::handleEvent(SDL_Event* event) {
    if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP ) {

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if(mouseX < position.x) {
            return false;
        } else if(mouseX > (position.x + BUTTON_WIDTH) ) {
            return false;
        } else if(mouseY < position.y) {
            return false;
        } else if(mouseY > (position.y + BUTTON_HEIGHT) ) {
            return false;
        }

        if(event->type == SDL_MOUSEBUTTONDOWN) {
            setPosition(position.x + 5, position.y + 5);
            return true;
        } else if(event->type == SDL_MOUSEBUTTONUP) {
            setPosition(position.x - 5, position.y - 5);
            return false;
        }
    }

    return false;
}

void Button::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, buttonTexture, &sourceRect, &destinationRect);
}

void Button::setPosition(int x, int y) {
    position.x = x;
    position.y = y;

    sourceRect.w = 150;
    sourceRect.h = 50;
    sourceRect.x = 0;
    sourceRect.y = 0;

    destinationRect.w = 150;
    destinationRect.h = 50;
    destinationRect.x = x;
    destinationRect.y = y;
}