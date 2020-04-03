#include <iostream>
#include "resourceManager.h"
#include "def.h"
#include "sdlutils.h"

CResourceManager& CResourceManager::instance(void)
{
    static CResourceManager l_singleton;
    return l_singleton;
}

CResourceManager::CResourceManager(void) :
    m_font(NULL)
{
    // Load sounds
    m_sounds[T_SOUND_MOVE] = SDL_utils::loadSound("res/soundMove.dat");
    m_sounds[T_SOUND_DRAW] = SDL_utils::loadSound("res/soundDraw.dat");
    m_sounds[T_SOUND_UNDO] = SDL_utils::loadSound("res/soundUndo.dat");
    m_sounds[T_SOUND_SOLVED] = SDL_utils::loadSound("res/soundSolved.dat");
    m_sounds[T_SOUND_START] = SDL_utils::loadSound("res/soundStart.dat");
    m_sounds[T_SOUND_BACK] = SDL_utils::loadSound("res/soundBack.dat");
    m_sounds[T_SOUND_NEXT] = SDL_utils::loadSound("res/soundNext.dat");
    // Load images
    m_surfaces[T_SURFACE_BG_TITLE] = SDL_utils::loadImage("res/title.png");
    m_surfaces[T_SURFACE_BG_5] = SDL_utils::loadImage("res/grid5.png");
    m_surfaces[T_SURFACE_BG_7] = SDL_utils::loadImage("res/grid7.png");
    m_surfaces[T_SURFACE_BG_10] = SDL_utils::loadImage("res/grid10.png");
    m_surfaces[T_SURFACE_HELP_1] = SDL_utils::loadImage("res/help1.png");
    m_surfaces[T_SURFACE_HELP_2] = SDL_utils::loadImage("res/help2.png");
    m_surfaces[T_SURFACE_HELP_3] = SDL_utils::loadImage("res/help3.png");
    m_surfaces[T_SURFACE_CURSOR] = SDL_utils::loadImage("res/cursor.png");
    SDL_SetAlpha(m_surfaces[T_SURFACE_CURSOR], SDL_SRCALPHA, CURSOR_ALPHA_MAX);
    m_surfaces[T_SURFACE_LINE_VERT] = SDL_utils::loadImage("res/lineVertical.png");
    m_surfaces[T_SURFACE_LINE_HORI] = SDL_utils::loadImage("res/lineHorizontal.png");
    m_surfaces[T_SURFACE_CROSS] = SDL_utils::loadImage("res/cross.png");
    m_surfaces[T_SURFACE_ARROW_UP_SMALL] = SDL_utils::loadImage("res/arrowUpSmall.png");
    m_surfaces[T_SURFACE_ARROW_DOWN_SMALL] = SDL_utils::loadImage("res/arrowDownSmall.png");
    m_surfaces[T_SURFACE_MENU_PAUSE] = SDL_utils::loadImage("res/menuPause.png");
    m_surfaces[T_SURFACE_MENU_CURSOR] = SDL_utils::loadImage("res/menuCursor.png");
    m_surfaces[T_SURFACE_NOTE] = SDL_utils::loadImage("res/note.png");
    m_surfaces[T_SURFACE_NOTE_SELECTED] = SDL_utils::loadImage("res/noteSelected.png");
    m_surfaces[T_SURFACE_BORDER_H] = SDL_utils::loadImage("res/borderH.png");
    m_surfaces[T_SURFACE_BORDER_V] = SDL_utils::loadImage("res/borderV.png");
    SDL_SetAlpha(m_surfaces[T_SURFACE_BORDER_H], SDL_SRCALPHA, 153);
    SDL_SetAlpha(m_surfaces[T_SURFACE_BORDER_V], SDL_SRCALPHA, 153);
    // Load font
    m_font = SDL_utils::loadFont("res/hardpixel.ttf", 10);
    // Print static text
    this->initImages();
}

void CResourceManager::initImages(void)
{
    // Init title image
    SDL_utils::applyText(100, 95, m_surfaces[T_SURFACE_BG_TITLE], m_font, "Tiny", Globals::g_textColorBlack);
    SDL_utils::applyText(100, 115, m_surfaces[T_SURFACE_BG_TITLE], m_font, "Small", Globals::g_textColorBlack);
    SDL_utils::applyText(100, 135, m_surfaces[T_SURFACE_BG_TITLE], m_font, "Standard", Globals::g_textColorBlack);
    SDL_utils::applyText(100, 155, m_surfaces[T_SURFACE_BG_TITLE], m_font, "Large", Globals::g_textColorBlack);
    // Init help 1 image
    Sint16 l_y(32);
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "Slitherlink is a logic puzzle game created by Japanese", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "publisher Nikoli, famous for their Sudoku.", Globals::g_textColorBlack);
    l_y = 92;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "Draw lines on the grid to create a meandering path that", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "forms a single loop, without crossing itself or branching.", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "The numbers indicate how many lines surround each cell.", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "Empty cells may be surrounded by any number of lines.", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "Draw small x's on segments that cannot be connected.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "There is one unique solution, to be found by logical", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "deduction, without guessing.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(8, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "More information:", Globals::g_textColorBlack);
    SDL_utils::applyText(105, l_y, m_surfaces[T_SURFACE_HELP_1], m_font, "http://en.wikipedia.org/wiki/Slitherlink", Globals::g_textColorRed);
    // Init help 2 image
    l_y = 40;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "D-pad", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Move the cursor.", Globals::g_textColorBlack);
    l_y += 1.6 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "A/B/X/Y", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Draw a line in the corresponding direction,", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "then an 'x', then nothing.", Globals::g_textColorBlack);
    l_y += 1.6 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "L + A/B/X/Y", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Draw an 'x' in the corresponding direction,", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "then a line, then nothing.", Globals::g_textColorBlack);
    l_y += 1.6 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "R", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Undo last action.", Globals::g_textColorBlack);
    l_y += 1.6 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "START", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Open menu.", Globals::g_textColorBlack);
    l_y += 1.6 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "SELECT", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_2], m_font, "Quit.", Globals::g_textColorBlack);
    // Init help 3 image
    l_y = 32;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Version:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Slitherlink v1.0 for Dingoo A320.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Homepage:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "http://beyondds.free.fr", Globals::g_textColorRed);
    l_y = 98;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Dev:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Mia.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Concept:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Nikoli.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Puzzles:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Krazy Dad.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Fonts:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Karmatic Arcade by Vic Fieger,", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Hardpixel by Jovanny Lemonad.", Globals::g_textColorBlack);
    l_y += 1.4 * TEXT_GAP;
    SDL_utils::applyText(68, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Sounds:", Globals::g_textColorBlack, SDL_utils::T_TEXT_ALIGN_RIGHT);
    SDL_utils::applyText(80, l_y, m_surfaces[T_SURFACE_HELP_3], m_font, "Junggle.", Globals::g_textColorBlack);
    // Init pause menu
    l_y = 7;
    SDL_utils::applyText(24, l_y, m_surfaces[T_SURFACE_MENU_PAUSE], m_font, "Retry", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(24, l_y, m_surfaces[T_SURFACE_MENU_PAUSE], m_font, "Save state", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(24, l_y, m_surfaces[T_SURFACE_MENU_PAUSE], m_font, "Sound", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(24, l_y, m_surfaces[T_SURFACE_MENU_PAUSE], m_font, "Help", Globals::g_textColorBlack);
    l_y += TEXT_GAP;
    SDL_utils::applyText(24, l_y, m_surfaces[T_SURFACE_MENU_PAUSE], m_font, "Quit game", Globals::g_textColorBlack);
}

void CResourceManager::sdlCleanup(void)
{
    INHIBIT(std::cout << "CResourceManager::sdlCleanup" << std::endl;)
    int l_i(0);
    // Free sounds
    for (l_i = 0; l_i < NB_SOUNDS; ++l_i)
    {
        if (m_sounds[l_i] != NULL)
        {
            Mix_FreeChunk(m_sounds[l_i]);
            m_sounds[l_i] = NULL;
        }
    }
    // Free surfaces
    for (l_i = 0; l_i < NB_SURFACES; ++l_i)
    {
        if (m_surfaces[l_i] != NULL)
        {
            SDL_FreeSurface(m_surfaces[l_i]);
            m_surfaces[l_i] = NULL;
        }
    }
    // Free font
    if (m_font != NULL)
    {
        TTF_CloseFont(m_font);
        m_font = NULL;
    }
}

Mix_Chunk *CResourceManager::getSound(const T_SOUND p_sound) const
{
    return m_sounds[p_sound];
}

SDL_Surface *CResourceManager::getSurface(const T_SURFACE p_surface) const
{
    return m_surfaces[p_surface];
}

TTF_Font *CResourceManager::getFont(void) const
{
    return m_font;
}
