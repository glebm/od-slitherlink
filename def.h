#ifndef _DEF_H_
#define _DEF_H_

#include "SDL/SDL.h"

//~ #define INHIBIT(X) X
#define INHIBIT(X) /* X */

#ifndef RES_DIR
#define RES_DIR "res/"
#endif

#ifndef LOCAL_STORAGE_DIR
#define LOCAL_STORAGE_DIR ""
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 320
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 240
#endif

#ifndef SCREEN_BPP
#define SCREEN_BPP 16
#endif

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
#define SAVE_FILE LOCAL_STORAGE_DIR"slitherlink.sav"
#define SAVE_STATE_FILE LOCAL_STORAGE_DIR"slitherlink_state.sav"
#define CONFIG_FILE LOCAL_STORAGE_DIR"slitherlink.cfg"
#define LEVELDATA_FILE RES_DIR"levels.dat"
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

#ifndef MYKEY_UP
#define MYKEY_UP SDLK_UP
#endif
#ifndef MYKEY_RIGHT
#define MYKEY_RIGHT SDLK_RIGHT
#endif
#ifndef MYKEY_DOWN
#define MYKEY_DOWN SDLK_DOWN
#endif
#ifndef MYKEY_LEFT
#define MYKEY_LEFT SDLK_LEFT
#endif
#ifndef MYKEY_LINEUP
#define MYKEY_LINEUP SDLK_r
#endif
#ifndef MYKEY_LINERIGHT
#define MYKEY_LINERIGHT SDLK_g
#endif
#ifndef MYKEY_LINEDOWN
#define MYKEY_LINEDOWN SDLK_f
#endif 
#ifndef MYKEY_LINELEFT
#define MYKEY_LINELEFT SDLK_d
#endif
#ifndef MYKEY_QUIT
#define MYKEY_QUIT SDLK_q
#endif
#ifndef MYKEY_START
#define MYKEY_START SDLK_s
#endif
#ifndef MYKEY_MOD
#define MYKEY_MOD SDLK_l
#endif
#ifndef MYKEY_UNDO
#define MYKEY_UNDO SDLK_u
#endif
#ifndef MYKEY_SYSTEM
#define MYKEY_SYSTEM SDLK_q
#endif
#ifndef MYKEY_PAGEUP
#define MYKEY_PAGEUP SDLK_PAGEUP
#endif
#ifndef MYKEY_PAGEDOWN
#define MYKEY_PAGEDOWN SDLK_PAGEDOWN
#endif
#ifndef MYKEY_OPEN
#define MYKEY_OPEN SDLK_RETURN
#endif
#ifndef MYKEY_PARENT
#define MYKEY_PARENT SDLK_BACKSPACE
#endif
#ifndef MYKEY_OPERATION
#define MYKEY_OPERATION SDLK_a
#endif
#ifndef MYKEY_SELECT
#define MYKEY_SELECT SDLK_INSERT
#endif
#ifndef MYKEY_TRANSFER
#define MYKEY_TRANSFER SDLK_w
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
