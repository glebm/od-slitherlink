#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "cell.h"
#include "draw.h"
#include "def.h"
#include "save.h"

class CPoint
{
    public:
    CPoint(void) : m_x(0), m_y(0) {}
    CPoint(const unsigned char p_x, const unsigned char p_y) : m_x(p_x), m_y(p_y) {}
    CPoint(const CPoint &p_source) : m_x(p_source.m_x), m_y(p_source.m_y) {}
    const CPoint &operator =(const CPoint &p_source) { m_x = p_source.m_x; m_y = p_source.m_y; return *this;}
    virtual ~CPoint(void) {}
    unsigned char m_x;
    unsigned char m_y;
};

class CLevel
{
    public:

    // Constructor
    CLevel(SDL_Surface *p_screen);

    // Destructor
    virtual ~CLevel(void);

    // Free SDL resources
    void sdlCleanup(void);

    // Display level on the given surface
    void render(const CDraw &p_drawing) const;

    // Load a specified level
    void loadLevel(const unsigned int p_number);

    // Check if the given drawing is the answer
    const bool solutionFound(const CDraw &p_drawing);

    // Reset level
    void reset(void);

    // Get level number
    const unsigned int getLevelNumber(void) const;

    private:

    // Forbidden
    CLevel(void);
    CLevel(const CLevel &p_source);
    const CLevel &operator =(const CLevel &p_source);

    // Screen
    SDL_Surface *m_screen;

    // Background image
    SDL_Surface *m_imageBackground;
    
    // Borders
    SDL_Surface *m_borderH;
    SDL_Surface *m_borderV;

    // Current level number
    unsigned int m_levelNumber;

    // Images for the numbers 0, 1, 2, 3 in normal and alternative color
    SDL_Surface *m_imageNumberNormal[4];
    SDL_Surface *m_imageNumberAlt[4];

    // Solution of the current level
    CCell *m_cells;

    // Number hints of the current levels
    std::vector<CPoint> m_hints;

    // Font used for text
    TTF_Font *m_font;

    // The level is solved, or not
    bool m_solved;

    // Static text renders
    SDL_Surface *m_STLevel;
    SDL_Surface *m_STLevelNumber;
    SDL_Surface *m_STSolved;
};

#endif
