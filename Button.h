//
// Created by Christian Curry on 4/20/16.
//

#ifndef TILEGAME_BUTTON_H
#define TILEGAME_BUTTON_H


#include <SDL_events.h>
#include <SDL_render.h>

class Button {

public:
    Button();

    const int BUTTON_WIDTH = 150;
    const int BUTTON_HEIGHT = 50;

    bool handleEvent(SDL_Event* event);
    void render(SDL_Renderer* renderer);
    void setPosition(int x, int y);

    SDL_Texture* buttonTexture;
    SDL_Rect sourceRect;
    SDL_Rect destinationRect;

private:
    SDL_Point position;
};


#endif //TILEGAME_BUTTON_H
