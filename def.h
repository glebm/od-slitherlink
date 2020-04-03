#ifndef _DEF_H_
#define _DEF_H_

#include "SDL/SDL.h"

//~ #define INHIBIT(X) X
#define INHIBIT(X) /* X */

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_BPP 16
#define MS_PER_FRAME 33
#define NB_LEVELS 456
#define NB_LEVELS_EASY 30
#define NB_LEVELS_NORMAL 100
#define NB_LEVELS_HARD 316
#define NB_LEVELS_EXPERT 10
#define COLOR_BLACK 70,27,10
#define COLOR_RED 154,0,0
#define COLOR_BLUE 120,153,162
#define COLOR_GREEN 90,150,90
#define SAVE_FILE "slitherlink.sav"
#define SAVE_STATE_FILE "slitherlink_state.sav"
#define CONFIG_FILE "slitherlink.cfg"
#define LEVELDATA_FILE "res/levels.dat"
#define KEYHOLD_TIMER_FIRST 6
#define KEYHOLD_TIMER 2
#define TEXT_GAP 16
#define CELL_WIDTH 21
#define CELLS_ONSCREEN 9
#define MAX_UNDO_ACTIONS 128
#define NB_ITEMS_PAUSE_MENU 5
#define CURSOR_ALPHA_MAX 96
#define CURSOR_ALPHA_MIN 40
#define CURSOR_ALPHA_STEP 4

#ifdef PLATFORM_DINGOO
// Keys for Dingoo
#define MYKEY_UP        SDLK_UP
#define MYKEY_RIGHT     SDLK_RIGHT
#define MYKEY_DOWN      SDLK_DOWN
#define MYKEY_LEFT      SDLK_LEFT
#define MYKEY_LINEUP    SDLK_SPACE
#define MYKEY_LINERIGHT SDLK_LCTRL
#define MYKEY_LINEDOWN  SDLK_LALT
#define MYKEY_LINELEFT  SDLK_LSHIFT
#define MYKEY_QUIT      SDLK_ESCAPE
#define MYKEY_START     SDLK_RETURN
#define MYKEY_MOD       SDLK_TAB
#define MYKEY_UNDO      SDLK_BACKSPACE
#else
// Keys for PC keyboard
#define MYKEY_UP        SDLK_UP
#define MYKEY_RIGHT     SDLK_RIGHT
#define MYKEY_DOWN      SDLK_DOWN
#define MYKEY_LEFT      SDLK_LEFT
#define MYKEY_LINEUP    SDLK_r
#define MYKEY_LINERIGHT SDLK_g
#define MYKEY_LINEDOWN  SDLK_f
#define MYKEY_LINELEFT  SDLK_d
#define MYKEY_QUIT      SDLK_q
#define MYKEY_START     SDLK_s
#define MYKEY_MOD       SDLK_l
#define MYKEY_UNDO      SDLK_u
#endif


// Directions
typedef enum
{
    T_DIRECTION_UP = 0,
    T_DIRECTION_DOWN,
    T_DIRECTION_RIGHT,
    T_DIRECTION_LEFT
}
T_DIRECTION;

// Globals
namespace Globals
{
    // Number of cells
    extern unsigned char g_nbCells;
    // Colors
    extern const SDL_Color g_textColorBlack;
    extern const SDL_Color g_textColorRed;
    extern const SDL_Color g_textColorGreen;
    extern const SDL_Color g_textColorBlue;
    // Play sounds or not
    extern bool g_playSounds;
    // Offsets
    extern unsigned int g_offsetX;
    extern unsigned int g_offsetY;
    // "Camera", for grids larger than 10x10.
    extern unsigned char g_cameraX;
    extern unsigned char g_cameraY;
}

#endif
