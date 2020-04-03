#include <iostream>
#include "sdlutils.h"
#include "def.h"

// Load an image using SDL_image
SDL_Surface *SDL_utils::loadImage(const std::string &p_filename)
{
    INHIBIT(std::cout << "SDL_utils::loadImage(" << p_filename << ")" << std::endl;)
    // Load image
    SDL_Surface* l_img = IMG_Load(p_filename.c_str());
    SDL_Surface* l_img2 = NULL;
    if(l_img != NULL)
    {
        // Optimize the image
        l_img2 = SDL_DisplayFormat(l_img);
        // Free the first image
        SDL_FreeSurface(l_img);
        // Set magenta = transparent
        if (l_img2 != NULL)
            SDL_SetColorKey(l_img2, SDL_SRCCOLORKEY, SDL_MapRGB(l_img2->format, 0xFF, 0, 0xFF));
    }
    // Check errors
    if (l_img2 == NULL)
        std::cout << "SDL_utils::loadImage: " << SDL_GetError() << std::endl;
    return l_img2;
}

void SDL_utils::applySurface(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_source, SDL_Surface* p_destination, SDL_Rect *p_clip)
{
    // Rectangle to hold the offsets
    SDL_Rect l_offset;
    // Set offsets
    l_offset.x = p_x;
    l_offset.y = p_y;
    //Blit the surface
    SDL_BlitSurface(p_source, p_clip, p_destination, &l_offset);
}

TTF_Font *SDL_utils::loadFont(const std::string &p_font, const int p_size)
{
    INHIBIT(std::cout << "SDL_utils::loadFont(" << p_font << ", " << p_size << ")" << std::endl;)
    TTF_Font *l_font = TTF_OpenFont(p_font.c_str(), p_size);
    if (l_font == NULL)
        std::cout << "SDL_utils::loadFont: " << SDL_GetError() << std::endl;
    return l_font;
}

SDL_Surface *SDL_utils::renderText(TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg)
{
    INHIBIT(std::cout << "SDL_utils::renderText(" << p_text << ")" << std::endl;)
    SDL_Surface* l_img = TTF_RenderText_Solid(p_font, p_text.c_str(), p_fg);
    SDL_Surface* l_img2 = NULL;
    if (l_img != NULL)
    {
        l_img2 = SDL_DisplayFormat(l_img);
        SDL_FreeSurface(l_img);
    }
    if (l_img2 == NULL)
        std::cout << "SDL_utils::renderText: " << SDL_GetError() << std::endl;
    return l_img2;
}

void SDL_utils::applyText(const Sint16 p_x, const Sint16 p_y, SDL_Surface* p_destination, TTF_Font *p_font, const std::string &p_text, const SDL_Color &p_fg, const T_TEXT_ALIGN p_align)
{
    SDL_Surface *l_text = renderText(p_font, p_text, p_fg);
    switch (p_align)
    {
        case T_TEXT_ALIGN_LEFT: applySurface(p_x, p_y, l_text, p_destination); break;
        case T_TEXT_ALIGN_RIGHT: applySurface(p_x - l_text->w, p_y, l_text, p_destination); break;
        case T_TEXT_ALIGN_CENTER: applySurface(p_x - l_text->w / 2, p_y, l_text, p_destination); break;
        default: break;
    }
    SDL_FreeSurface(l_text);
}

Mix_Chunk *SDL_utils::loadSound(const std::string &p_filename)
{
    INHIBIT(std::cout << "SDL_utils::loadSound(" << p_filename << ")" << std::endl;)
    Mix_Chunk *l_sound = Mix_LoadWAV(p_filename.c_str());
    if (l_sound == NULL)
        std::cout << "SDL_utils::loadSound: " << Mix_GetError() << std::endl;
    return l_sound;
}
