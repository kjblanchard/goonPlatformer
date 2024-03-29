#include <GoonEngine/gnpch.h>
#include <GoonEngine/Sound.h>
#include <SupergoonSound/include/sound.h>

static SDL_Event event;

Bgm *BgmLoad(const char *filename, float begin, float end)
{
    // These should be removed, and used to simplify
    // InitializeSound();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            // HandleKeyboardEvent(&event, L);
            break;
        default:
            break;
        }
    }
    // Arg1: String, filename
    // Arg2: float begin loop marker
    // Arg3: float end loop marker
    Bgm *bgm = LoadBgm(filename, begin, end);
    if (!bgm)
    {
        fprintf(stderr, "Could not load BGM %s", filename);
        return 1;
    }
    int result = PreLoadBgm(bgm);
    // Returns BGM pointer, or nil, which should be free'd afterwards.
    return bgm;
}

static Sfx *SfxLoad(const char *filename)
{
    Sfx *sfx = LoadSfxHelper(filename);
    if (!sfx)
    {
        // LogError("Could not load Sfx %s", filename);
        return NULL;
    }
    int result = LoadSfx(sfx);
    return sfx;
}
static int SfxPlay(Sfx *sfx, float volume)
{
    PlaySfxOneShot(sfx, volume);
    return 0;
}

int BgmPlay(Bgm *bgm, float volume)
{
    // Arg1: Bgm* bgm to play
    // Arg2  number - volume
    int result = PreLoadBgm(bgm);
    PlayBgm(bgm, volume);
    return 0;
}

// static int DestroyBgm(lua_State *L)
// {
//     // Arg1: Bgm*
//     if (!lua_islightuserdata(L, 1))
//     {
//         LogError("Bad argument passed into PlayBGm, expected a userdata ptr");
//         return 0;
//     }
//     Bgm *bgm = (Bgm *)lua_touserdata(L, 1);
//     if (!bgm)
//     {
//         LogError("Pointer passed to playbgm is not able to be casted to a bgm");
//         return 0;
//     }
//     free(bgm);
//     return 0;
// }