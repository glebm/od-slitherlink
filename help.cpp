#include <iostream>
#include <sstream>
#include "help.h"
#include "def.h"
#include "sdlutils.h"
#include "resourceManager.h"

CHelp::CHelp(SDL_Surface *p_screen) :
    m_screen(p_screen),
    m_textFont(CResourceManager::instance().getFont()),
    m_page(0),
    m_soundMove(CResourceManager::instance().getSound(CResourceManager::T_SOUND_MOVE))
{
    m_background[0] = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_HELP_1);
    m_background[1] = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_HELP_2);
    m_background[2] = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_HELP_3);
}

CHelp::~CHelp(void)
{
}

void CHelp::render(void) const
{
    INHIBIT(std::cout << "CHelp::render" << std::endl;)
    // Draw background image
    SDL_utils::applySurface(0, 0, m_background[m_page], m_screen);
}

const bool CHelp::keyPressed(const SDL_Event &p_event, bool &p_quit)
{
    bool l_ret(false);
    p_quit = false;
    switch (p_event.key.keysym.sym)
    {
        case MYKEY_LINERIGHT:
        case MYKEY_LINEDOWN:
        case MYKEY_RIGHT:
        case MYKEY_START:
            if (m_page == 2)
            {
                m_page = 0;
                p_quit = true;
            }
            else
                ++m_page;
            l_ret = true;
            break;
        case MYKEY_LEFT:
            if (m_page)
            {
                --m_page;
                l_ret = true;
            }
            break;
        default:
            break;
    }
    if (l_ret && !p_quit && Globals::g_playSounds)
        Mix_PlayChannel(-1, m_soundMove, 0);
    return l_ret;
}

void CHelp::setPage(const unsigned char p_page)
{
    m_page = p_page;
}

const unsigned char CHelp::getPage(void) const
{
    return m_page;
}
