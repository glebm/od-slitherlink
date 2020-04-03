#include <iostream>
#include "menuPause.h"
#include "resourceManager.h"
#include "sdlutils.h"

CMenuPause::CMenuPause(SDL_Surface *p_screen) :
    m_screen(p_screen),
    m_background(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_MENU_PAUSE)),
    m_cursor(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_MENU_CURSOR)),
    m_font(CResourceManager::instance().getFont()),
    m_soundMove(CResourceManager::instance().getSound(CResourceManager::T_SOUND_MOVE)),
    m_selected(0)
{
    // Note images
    m_note[0] = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_NOTE);
    m_note[1] = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_NOTE_SELECTED);
    // Render static text
    m_selectedText[0] = SDL_utils::renderText(m_font, "Retry", Globals::g_textColorRed);
    m_selectedText[1] = SDL_utils::renderText(m_font, "Save state", Globals::g_textColorRed);
    m_selectedText[2] = SDL_utils::renderText(m_font, "Sound", Globals::g_textColorRed);
    m_selectedText[3] = SDL_utils::renderText(m_font, "Help", Globals::g_textColorRed);
    m_selectedText[4] = SDL_utils::renderText(m_font, "Quit game", Globals::g_textColorRed);
    // Init timers
    m_timer[0] = 0;
    m_timer[1] = 0;
}

CMenuPause::~CMenuPause(void)
{
}

void CMenuPause::sdlCleanup(void)
{
    // Free static text images
    for (int l_i = 0; l_i < NB_ITEMS_PAUSE_MENU; ++l_i)
    {
        if (m_selectedText[l_i] != NULL)
        {
            SDL_FreeSurface(m_selectedText[l_i]);
            m_selectedText[l_i] = NULL;
        }
    }
}

void CMenuPause::render(void) const
{
    INHIBIT(std::cout << "CMenuPause::render" << std::endl;)
    // Draw background
    SDL_utils::applySurface(116, 75, m_background, m_screen);
    // Draw cursor
    SDL_utils::applySurface(125, 84 + m_selected * TEXT_GAP, m_cursor, m_screen);
    // Draw selected text in red
    SDL_utils::applySurface(140, 82 + m_selected * TEXT_GAP, m_selectedText[m_selected], m_screen);
    // Draw note if sound is enabled
    if (Globals::g_playSounds)
        SDL_utils::applySurface(175, 116, m_note[m_selected == 2], m_screen);
}

const bool CMenuPause::keyPressed(const SDL_Event &p_event)
{
    bool l_ret(false);
    switch(p_event.key.keysym.sym)
    {
        case MYKEY_DOWN:
            if (m_selected < NB_ITEMS_PAUSE_MENU - 1)
                ++m_selected;
            else
                m_selected = 0;
            l_ret = true;
            break;
        case MYKEY_UP:
            if (m_selected > 0)
                --m_selected;
            else
                m_selected = NB_ITEMS_PAUSE_MENU - 1;
            l_ret = true;
            break;
        default:
            break;
    }
    if (l_ret && Globals::g_playSounds)
        Mix_PlayChannel(-1, m_soundMove, 0);
    return l_ret;
}

const unsigned char CMenuPause::getSelected(void) const
{
    return m_selected;
}

const bool CMenuPause::keyHold(const T_DIRECTION p_direction, const bool p_held)
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
                switch (p_direction)
                {
                    case T_DIRECTION_UP:
                        if (m_selected > 0)
                        {
                            --m_selected;
                            l_ret = true;
                        }
                        break;
                    case T_DIRECTION_DOWN:
                        if (m_selected < NB_ITEMS_PAUSE_MENU - 1)
                        {
                            ++m_selected;
                            l_ret = true;
                        }
                        break;
                    default:
                        break;
                }
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
        if (m_timer[p_direction])
            m_timer[p_direction] = 0;
    }
    if (l_ret && Globals::g_playSounds)
        Mix_PlayChannel(-1, m_soundMove, 0);
    return l_ret;
}
