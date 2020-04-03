#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "def.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

class CCursor
{
    public:

    // Constructor
    CCursor(SDL_Surface *p_screen);

    // Destructor
    virtual ~CCursor(void);

    // Handle key inputs
    const bool keyPressed(const SDL_Event &p_event);

    // Handle key hold
    const bool keyHold(const T_DIRECTION p_direction, const bool p_held);

    // Display cursor on the given surface
    void render(void);

    // Get attributes
    const unsigned char getX(void) const;
    const unsigned char getY(void) const;

    // Set position
    void goTo(const unsigned char p_x, const unsigned char p_y);

    // Reset
    void reset(void);

    // Update cursor (for animation)
    void update(void);

    private:

    // Forbidden
    CCursor(void);
    CCursor(const CCursor &p_source);
    const CCursor &operator =(const CCursor &p_source);

    // Move the cursor 1 cell
    const bool move(const T_DIRECTION p_direction, const bool p_loop);

    // Adjust the camera
    void adjustCamera(void) const;

    // The screen
    SDL_Surface *m_screen;

    // Coordinates in the matrix
    unsigned char m_x;
    unsigned char m_y;

    // The cursor image
    SDL_Surface *m_image;

    // Timers for key hold
    unsigned int m_timer[4];

    // Sounds
    Mix_Chunk *m_soundMove;

    // Alpha
    Uint8 m_alpha;
    Sint8 m_alphaStep;
};

#endif
