#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#define NB_SOUNDS 7
#define NB_SURFACES 19

class CResourceManager
{
    public:

    typedef enum
    {
        T_SOUND_MOVE = 0,
        T_SOUND_DRAW,
        T_SOUND_UNDO,
        T_SOUND_SOLVED,
        T_SOUND_START,
        T_SOUND_BACK,
        T_SOUND_NEXT
    }
    T_SOUND;

    typedef enum
    {
        T_SURFACE_BG_TITLE = 0,
        T_SURFACE_BG_5,
        T_SURFACE_BG_7,
        T_SURFACE_BG_10,
        T_SURFACE_HELP_1,
        T_SURFACE_HELP_2,
        T_SURFACE_HELP_3,
        T_SURFACE_CURSOR,
        T_SURFACE_LINE_VERT,
        T_SURFACE_LINE_HORI,
        T_SURFACE_CROSS,
        T_SURFACE_ARROW_UP_SMALL,
        T_SURFACE_ARROW_DOWN_SMALL,
        T_SURFACE_MENU_PAUSE,
        T_SURFACE_MENU_CURSOR,
        T_SURFACE_NOTE,
        T_SURFACE_NOTE_SELECTED,
        T_SURFACE_BORDER_H,
        T_SURFACE_BORDER_V
    }
    T_SURFACE;

    // Method to get the instance
    static CResourceManager& instance(void);

    // Cleanup all resources
    void sdlCleanup(void);

    // Get a loaded sound effect
    Mix_Chunk *getSound(const T_SOUND p_sound) const;

    // Get a loaded surface
    SDL_Surface *getSurface(const T_SURFACE p_surface) const;

    // Get a loaded font
    TTF_Font *getFont(void) const;

    private:

    // Forbidden
    CResourceManager(void);
    CResourceManager(const CResourceManager &p_source);
    const CResourceManager &operator =(const CResourceManager &p_source);

    // Print static text on images
    void initImages(void);

    // Sounds effects
    Mix_Chunk *m_sounds[NB_SOUNDS];

    // Images
    SDL_Surface *m_surfaces[NB_SURFACES];

    // Font
    TTF_Font *m_font;
};

#endif
