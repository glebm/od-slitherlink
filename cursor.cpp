#include <iostream>
#include "cursor.h"
#include "sdlutils.h"
#include "resourceManager.h"

CCursor::CCursor(SDL_Surface *p_screen):
    m_screen(p_screen),
    m_x(0),
    m_y(0),
    m_image(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_CURSOR)),
    m_soundMove(CResourceManager::instance().getSound(CResourceManager::T_SOUND_MOVE)),
    m_alpha(CURSOR_ALPHA_MAX),
    m_alphaStep(-CURSOR_ALPHA_STEP)
{
    // Timers
    m_timer[0] = 0;
    m_timer[1] = 0;
    m_timer[2] = 0;
    m_timer[3] = 0;
}

CCursor::~CCursor(void)
{
}

void CCursor::render(void)
{
    if (m_x >= Globals::g_cameraX && m_x <= Globals::g_cameraX + CELLS_ONSCREEN && m_y >= Globals::g_cameraY && m_y <= Globals::g_cameraY + CELLS_ONSCREEN)
        SDL_utils::applySurface(Globals::g_offsetX + (m_x - Globals::g_cameraX) * CELL_WIDTH, Globals::g_offsetY + (m_y - Globals::g_cameraY) * CELL_WIDTH, m_image, m_screen);
}

const bool CCursor::keyPressed(const SDL_Event &p_event)
{
    bool l_retVal(false);
    // Move the cursor
    switch(p_event.key.keysym.sym)
    {
        case MYKEY_RIGHT:
            l_retVal = this->move(T_DIRECTION_RIGHT, true);
            break;
        case MYKEY_LEFT:
            l_retVal = this->move(T_DIRECTION_LEFT, true);
            break;
        case MYKEY_UP:
            l_retVal = this->move(T_DIRECTION_UP, true);
            break;
        case MYKEY_DOWN:
            l_retVal = this->move(T_DIRECTION_DOWN, true);
            break;
        default:
            break;
    }
    return l_retVal;
}

const unsigned char CCursor::getX(void) const
{
    return m_x;
}

const unsigned char CCursor::getY(void) const
{
    return m_y;
}

void CCursor::goTo(const unsigned char p_x, const unsigned char p_y)
{
    INHIBIT(std::cout << "CCursor::goTo(" << (int)p_x << ", " << (int)p_y << ")" << std::endl;)
    m_x = p_x;
    m_y = p_y;
    // Camera
    if (Globals::g_nbCells > 10)
        this->adjustCamera();
    // Reset transparency
    m_alpha = CURSOR_ALPHA_MAX;
    m_alphaStep = -CURSOR_ALPHA_STEP;
    SDL_SetAlpha(m_image, SDL_SRCALPHA, CURSOR_ALPHA_MAX);
}

const bool CCursor::keyHold(const T_DIRECTION p_direction, const bool p_held)
{
    bool l_ret(false);
    if (p_held)
    {
        if (m_timer[p_direction])
        {
            // Timer running
            --m_timer[p_direction];
            if (m_timer[p_direction] == 0)
            {
                // Trigger!
                l_ret = this->move(p_direction, false);
                // Timer continues
                m_timer[p_direction] = KEYHOLD_TIMER;
            }
        }
        else
        {
            // Start timer
            m_timer[p_direction] = KEYHOLD_TIMER_FIRST;
        }
    }
    else
    {
        // Stop timer if necessary
        if (m_timer[p_direction]) m_timer[p_direction] = 0;
    }
    return l_ret;
}

void CCursor::reset(void)
{
    INHIBIT(std::cout << "CCursor::reset" << std::endl;)
    m_x = 0;
    m_y = 0;
    m_timer[0] = 0;
    m_timer[1] = 0;
    m_timer[2] = 0;
    m_timer[3] = 0;
    Globals::g_cameraX = 0;
    Globals::g_cameraY = 0;
    m_alpha = CURSOR_ALPHA_MAX;
    m_alphaStep = -CURSOR_ALPHA_STEP;
    SDL_SetAlpha(m_image, SDL_SRCALPHA, CURSOR_ALPHA_MAX);
}

const bool CCursor::move(const T_DIRECTION p_direction, const bool p_loop)
{
    bool l_ret(false);
    switch (p_direction)
    {
        case T_DIRECTION_UP:
            if (m_y == 0)
            {
                if (p_loop)
                {
                    m_y = Globals::g_nbCells - 1;
                    l_ret = true;
                }
            }
            else
            {
                --m_y;
                l_ret = true;
            }
            break;
        case T_DIRECTION_RIGHT:
            if (m_x == Globals::g_nbCells - 1)
            {
                if (p_loop)
                {
                    m_x = 0;
                    l_ret = true;
                }
            }
            else
            {
                ++m_x;
                l_ret = true;
            }
            break;
        case T_DIRECTION_DOWN:
            if (m_y == Globals::g_nbCells - 1)
            {
                if (p_loop)
                {
                    m_y = 0;
                    l_ret = true;
                }
            }
            else
            {
                ++m_y;
                l_ret = true;
            }
            break;
        case T_DIRECTION_LEFT:
            if (m_x == 0)
            {
                if (p_loop)
                {
                    m_x = Globals::g_nbCells - 1;
                    l_ret = true;
                }
            }
            else
            {
                --m_x;
                l_ret = true;
            }
            break;
            break;
        default:
            break;
    }
    if (l_ret)
    {
        // Camera
        if (Globals::g_nbCells > 10)
            this->adjustCamera();
        // Reset transparency
        m_alpha = CURSOR_ALPHA_MAX;
        m_alphaStep = -CURSOR_ALPHA_STEP;
        SDL_SetAlpha(m_image, SDL_SRCALPHA, CURSOR_ALPHA_MAX);
        // Sound
        if (Globals::g_playSounds)
            Mix_PlayChannel(-1, m_soundMove, 0);
    }
    return l_ret;
}

void CCursor::adjustCamera(void) const
{
    // Adjust x
    if (m_x < Globals::g_cameraX) Globals::g_cameraX = m_x;
    else if (m_x > Globals::g_cameraX + CELLS_ONSCREEN) Globals::g_cameraX = m_x - CELLS_ONSCREEN;
    // Adjust Y
    if (m_y < Globals::g_cameraY) Globals::g_cameraY = m_y;
    else if (m_y > Globals::g_cameraY + CELLS_ONSCREEN) Globals::g_cameraY = m_y - CELLS_ONSCREEN;
    INHIBIT(std::cout << "Cursor [ " << (int)m_x << ", " << (int)m_y << "]  -  Camera [" << (int)Globals::g_cameraX << ", " << (int)Globals::g_cameraY << "]" << std::endl;)
}

void CCursor::update(void)
{
    // Update transparency
    m_alpha += m_alphaStep;
    if (m_alpha > CURSOR_ALPHA_MAX)
    {
        m_alpha = CURSOR_ALPHA_MAX;
        m_alphaStep = -CURSOR_ALPHA_STEP;
    }
    else if (m_alpha < CURSOR_ALPHA_MIN)
    {
        m_alpha = CURSOR_ALPHA_MIN;
        m_alphaStep = CURSOR_ALPHA_STEP;
    }
    SDL_SetAlpha(m_image, SDL_SRCALPHA, m_alpha);
}
