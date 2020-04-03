#ifndef _DRAW_H_
#define _DRAW_H_

#include <deque>
#include <string>
#include "SDL/SDL.h"
#include "cell.h"
#include "cursor.h"
#include "def.h"

class CAction
{
    public:
    CAction(void) : m_x(0), m_y(0), m_dir(T_DIRECTION_UP), m_s(1) {}
    CAction(const unsigned char p_x, const unsigned char p_y, const T_DIRECTION p_dir, const unsigned char p_s) : m_x(p_x), m_y(p_y), m_dir(p_dir), m_s(p_s) {}
    CAction(const CAction &p_src) : m_x(p_src.m_x), m_y(p_src.m_y), m_dir(p_src.m_dir), m_s(p_src.m_s) {}
    const CAction &operator =(const CAction &p_src) { m_x = p_src.m_x; m_y = p_src.m_y; m_dir = p_src.m_dir; m_s = p_src.m_s; return *this; }
    virtual ~CAction(void) {}
    unsigned char m_x;
    unsigned char m_y;
    T_DIRECTION m_dir;
    unsigned char m_s;
};

class CDraw
{
    public:

    // Constructor
    CDraw(SDL_Surface *p_screen);

    // Destructor
    virtual ~CDraw(void);

    // Handle key inputs
    const bool keyPressed(const SDL_Event &p_event);

    // Handle key hold
    const bool keyHold(void);

    // Display drawn elements on the given surface
    void render(const bool p_renderCursor);

    // Get cursor
    CCursor &getCursor(void);

    // Get a specific cell
    const CCell * const getCell(void) const;

    // Clean drawing and actions history
    void reset(void);

    // Undo last action
    void undo(void);

    // Allocate cells and sets current drawing
    void loadLevel(const std::string &p_drawing = "");

    // Clear all x's from the cells
    void clearXs(void);

    private:

    // Forbidden
    CDraw(void);
    CDraw(const CDraw &p_source);
    const CDraw &operator =(const CDraw &p_source);

    // Switch a line
    void switchLine(const T_DIRECTION p_direction, const unsigned char p_step, const bool p_recordAction);

    // The screen
    SDL_Surface *m_screen;

    // The cells
    CCell *m_cells;

    // The cursor
    CCursor m_cursor;

    // The images for vertical and horizontal lines
    SDL_Surface *m_imageVertical;
    SDL_Surface *m_imageHorizontal;

    // Cross image
    SDL_Surface *m_imageCross;

    // List of past actions, for "undo"
    std::deque<CAction> m_actions;

    // Sounds
    Mix_Chunk *m_soundDraw;
    Mix_Chunk *m_soundUndo;
};

#endif
