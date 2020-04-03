#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

namespace SDL_utils
{
    // Text alignment
    typedef enum
    {
        T_TEXT_ALIGN_LEFT = 0,
        T_TEXT_ALIGN_RIGHT,
        T_TEXT_ALIGN_CENTER
    }
    T_TEXT_ALIGN;

    // Load an image using SDL_image
    SDL_Surface *loadImage(const std::string &p_filename);

    // Load a TTF font
    TTF_Font *loadFont(const std::string &p_font, const int p_size);

    // Load a sound
    Mix_Chunk *loadSound(const std::string &p_filename);

    // Apply a surface on another surface
    void applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect *p_clip = NULL);

    // Render a text in the given font
    SDL_Surface *renderText(TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg);

    // Render a text and apply on a given surface
    void applyText(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_destination, TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg, const T_TEXT_ALIGN p_align = T_TEXT_ALIGN_LEFT);
}

#endif
