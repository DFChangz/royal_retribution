#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "pallete.h"
#include "sprite.h"

class Text : public Sprite {
  public:
    virtual void load();
    virtual void cleanup();

    Text(SDL_Renderer *renderer, std::string font_filename,
    ErrorHandler *error_handler, int pos_x, int pos_y, int font_size,
    std::string text, const SDL_Color &color = WHITE);

    virtual ~Text();

    virtual SDL_Rect* getDestRect();
  private:
    int font_size;
    std::string text;
    const SDL_Color &color;
    SDL_Surface *surface = nullptr;

    TTF_Font *font = nullptr;
};

#endif
