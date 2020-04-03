#ifndef _SAVE_H_
#define _SAVE_H_

#include <string>
#include <vector>
#include "cell.h"

class CSave
{
    public:

    // Destructor
    virtual ~CSave(void);

    // Method to get the instance
    static CSave& instance(void);

    // Called when a level is beaten
    void levelBeaten(const unsigned int p_nb);

    // Returns the first unbeaten level
    const unsigned int getContinueLevel(const unsigned int p_start = 1) const;

    // Count beaten levels in each difficulty
    void getBeatenLevels(unsigned int &p_easy, unsigned int &p_normal, unsigned int &p_hard, unsigned int &p_expert) const;

    // Get status of a the given level
    const bool levelIsBeaten(const unsigned int p_nb) const;

    // True = no save file found
    const bool firstTimePlay(void) const;

    // Get the solution for a specific level
    const std::string &getLevelSolution(const unsigned int p_nb) const;

    // Get the hints (cell numbers) for a specific level
    const std::string &getLevelHints(const unsigned int p_nb) const;

    // Save current drawing in a file, to resume next time
    void saveState(const unsigned int p_nbLevel, const CCell * const p_cells) const;

    // Get save state information
    void getSaveStateInfo(unsigned int &p_level, std::string &p_draw) const;

    // Delete save state file
    void clearSaveState(void) const;

    // Write config file
    void writeConfigFile(void) const;

    private:

    // Forbidden
    CSave(void);
    CSave(const CSave &p_source);
    const CSave &operator =(const CSave &p_source);

    // The levels beaten (or not)
    std::string m_save;

    // Level solutions
    std::vector<std::string> m_levelSolutions;

    // Level hints
    std::vector<std::string> m_levelHints;
};

#endif
