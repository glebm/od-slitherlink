#ifndef _MENUPAUSE_H_
#define _MENUPAUSE_H_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "def.h"

class CMenuPause
{
    public:

    // Constructor
    CMenuPause(SDL_Surface *p_screen);

    // Destructor
    virtual ~CMenuPause(void);

    // Free SDL resources
    void sdlCleanup(void);

    // Display title on the given surface
    void render(void) const;

    // Handle key inputs
    const bool keyPressed(const SDL_Event &p_event);

    // Handle key hold
    const bool keyHold(const T_DIRECTION p_direction, const bool p_held);

    // Get selected item
    const unsigned char getSelected(void) const;

    private:

    // Forbidden
    CMenuPause(void);
    CMenuPause(const CMenuPause &p_source);
    const CMenuPause &operator =(const CMenuPause &p_source);

    // Screen
    SDL_Surface *m_screen;

    // Menu images
    SDL_Surface *m_background;
    SDL_Surface *m_cursor;
    SDL_Surface *m_note[2];

    // Fonts
    TTF_Font *m_font;

    // Sounds
    Mix_Chunk *m_soundMove;

    // Selected item
    unsigned char m_selected;
    SDL_Surface* m_selectedText[NB_ITEMS_PAUSE_MENU];

    // Timers for key hold
    unsigned int m_timer[2];
};

#endif
