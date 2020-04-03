#include <iostream>
#include <sstream>
#include "draw.h"
#include "sdlutils.h"
#include "resourceManager.h"

CDraw::CDraw(SDL_Surface *p_screen) :
    m_screen(p_screen),
    m_cells(NULL),
    m_cursor(p_screen),
    m_imageVertical(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_LINE_VERT)),
    m_imageHorizontal(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_LINE_HORI)),
    m_imageCross(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_CROSS)),
    m_soundDraw(CResourceManager::instance().getSound(CResourceManager::T_SOUND_DRAW)),
    m_soundUndo(CResourceManager::instance().getSound(CResourceManager::T_SOUND_UNDO))
{
}

CDraw::~CDraw(void)
{
    // Delete cells
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
}

const bool CDraw::keyPressed(const SDL_Event &p_event)
{
    unsigned char l_step(1);
    if (SDL_GetKeyState(NULL)[MYKEY_MOD]) l_step = 2;
    // Move cursor
    bool l_retVal = m_cursor.keyPressed(p_event);
    // Switch corresponding line
    if (!l_retVal)
    {
        switch(p_event.key.keysym.sym)
        {
            case MYKEY_LINEUP:
                this->switchLine(T_DIRECTION_UP, l_step, true);
                l_retVal = true;
                break;
            case MYKEY_LINEDOWN:
                this->switchLine(T_DIRECTION_DOWN, l_step, true);
                l_retVal = true;
                break;
            case MYKEY_LINELEFT:
                this->switchLine(T_DIRECTION_LEFT, l_step, true);
                l_retVal = true;
                break;
            case MYKEY_LINERIGHT:
                this->switchLine(T_DIRECTION_RIGHT, l_step, true);
                l_retVal = true;
                break;
            case MYKEY_UNDO:
                // Undo last action
                this->undo();
                l_retVal = true;
                break;
            default:
                break;
        }
    }
    return l_retVal;
}

void CDraw::render(const bool p_renderCursor)
{
    // Draw cursor
    if (p_renderCursor)
        m_cursor.render();
    // Draw lines
    unsigned int l_i(0);
    unsigned int l_j(0);
    unsigned int l_limitX = Globals::g_cameraX + CELLS_ONSCREEN;
    if (l_limitX + 1 > Globals::g_nbCells)
        l_limitX = Globals::g_nbCells - 1;
    unsigned int l_limitY = Globals::g_cameraY + CELLS_ONSCREEN;
    if (l_limitY + 1 > Globals::g_nbCells)
        l_limitY = Globals::g_nbCells - 1;
    for (l_i = Globals::g_cameraX; l_i <= l_limitX; ++l_i)
    {
        for (l_j = Globals::g_cameraY; l_j <= l_limitY; ++l_j)
        {
            switch (m_cells[l_i * Globals::g_nbCells + l_j].getLineState(T_DIRECTION_UP))
            {
                case 1: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH - 2, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH - 3, m_imageHorizontal, m_screen); break;
                case 2: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH + (CELL_WIDTH / 2) - 4, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH - 4, m_imageCross, m_screen); break;
                default : break;
            }
            switch (m_cells[l_i * Globals::g_nbCells + l_j].getLineState(T_DIRECTION_LEFT))
            {
                case 1: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH - 3, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH - 2, m_imageVertical, m_screen); break;
                case 2: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH - 4, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH + (CELL_WIDTH / 2) - 4, m_imageCross, m_screen); break;
                default: break;
            }
            // Draw right line only for the right-most column
            if (l_i == l_limitX)
            {
                switch (m_cells[l_i * Globals::g_nbCells + l_j].getLineState(T_DIRECTION_RIGHT))
                {
                    case 1: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH - 2 + CELL_WIDTH, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH - 2, m_imageVertical, m_screen); break;
                    case 2: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH + CELL_WIDTH - 4, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH + (CELL_WIDTH / 2) - 4, m_imageCross, m_screen); break;
                    default: break;
                }
            }
            // Draw bottom line only for the bottom row
            if (l_j == l_limitY)
            {
                switch (m_cells[l_i * Globals::g_nbCells + l_j].getLineState(T_DIRECTION_DOWN))
                {
                    case 1: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH - 2, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH - 2 + CELL_WIDTH, m_imageHorizontal, m_screen); break;
                    case 2: SDL_utils::applySurface(Globals::g_offsetX + (l_i - Globals::g_cameraX) * CELL_WIDTH + (CELL_WIDTH / 2) - 4, Globals::g_offsetY + (l_j - Globals::g_cameraY) * CELL_WIDTH + CELL_WIDTH - 4, m_imageCross, m_screen); break;
                    default: break;
                }
            }
        }
    }
}

void CDraw::switchLine(const T_DIRECTION p_direction, const unsigned char p_step, const bool p_recordAction)
{
    switch (p_direction)
    {
        case T_DIRECTION_UP:
            m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_UP, p_step);
            if (m_cursor.getY() > 0) m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY() - 1].switchLine(T_DIRECTION_DOWN, p_step);
            break;
        case T_DIRECTION_DOWN:
            m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_DOWN, p_step);
            if (m_cursor.getY() < Globals::g_nbCells - 1) m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY() + 1].switchLine(T_DIRECTION_UP, p_step);
            break;
        case T_DIRECTION_RIGHT:
            m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_RIGHT, p_step);
            if (m_cursor.getX() < Globals::g_nbCells - 1) m_cells[(m_cursor.getX() + 1) * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_LEFT, p_step);
            break;
        case T_DIRECTION_LEFT:
            m_cells[m_cursor.getX() * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_LEFT, p_step);
            if (m_cursor.getX() > 0) m_cells[(m_cursor.getX() - 1) * Globals::g_nbCells + m_cursor.getY()].switchLine(T_DIRECTION_RIGHT, p_step);
            break;
        default:
            break;
    }
    if (p_recordAction)
    {
        // Add action to the history
        if (m_actions.size() == MAX_UNDO_ACTIONS)
            m_actions.pop_front();
        m_actions.push_back(CAction(m_cursor.getX(), m_cursor.getY(), p_direction, p_step));
        if (Globals::g_playSounds)
            Mix_PlayChannel(-1, m_soundDraw, 0);
    }
    else
    {
        if (Globals::g_playSounds)
            Mix_PlayChannel(-1, m_soundUndo, 0);
    }
    INHIBIT(std::cout << "Actions: " << m_actions.size() << std::endl;)
}

CCursor &CDraw::getCursor(void)
{
    return m_cursor;
}

const CCell * const CDraw::getCell(void) const
{
    return m_cells;
}

void CDraw::reset(void)
{
    INHIBIT(std::cout << "CDraw::reset" << std::endl;)
    // Free cells
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
    // Reset action history
    m_actions.clear();
    // Reset cursor
    m_cursor.reset();
}

void CDraw::undo(void)
{
    // Undo last action
    if (m_actions.size() > 0)
    {
        m_cursor.goTo(m_actions.back().m_x, m_actions.back().m_y);
        this->switchLine(m_actions.back().m_dir, 3 - m_actions.back().m_s, false);
        m_actions.pop_back();
    }
}

const bool CDraw::keyHold(void)
{
    bool l_ret = m_cursor.keyHold(T_DIRECTION_UP, SDL_GetKeyState(NULL)[MYKEY_UP]);
    l_ret = m_cursor.keyHold(T_DIRECTION_DOWN, SDL_GetKeyState(NULL)[MYKEY_DOWN]) || l_ret;
    l_ret = m_cursor.keyHold(T_DIRECTION_LEFT, SDL_GetKeyState(NULL)[MYKEY_LEFT]) || l_ret;
    l_ret = m_cursor.keyHold(T_DIRECTION_RIGHT, SDL_GetKeyState(NULL)[MYKEY_RIGHT]) || l_ret;
    return l_ret;
}

void CDraw::loadLevel(const std::string &p_drawing)
{
    INHIBIT(std::cout << "CDraw::loadLevel(" << p_drawing << ")" << std::endl;)
    // Allocate cells
    const int l_nbTotal(Globals::g_nbCells * Globals::g_nbCells);
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
    m_cells = new CCell[l_nbTotal];
    // Sets current drawing if necessary
    if (!p_drawing.empty())
    {
        // Decode level
        int l_val(0);
        int l_index(0);
        for (int l_i = 0; l_i < l_nbTotal; ++l_i)
        {
            std::istringstream l_stream(p_drawing.substr(l_index, 2));
            l_stream >> std::hex;
            l_stream >> l_val;
            m_cells[l_i].setCellValue(l_val);
            l_index += 2;
        }
    }
}

void CDraw::clearXs(void)
{
    if (m_cells != NULL)
    {
        const int l_nbTotal(Globals::g_nbCells * Globals::g_nbCells);
        for (int l_i = 0; l_i < l_nbTotal; ++l_i)
            m_cells[l_i].clearXs();
    }
}
