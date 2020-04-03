#ifndef _TITLE_H_
#define _TITLE_H_

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "def.h"

class CTitle
{
    public:

    // Constructor
    CTitle(SDL_Surface *p_screen);

    // Destructor
    virtual ~CTitle(void);

    // Free SDL resources
    void sdlCleanup(void);

    // Display title on the given surface
    void render(void) const;

    // Get continue level
    const unsigned int getContinueLevel(void) const;

    // Handle key inputs
    const bool keyPressed(const SDL_Event &p_event);

    // Handle key hold
    const bool keyHold(const T_DIRECTION p_direction, const bool p_held);

    // Update level information from CSave object
    void updateLevelInformation(void);

    // Accessors to save state data
    const unsigned int getSaveStateLevel(void) const;
    const std::string &getSaveStateDraw(void) const;

    private:

    // Forbidden
    CTitle(void);
    CTitle(const CTitle &p_source);
    const CTitle &operator =(const CTitle &p_source);

    // Screen
    SDL_Surface *m_screen;

    // Background image
    SDL_Surface *m_imageBackground;

    // Fonts
    TTF_Font *m_font;

    // Levels information
    unsigned int m_beatenLevelsEasy;
    unsigned int m_beatenLevelsNormal;
    unsigned int m_beatenLevelsHard;
    unsigned int m_beatenLevelsExpert;
    unsigned int m_continueLevel;
    unsigned int m_saveStateLevel;
    std::string m_saveStateDraw;

    // Timers for key hold
    unsigned int m_timer[2];

    // Up and down arrows
    SDL_Surface *m_upArrow;
    SDL_Surface *m_downArrow;

    // Sounds
    Mix_Chunk *m_soundMove;
    
    // Static text renders
    SDL_Surface *m_STContinue;
    SDL_Surface *m_STLevel;
};

#endif
