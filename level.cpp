#include <iostream>
#include <iomanip>
#include <sstream>
#include "level.h"
#include "sdlutils.h"
#include "def.h"
#include "resourceManager.h"

CLevel::CLevel(SDL_Surface *p_screen) :
    m_screen(p_screen),
    m_imageBackground(NULL),
    m_borderH(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BORDER_H)),
    m_borderV(CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BORDER_V)),
    m_levelNumber(0),
    m_cells(NULL),
    m_font(CResourceManager::instance().getFont()),
    m_solved(false),
    m_STLevel(NULL),
    m_STLevelNumber(NULL),
    m_STSolved(NULL)
{
    // Load number images for 0, 1, 2, 3
    for (int l_i = 0; l_i <= 3; ++l_i)
    {
        std::ostringstream l_s;
        l_s << l_i;
        m_imageNumberNormal[l_i] = SDL_utils::renderText(m_font, l_s.str(), Globals::g_textColorBlack);
        m_imageNumberAlt[l_i] = SDL_utils::renderText(m_font, l_s.str(), Globals::g_textColorBlue);
    }
    // Render static text
    m_STLevel = SDL_utils::renderText(m_font, "Level", Globals::g_textColorBlack);
    m_STSolved = SDL_utils::renderText(m_font, "Solved", Globals::g_textColorGreen);
}

CLevel::~CLevel(void)
{
}

void CLevel::sdlCleanup(void)
{
    INHIBIT(std::cout << "CLevel::sdlCleanup" << std::endl;)
    // Free number images for 0, 1, 2, 3
    for (int l_i = 0; l_i <= 3; ++l_i)
    {
        if (m_imageNumberNormal[l_i] != NULL)
        {
            SDL_FreeSurface(m_imageNumberNormal[l_i]);
            m_imageNumberNormal[l_i] = NULL;
        }
        if (m_imageNumberAlt[l_i] != NULL)
        {
            SDL_FreeSurface(m_imageNumberAlt[l_i]);
            m_imageNumberAlt[l_i] = NULL;
        }
    }
    // Free static text surfaces
    if (m_STLevel != NULL)
    {
        SDL_FreeSurface(m_STLevel);
        m_STLevel = NULL;
    }
    if (m_STLevelNumber != NULL)
    {
        SDL_FreeSurface(m_STLevelNumber);
        m_STLevelNumber = NULL;
    }
    if (m_STSolved != NULL)
    {
        SDL_FreeSurface(m_STSolved);
        m_STSolved = NULL;
    }
    // Delete cells
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
}

void CLevel::render(const CDraw &p_drawing) const
{
    // Draw background
    SDL_utils::applySurface(0, 0, m_imageBackground, m_screen);
    // Draw level information
    SDL_utils::applySurface(8, 15, m_STLevel, m_screen);
    SDL_utils::applySurface(8, 15 + TEXT_GAP, m_STLevelNumber, m_screen);
    if (m_solved)
        SDL_utils::applySurface(8, 15 + 2 * TEXT_GAP, m_STSolved, m_screen);
    // Draw arrows and borders
    if (Globals::g_nbCells >= 10)
    {
        if (Globals::g_cameraX + 1 + CELLS_ONSCREEN >= Globals::g_nbCells)
            SDL_utils::applySurface(265, 15, m_borderV, m_screen);
        if (!Globals::g_cameraX)
            SDL_utils::applySurface(54, 15, m_borderV, m_screen);
        if (Globals::g_cameraY + 1 + CELLS_ONSCREEN >= Globals::g_nbCells)
            SDL_utils::applySurface(55, 225, m_borderH, m_screen);
        if (!Globals::g_cameraY)
            SDL_utils::applySurface(55, 14, m_borderH, m_screen);
    }
    // Draw hints
    SDL_Surface *l_number(NULL);
    for (std::vector<CPoint>::const_iterator l_it = m_hints.begin(); l_it != m_hints.end(); ++l_it)
    {
        if (l_it->m_x >= Globals::g_cameraX && l_it->m_x <= Globals::g_cameraX + CELLS_ONSCREEN && l_it->m_y >= Globals::g_cameraY && l_it->m_y <= Globals::g_cameraY + CELLS_ONSCREEN)
        {
            // The cell is inside the camera => we draw it
            if (p_drawing.getCell()[l_it->m_x * Globals::g_nbCells + l_it->m_y].getNbLines() == m_cells[l_it->m_x * Globals::g_nbCells + l_it->m_y].getNbLines())
                l_number = m_imageNumberAlt[m_cells[l_it->m_x * Globals::g_nbCells + l_it->m_y].getNbLines()];
            else
                l_number = m_imageNumberNormal[m_cells[l_it->m_x * Globals::g_nbCells + l_it->m_y].getNbLines()];
            SDL_utils::applySurface(Globals::g_offsetX + (l_it->m_x - Globals::g_cameraX) * CELL_WIDTH + (CELL_WIDTH / 2) - (l_number->w / 2), Globals::g_offsetY + (l_it->m_y - Globals::g_cameraY) * CELL_WIDTH + (CELL_WIDTH / 2) - (l_number->h / 2), l_number, m_screen);
        }
    }
}

void CLevel::loadLevel(const unsigned int p_number)
{
    INHIBIT(std::cout << "CLevel::loadLevel(" << p_number << ")" << std::endl;)
    int l_i(0);
    int l_val(0);
    // New level number
    m_levelNumber = p_number;
    // Get level data
    std::string l_level = CSave::instance().getLevelSolution(m_levelNumber);
    std::string l_hints = CSave::instance().getLevelHints(m_levelNumber);
    // Update nb cells
    switch (l_level.size())
    {
        case 25:
            // 5x5 puzzle
            Globals::g_nbCells = 5;
            Globals::g_offsetX = 108;
            Globals::g_offsetY = 68;
            m_imageBackground = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG_5);
            break;
        case 49:
            // 7x7 puzzle
            Globals::g_nbCells = 7;
            Globals::g_offsetX = 87;
            Globals::g_offsetY = 47;
            m_imageBackground = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG_7);
            break;
        case 100:
            // 10x10 puzzle
            Globals::g_nbCells = 10;
            Globals::g_offsetX = 56;
            Globals::g_offsetY = 16;
            m_imageBackground = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG_10);
            break;
        case 400:
            // 20x20 puzzle
            Globals::g_nbCells = 20;
            Globals::g_offsetX = 56;
            Globals::g_offsetY = 16;
            m_imageBackground = CResourceManager::instance().getSurface(CResourceManager::T_SURFACE_BG_10);
            break;
        default:
            std::cout << "Unexpected level length: " << l_level.size() << std::endl;
            break;
    }
    // Allocate cells
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
    m_cells = new CCell[Globals::g_nbCells * Globals::g_nbCells];
    // Decode level
    for (l_i = 0; l_i < Globals::g_nbCells * Globals::g_nbCells; ++l_i)
    {
        std::istringstream l_stream(l_level.substr(l_i, 1));
        l_stream >> std::hex;
        l_stream >> l_val;
        m_cells[l_i].setCellValue(l_val);
    }
    // Decode hints
    m_hints.clear();
    l_val = l_hints.size() - 2;
    for (l_i = 0; l_i <= l_val; l_i += 2)
        m_hints.push_back(CPoint(l_hints[l_i] - '0', l_hints[l_i + 1] - '0'));
    // Check whether the level was solved before
    m_solved = CSave::instance().levelIsBeaten(m_levelNumber);
    // Render static text: level number
    {
        std::ostringstream l_stream;
        l_stream << std::setfill('0') << std::setw(3) << m_levelNumber;
        if (m_STLevelNumber != NULL)
        {
            SDL_FreeSurface(m_STLevelNumber);
            m_STLevelNumber = NULL;
        }
        m_STLevelNumber = SDL_utils::renderText(m_font, l_stream.str(), Globals::g_textColorBlack);
    }
}

const bool CLevel::solutionFound(const CDraw &p_drawing)
{
    INHIBIT(std::cout << "CLevel::solutionFound" << std::endl;)
    bool l_ret(true);
    if (m_levelNumber > 0)
    {
        int l_i(0);
        int l_j(0);
        for (l_i = 0; l_ret && l_i < Globals::g_nbCells; ++l_i)
        {
            for (l_j = 0; l_ret && l_j < Globals::g_nbCells; ++l_j)
            {
                if (!(p_drawing.getCell()[l_i * Globals::g_nbCells + l_j] == m_cells[l_i * Globals::g_nbCells + l_j]))
                    l_ret = false;
            }
        }
    }
    else
    {
        l_ret = false;
    }
    // Update 'solved' status if necessary
    if (l_ret && !m_solved)
        m_solved = true;
    return l_ret;
}

void CLevel::reset(void)
{
    INHIBIT(std::cout << "CLevel::reset" << std::endl;)
    // Free cells
    if (m_cells != NULL)
    {
        delete [] m_cells;
        m_cells = NULL;
    }
    // Reset hints
    m_hints.clear();
    // Reset level number
    m_levelNumber = 0;
}

const unsigned int CLevel::getLevelNumber(void) const
{
    return m_levelNumber;
}
