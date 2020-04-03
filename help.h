#ifndef _HELP_H_
#define _HELP_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"

class CHelp
{
    public:

    // Constructor
    CHelp(SDL_Surface *p_screen);

    // Destructor
    virtual ~CHelp(void);

    // Display title on the given surface
    void render(void) const;

    // Handle key inputs
    const bool keyPressed(const SDL_Event &p_event, bool &p_quit);

    // Go to a given page
    void setPage(const unsigned char p_page);

    // Get the current page
    const unsigned char getPage(void) const;

    private:

    // Forbidden
    CHelp(void);
    CHelp(const CHelp &p_source);
    const CHelp &operator =(const CHelp &p_source);

    // Screen
    SDL_Surface *m_screen;

    // Font
    TTF_Font *m_textFont;

    // Background image
    SDL_Surface *m_background[3];

    // Current page
    unsigned char m_page;

    // Sound
    Mix_Chunk *m_soundMove;
};

#endif
