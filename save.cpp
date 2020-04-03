#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include "save.h"
#include "def.h"

CSave& CSave::instance(void)
{
    static CSave l_singleton;
    return l_singleton;
}

CSave::CSave(void):
    m_save("")
{
    // Load save file
    {
        std::ifstream l_file(SAVE_FILE);
        if (l_file.is_open())
        {
            INHIBIT(std::cout << "File " << SAVE_FILE << " opened successfully for reading" << std::endl;)
            std::getline(l_file, m_save);
            l_file.close();
        }
        else
        {
            INHIBIT(std::cout << "Unable to open file " << SAVE_FILE << " for reading" << std::endl;)
            // Create save string with zeros
            m_save.insert(0, NB_LEVELS, '0');
        }
        INHIBIT(std::cout << "Save line: " << m_save << std::endl;)
    }
    // Load level data
    {
        std::ifstream l_file(LEVELDATA_FILE);
        if (l_file.is_open())
        {
            std::string l_line("");
            size_t l_i(0);
            while (!l_file.eof())
            {
                std::getline(l_file, l_line);
                if (!l_line.empty() && l_line.at(0) != '#')
                {
                    l_i = l_line.find(' ');
                    m_levelSolutions.push_back(l_line.substr(0, l_i));
                    m_levelHints.push_back(l_line.substr(l_i + 1));
                }
            }
            l_file.close();
        }
        else
            std::cout << "Error: unable to open file " << LEVELDATA_FILE << std::endl;
        INHIBIT(std::cout << "Levels loaded: " <<  m_levelSolutions.size() << " - " << m_levelHints.size() << " / " << NB_LEVELS << std::endl;)
    }
    // Read config file
    {
        std::ifstream l_file(CONFIG_FILE);
        if (l_file.is_open())
        {
            INHIBIT(std::cout << "File " << CONFIG_FILE << " opened successfully for reading" << std::endl;)
            std::string l_line("");
            // Sound option
            std::getline(l_file, l_line);
            INHIBIT(std::cout << "Sound option = " << l_line << std::endl;)
            Globals::g_playSounds = (l_line == "1");
            // End
            l_file.close();
        }
        else
        {
            INHIBIT(std::cout << "Unable to open file " << CONFIG_FILE << " for reading => default values" << std::endl;)
            // Default values
            // Sound option
            Globals::g_playSounds = true;
        }
    }
}

CSave::~CSave(void)
{
}

void CSave::levelBeaten(const unsigned int p_nb)
{
    INHIBIT(std::cout << "CSave::levelBeaten" << std::endl;)
    // Set level to "beaten"
    m_save.at(p_nb - 1) = '1';
    // Save file
    std::ofstream l_file(SAVE_FILE);
    if (l_file.is_open())
    {
        INHIBIT(std::cout << "File " << SAVE_FILE << " opened successfully for writing" << std::endl;)
        l_file << m_save;
        l_file.close();
    }
    INHIBIT(else std::cout << "Unable to open file " << SAVE_FILE << " for writing" << std::endl;)
}

const unsigned int CSave::getContinueLevel(const unsigned int p_start) const
{
    unsigned int l_i(p_start - 1);
    while (l_i < NB_LEVELS && m_save[l_i] != '0')
        ++l_i;
    if (l_i == NB_LEVELS)
        l_i = NB_LEVELS - 1;
    return l_i + 1;
}

void CSave::getBeatenLevels(unsigned int &p_easy, unsigned int &p_normal, unsigned int &p_hard, unsigned int &p_expert) const
{
    p_easy = 0;
    p_normal = 0;
    p_hard = 0;
    p_expert = 0;
    for (unsigned int l_i(0); l_i < NB_LEVELS; ++l_i)
    {
        if (l_i < NB_LEVELS_EASY)
        {
            if (m_save.at(l_i) == '1')
                ++p_easy;
        }
        else if (l_i < NB_LEVELS_EASY + NB_LEVELS_NORMAL)
        {
            if (m_save.at(l_i) == '1')
                ++p_normal;
        }
        else if (l_i < NB_LEVELS_EASY + NB_LEVELS_NORMAL + NB_LEVELS_HARD)
        {
            if (m_save.at(l_i) == '1')
                ++p_hard;
        }
        else
        {
            if (m_save.at(l_i) == '1')
                ++p_expert;
        }
    }
    INHIBIT(std::cout << "CSave::getBeatenLevels " << p_easy << " " << p_normal << " " << p_hard << " " << p_expert << std::endl;)
}

const bool CSave::levelIsBeaten(const unsigned int p_nb) const
{
    return m_save.at(p_nb - 1) == '1';
}

const bool CSave::firstTimePlay(void) const
{
    bool l_ret(true);
    std::ifstream l_file(SAVE_FILE);
    std::ifstream l_file2(SAVE_STATE_FILE);
    if (l_file.is_open())
    {
        l_ret = false;
        l_file.close();
    }
    if (l_file2.is_open())
    {
        l_ret = false;
        l_file2.close();
    }
    return l_ret;
}

const std::string &CSave::getLevelSolution(const unsigned int p_nb) const
{
    return m_levelSolutions.at(p_nb - 1);
}

const std::string &CSave::getLevelHints(const unsigned int p_nb) const
{
    return m_levelHints.at(p_nb - 1);
}

void CSave::saveState(const unsigned int p_nbLevel, const CCell * const p_cells) const
{
    INHIBIT(std::cout << "CSave::saveState" << std::endl;)
    std::ostringstream l_s;
    l_s << p_nbLevel << std::endl;
    l_s << std::hex;
    for (int l_i = 0; l_i < Globals::g_nbCells * Globals::g_nbCells; ++l_i)
        l_s << std::setfill('0') << std::setw(2)
            << (int)(((p_cells[l_i].getLineState(T_DIRECTION_UP) == 2) << 7) | ((p_cells[l_i].getLineState(T_DIRECTION_RIGHT) == 2) << 6) | ((p_cells[l_i].getLineState(T_DIRECTION_DOWN) == 2) << 5) | ((p_cells[l_i].getLineState(T_DIRECTION_LEFT) == 2) << 4) |
                     ((p_cells[l_i].getLineState(T_DIRECTION_UP) == 1) << 3) | ((p_cells[l_i].getLineState(T_DIRECTION_RIGHT) == 1) << 2) | ((p_cells[l_i].getLineState(T_DIRECTION_DOWN) == 1) << 1) | (p_cells[l_i].getLineState(T_DIRECTION_LEFT) == 1));
    // Save state in file
    std::ofstream l_file(SAVE_STATE_FILE);
    if (l_file.is_open())
    {
        l_file << l_s.str();
        l_file.close();
    }
    INHIBIT(else std::cout << "Unable to open file " << SAVE_STATE_FILE << " for writing" << std::endl;)
}

void CSave::getSaveStateInfo(unsigned int &p_level, std::string &p_draw) const
{
    p_level = 0;
    p_draw = "";
    std::ifstream l_file(SAVE_STATE_FILE);
    if (l_file.is_open())
    {
        // Get level number
        std::string l_line("");
        std::getline(l_file, l_line);
        std::istringstream l_stream(l_line);
        l_stream >> p_level;
        // Get drawing
        std::getline(l_file, p_draw);
        // Close
        l_file.close();
    }
    INHIBIT(std::cout << "CSave::getSaveStateInfo " << p_level << " " << p_draw << std::endl;)
}

void CSave::clearSaveState(void) const
{
    // Remove file if necessary
    std::ifstream l_file(SAVE_STATE_FILE);
    if (l_file.is_open())
    {
        // File exists
        l_file.close();
        if (remove(SAVE_STATE_FILE))
            perror("Error deleting file");
    }
}

void CSave::writeConfigFile(void) const
{
    // Open config file for writing
    std::ofstream l_file(CONFIG_FILE);
    if (l_file.is_open())
    {
        INHIBIT(std::cout << "File " << CONFIG_FILE << " opened successfully for writing" << std::endl;)
        std::ostringstream l_stream;
        // Sound option
        l_stream << Globals::g_playSounds;
        l_file << l_stream.str();
        // End
        l_file.close();
    }
}
