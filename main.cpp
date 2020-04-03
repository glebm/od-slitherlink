#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "def.h"
#include "save.h"
#include "title.h"
#include "help.h"
#include "resourceManager.h"
#include "level.h"
#include "draw.h"
#include "menuPause.h"

// Global variables
unsigned char Globals::g_nbCells(5);
const SDL_Color Globals::g_textColorBlack = {COLOR_BLACK};
const SDL_Color Globals::g_textColorRed = {COLOR_RED};
const SDL_Color Globals::g_textColorBlue = {COLOR_BLUE};
const SDL_Color Globals::g_textColorGreen = {COLOR_GREEN};
bool Globals::g_playSounds(true);
unsigned int Globals::g_offsetX(0);
unsigned int Globals::g_offsetY(0);
unsigned char Globals::g_cameraX(0);
unsigned char Globals::g_cameraY(0);

typedef enum
{
    T_GAME_STATE_TITLE = 0,
    T_GAME_STATE_PLAY,
    T_GAME_STATE_SOLVED,
    T_GAME_STATE_HELP,
    T_GAME_STATE_PAUSE
}
T_GAME_STATE;

int main(int argc, char** argv)
{
    // Avoid crash due to the absence of mouse
    char l_s[]="SDL_NOMOUSE=1";
    putenv(l_s);
    
    // Init SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // Hide cursor before creating the output surface.
    SDL_ShowCursor(SDL_DISABLE);

    // Screen
    SDL_Surface *l_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (l_screen == NULL)
    {
        std::cout << "SDL_SetVideoMode failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Init font
    if (TTF_Init() == -1)
    {
        std::cout << "TTF_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == -1)
    {
        std::cout << "Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Create instances
    CResourceManager::instance();
    CSave::instance();

    // Game state
    T_GAME_STATE l_gameState(T_GAME_STATE_TITLE);
    // Title object
    CTitle l_title(l_screen);
    // Level object
    CLevel l_level(l_screen);
    // Draw object
    CDraw l_draw(l_screen);
    // Help object
    CHelp l_help(l_screen);
    // Pause menu object
    CMenuPause l_menuPause(l_screen);

    // Used for framerate limiter
    Uint32 l_time(0);

    // Render first frame to avoid graphical glitch on Dingoo
    l_time = SDL_GetTicks();
    l_title.render();
    SDL_Flip(l_screen);
    l_time = MS_PER_FRAME - (SDL_GetTicks() - l_time);
    if (l_time <= MS_PER_FRAME) SDL_Delay(l_time);

    // Main loop
    SDL_Event l_event;
    bool l_loop(true);
    bool l_render(true);
    while (l_loop)
    {
        l_time = SDL_GetTicks();
        switch (l_gameState)
        {
//==============================================================================
            case T_GAME_STATE_TITLE:
                // Handle events
                while (SDL_PollEvent(&l_event))
                {
                    if (l_event.type == SDL_KEYDOWN)
                    {
                        switch (l_event.key.keysym.sym)
                        {
                            case MYKEY_LINEDOWN:
                            case MYKEY_LINERIGHT:
                            case MYKEY_START:
                                // Start the game
                                if (l_title.getSaveStateLevel())
                                {
                                    // Load save state
                                    l_level.reset();
                                    l_draw.reset();
                                    l_level.loadLevel(l_title.getSaveStateLevel());
                                    l_draw.loadLevel(l_title.getSaveStateDraw());
                                    CSave::instance().clearSaveState();
                                    l_gameState = T_GAME_STATE_PLAY;
                                }
                                else
                                {
                                    // Load requested level
                                    l_level.reset();
                                    l_draw.reset();
                                    l_level.loadLevel(l_title.getContinueLevel());
                                    l_draw.loadLevel();
                                    if (CSave::instance().firstTimePlay())
                                    {
                                        l_help.setPage(0);
                                        l_gameState = T_GAME_STATE_HELP;
                                    }
                                    else
                                        l_gameState = T_GAME_STATE_PLAY;
                                }
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_START), 0);
                                l_render = true;
                                break;
                            case MYKEY_QUIT:
                                l_loop = false;
                                break;
                            default:
                                if (!l_title.getSaveStateLevel())
                                    l_render = l_title.keyPressed(l_event);
                                break;
                        }
                    }
                    // If the user has Xed out the window
                    else if (l_event.type == SDL_QUIT)
                    {
                        //Quit the program
                        l_loop = false;
                    }
                }
                // Handle key hold
                if (!l_title.getSaveStateLevel())
                {
                    l_render = l_title.keyHold(T_DIRECTION_UP, SDL_GetKeyState(NULL)[MYKEY_UP]) || l_render;
                    l_render = l_title.keyHold(T_DIRECTION_DOWN, SDL_GetKeyState(NULL)[MYKEY_DOWN]) || l_render;
                }
                // Render if necessary
                if (l_render && l_gameState == T_GAME_STATE_TITLE)
                {
                    l_title.render();
                    SDL_Flip(l_screen);
                    l_render = false;
                    INHIBIT(std::cout << "Render time: " << SDL_GetTicks() - l_time << std::endl;)
                }
                break;
//==============================================================================
            case T_GAME_STATE_HELP:
                // Handle events
                while (SDL_PollEvent(&l_event))
                {
                    if (l_event.type == SDL_KEYDOWN)
                    {
                        switch (l_event.key.keysym.sym)
                        {
                            case MYKEY_LINEUP:
                            case MYKEY_LINELEFT:
                            case MYKEY_QUIT:
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_UNDO), 0);
                                l_gameState = T_GAME_STATE_PLAY;
                                l_render = true;
                                break;
                            default:
                            {
                                bool l_quitHelp(false);
                                l_render = l_help.keyPressed(l_event, l_quitHelp);
                                if (l_quitHelp)
                                {
                                    if (Globals::g_playSounds)
                                        Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_UNDO), 0);
                                    l_gameState = T_GAME_STATE_PLAY;
                                }
                            }
                            break;
                        }
                    }
                    else if (l_event.type == SDL_QUIT)
                    {
                        //Quit the program
                        l_loop = false;
                    }
                }
                // Render if necessary
                if (l_render && l_gameState == T_GAME_STATE_HELP)
                {
                    l_help.render();
                    SDL_Flip(l_screen);
                    l_render = false;
                    INHIBIT(std::cout << "Render time: " << SDL_GetTicks() - l_time << std::endl;)
                }
                break;
//==============================================================================
            case T_GAME_STATE_PLAY:
                // Handle events
                while (SDL_PollEvent(&l_event))
                {
                    if (l_event.type == SDL_KEYDOWN)
                    {
                        switch (l_event.key.keysym.sym)
                        {
                            case MYKEY_QUIT:
                                l_level.reset();
                                l_draw.reset();
                                l_title.updateLevelInformation();
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_BACK), 0);
                                l_gameState = T_GAME_STATE_TITLE;
                                l_render = true;
                                break;
                            case MYKEY_START:
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_SOLVED), 0);
                                l_gameState = T_GAME_STATE_PAUSE;
                                l_render = true;
                                break;
                            default:
                                l_render = l_draw.keyPressed(l_event);
                                break;
                        }
                    }
                    // If the user has Xed out the window
                    else if (l_event.type == SDL_QUIT)
                    {
                        //Quit the program
                        l_loop = false;
                    }
                }
                // Handle key hold
                l_render = l_draw.keyHold() || l_render;
                // Update cursor animation
                l_draw.getCursor().update();
                // Check whether the puzzle is solved
                if (l_render && l_gameState == T_GAME_STATE_PLAY && l_level.solutionFound(l_draw))
                {
                    CSave::instance().levelBeaten(l_level.getLevelNumber());
                    l_draw.clearXs();
                    if (Globals::g_playSounds)
                        Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_SOLVED), 0);
                    l_gameState = T_GAME_STATE_SOLVED;
                }
                // Render every time because of the cursor animation
                if (l_gameState == T_GAME_STATE_PLAY)
                {
                    l_level.render(l_draw);
                    l_draw.render(true);
                    SDL_Flip(l_screen);
                    l_render = false;
                    //~ INHIBIT(std::cout << "Render time: " << SDL_GetTicks() - l_time << std::endl;)
                }
                break;
//==============================================================================
            case T_GAME_STATE_SOLVED:
                // Handle events
                while (SDL_PollEvent(&l_event))
                {
                    if (l_event.type == SDL_KEYDOWN)
                    {
                        switch (l_event.key.keysym.sym)
                        {
                            case MYKEY_LINEDOWN:
                            case MYKEY_LINERIGHT:
                            case MYKEY_START:
                            {
                                unsigned int l_nb(l_level.getLevelNumber());
                                if (l_nb < NB_LEVELS)
                                {
                                    // Go to next level
                                    l_level.reset();
                                    l_draw.reset();
                                    l_level.loadLevel(l_nb + 1);
                                    l_draw.loadLevel();
                                    if (Globals::g_playSounds)
                                        Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_NEXT), 0);
                                    l_gameState = T_GAME_STATE_PLAY;
                                    l_render = true;
                                }
                                else
                                {
                                    // Back to title
                                    l_level.reset();
                                    l_draw.reset();
                                    l_title.updateLevelInformation();
                                    if (Globals::g_playSounds)
                                        Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_BACK), 0);
                                    l_gameState = T_GAME_STATE_TITLE;
                                    l_render = true;
                                }
                            }
                            break;
                            case MYKEY_LINEUP:
                            case MYKEY_LINELEFT:
                            case MYKEY_QUIT:
                                l_level.reset();
                                l_draw.reset();
                                l_title.updateLevelInformation();
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_UNDO), 0);
                                l_gameState = T_GAME_STATE_TITLE;
                                l_render = true;
                                break;
                            default:
                                break;
                        }
                    }
                    else if (l_event.type == SDL_QUIT)
                    {
                        //Quit the program
                        l_loop = false;
                    }
                }
                // Render if necessary
                if (l_render && l_gameState == T_GAME_STATE_SOLVED)
                {
                    l_level.render(l_draw);
                    l_draw.render(false);
                    SDL_Flip(l_screen);
                    l_render = false;
                }
                break;
//==============================================================================
            case T_GAME_STATE_PAUSE:
                // Handle events
                while (SDL_PollEvent(&l_event))
                {
                    if (l_event.type == SDL_KEYDOWN)
                    {
                        switch (l_event.key.keysym.sym)
                        {
                            case MYKEY_LINEDOWN:
                            case MYKEY_LINERIGHT:
                                switch(l_menuPause.getSelected())
                                {
                                    case 0:
                                    {
                                        // Retry
                                        unsigned int l_nb(l_level.getLevelNumber());
                                        l_level.reset();
                                        l_draw.reset();
                                        l_level.loadLevel(l_nb);
                                        l_draw.loadLevel();
                                        if (Globals::g_playSounds)
                                            Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_NEXT), 0);
                                        l_gameState = T_GAME_STATE_PLAY;
                                        l_render = true;
                                    }
                                    break;
                                    case 1:
                                        // Save state
                                        CSave::instance().saveState(l_level.getLevelNumber(), l_draw.getCell());
                                        l_level.reset();
                                        l_draw.reset();
                                        l_title.updateLevelInformation();
                                        if (Globals::g_playSounds)
                                            Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_NEXT), 0);
                                        l_gameState = T_GAME_STATE_TITLE;
                                        l_render = true;
                                        break;
                                    case 2:
                                        // Sounds
                                        Globals::g_playSounds = !Globals::g_playSounds;
                                        Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_DRAW), 0);
                                        // Save new value in config file
                                        CSave::instance().writeConfigFile();
                                        l_render = true;
                                        break;
                                    case 3:
                                        // Help
                                        if (Globals::g_playSounds)
                                            Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_MOVE), 0);
                                        l_gameState = T_GAME_STATE_HELP;
                                        l_render = true;
                                        break;
                                    case 4:
                                        // Quit game
                                        l_loop = false;
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case MYKEY_QUIT:
                            case MYKEY_START:
                                if (Globals::g_playSounds)
                                    Mix_PlayChannel(-1, CResourceManager::instance().getSound(CResourceManager::T_SOUND_UNDO), 0);
                                l_gameState = T_GAME_STATE_PLAY;
                                l_render = true;
                                break;
                            default:
                                l_render = l_menuPause.keyPressed(l_event);
                                break;
                        }
                    }
                    else if (l_event.type == SDL_QUIT)
                    {
                        //Quit the program
                        l_loop = false;
                    }
                }
                // Handle key hold
                l_render = l_menuPause.keyHold(T_DIRECTION_UP, SDL_GetKeyState(NULL)[MYKEY_UP]) || l_render;
                l_render = l_menuPause.keyHold(T_DIRECTION_DOWN, SDL_GetKeyState(NULL)[MYKEY_DOWN]) || l_render;
                // Render if necessary
                if (l_render && l_gameState == T_GAME_STATE_PAUSE)
                {
                    l_level.render(l_draw);
                    l_draw.render(false);
                    l_menuPause.render();
                    SDL_Flip(l_screen);
                    l_render = false;
                }
                break;
//==============================================================================
            default:
                break;
        }
        // Cap the framerate
        l_time = MS_PER_FRAME - (SDL_GetTicks() - l_time);
        if (l_time <= MS_PER_FRAME) SDL_Delay(l_time);
    }

    // Cleanup
    l_menuPause.sdlCleanup();
    l_title.sdlCleanup();
    l_level.sdlCleanup();
    CResourceManager::instance().sdlCleanup();
    TTF_Quit();
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();

    return 0;
}
