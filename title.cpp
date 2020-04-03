#include <iostream>
#include <iomanip>
#include <sstream>
#include "title.h"
#include "sdlutils.h"
#include "resourceManager.h"
#include "save.h"

CTitle::CTitle(SDL_Surface *p_screen) :
    m_screen(p_screen),
    m_imageBackground(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG_TITLE)),
    m_font(CResourceManager::instance().getFont()),
    m_beatenLevelsEasy(0),
    m_beatenLevelsNormal(0),
    m_beatenLevelsHard(0),
    m_beatenLevelsExpert(0),
    m_continueLevel(0),
    m_saveStateLevel(0),
    m_saveStateDraw(""),
    m_upArrow(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_ARROW_UP_SMALL)),
    m_downArrow(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_ARROW_DOWN_SMALL)),
    m_soundMove(CResourceManager::instance().getSound(CResourceManager::T_SOUND_MOVE)),
    m_STContinue(NULL),
    m_STLevel(NULL)
{
    // Static text render
    m_STContinue = SDL_utils::renderText(m_font, "Continue", Globals::g_textColorRed);
    m_STLevel = SDL_utils::renderText(m_font, "Level", Globals::g_textColorRed);
    // Level information
    this->updateLevelInformation();
}

CTitle::~CTitle(void)
{
}

void CTitle::sdlCleanup(void)
{
    if (m_STContinue != NULL)
    {
        SDL_FreeSurface(m_STContinue);
        m_STContinue = NULL;
    }
    if (m_STLevel != NULL)
    {
        SDL_FreeSurface(m_STLevel);
        m_STLevel = NULL;
    }
}

void CTitle::render(void) const
{
    INHIBIT(std::cout << "CTitle::render" << std::endl;)
    // Draw background image
    SDL_utils::applySurface(0, 0, m_imageBackground, m_screen);

    std::ostringstream l_stream;
    l_stream << m_beatenLevelsEasy << " / " << NB_LEVELS_EASY;
    SDL_utils::applyText(220, 95, m_screen, m_font, l_stream.str(), m_beatenLevelsEasy == NB_LEVELS_EASY ? Globals::g_textColorGreen : Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);

    l_stream.str("");
    l_stream << m_beatenLevelsNormal << " / " << NB_LEVELS_NORMAL;
    SDL_utils::applyText(220, 115, m_screen, m_font, l_stream.str(), m_beatenLevelsNormal == NB_LEVELS_NORMAL ? Globals::g_textColorGreen : Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);

    l_stream.str("");
    l_stream << m_beatenLevelsHard << " / " << NB_LEVELS_HARD;
    SDL_utils::applyText(220, 135, m_screen, m_font, l_stream.str(), m_beatenLevelsHard == NB_LEVELS_HARD ? Globals::g_textColorGreen : Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);

    l_stream.str("");
    l_stream << m_beatenLevelsExpert << " / " << NB_LEVELS_EXPERT;
    SDL_utils::applyText(220, 155, m_screen, m_font, l_stream.str(), m_beatenLevelsHard == NB_LEVELS_EXPERT ? Globals::g_textColorGreen : Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);

    if (m_saveStateLevel)
    {
        SDL_utils::applySurface(241, 211, m_STContinue, m_screen);
    }
    else
    {
        // Display level number
        SDL_utils::applySurface(241, 211, m_STLevel, m_screen);
        l_stream.str("");
        l_stream << std::setfill('0') << std::setw(3) << m_continueLevel;
        SDL_utils::applyText(285, 211, m_screen, m_font, l_stream.str(), Globals::g_textColorRed, SDL_utils::T_TEXT_ALIGN_CENTER);
        // Draw arrow images
        SDL_utils::applySurface(282, 206, m_upArrow, m_screen);
        SDL_utils::applySurface(282, 225, m_downArrow, m_screen);
    }
}

const unsigned int CTitle::getContinueLevel(void) const
{
    return m_continueLevel;
}

const bool CTitle::keyPressed(const SDL_Event &p_event)
{
    bool l_ret(false);
    switch(p_event.key.keysym.sym)
    {
        case MYKEY_DOWN:
            if (m_continueLevel > 1) --m_continueLevel;
            else m_continueLevel = NB_LEVELS;
            l_ret = true;
            break;
        case MYKEY_UP:
            if (m_continueLevel < NB_LEVELS) ++m_continueLevel;
            else m_continueLevel = 1;
            l_ret = true;
            break;
        default:
            break;
    }
    if (l_ret && Globals::g_playSounds)
        Mix_PlayChannel(-1, m_soundMove, 0);
    return l_ret;
}

void CTitle::updateLevelInformation(void)
{
    // Update numbers of beaten levels
    CSave::instance().getBeatenLevels(m_beatenLevelsEasy, m_beatenLevelsNormal, m_beatenLevelsHard, m_beatenLevelsExpert);
    // Update save state information
    CSave::instance().getSaveStateInfo(m_saveStateLevel, m_saveStateDraw);
    // Next level
    m_continueLevel = CSave::instance().getContinueLevel();
    // Reset timers
    m_timer[0] = 0;
    m_timer[1] = 0;
}

const bool CTitle::keyHold(const T_DIRECTION p_direction, const bool p_held)
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
                        if (m_continueLevel < NB_LEVELS)
                        {
                            ++m_continueLevel;
                            l_ret = true;
                        }
                        break;
                    case T_DIRECTION_DOWN:
                        if (m_continueLevel > 1)
                        {
                            --m_continueLevel;
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
        if (m_timer[p_direction]) m_timer[p_direction] = 0;
    }
    if (l_ret && Globals::g_playSounds)
        Mix_PlayChannel(-1, m_soundMove, 0);
    return l_ret;
}

const unsigned int CTitle::getSaveStateLevel(void) const
{
    return m_saveStateLevel;
}

const std::string &CTitle::getSaveStateDraw(void) const
{
    return m_saveStateDraw;
}
