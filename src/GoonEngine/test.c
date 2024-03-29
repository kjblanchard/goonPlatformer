#include <stdio.h>
#include <GoonEngine/test.h>
#include <GoonEngine/gnpch.h>
#include <GoonEngine/SdlWindow.h>
#include <SupergoonSound/include/sound.h>
// #include <GoonEngine/ecs/system.h>
// #include <GoonEngine/ecs/context.h>
#include <GoonEngine/keyboard.h>
#include <GoonEngine/joystick.h>
#include <GoonEngine/debug.h>

// Physics
#include <GoonPhysics/scene.h>

static gpScene *g_pScene;

extern SDL_Texture *g_BackgroundAtlas;
extern SDL_Rect g_backgroundDrawRect;

#define MAX_STARTUP_FRAMES 1000

static SDL_Event event;
static bool shouldQuit = false;

static uint64_t lastFrameMilliseconds;
static float msBuildup;

// TODO this should be different, it is inside of SDLwindow.c
extern SDL_Renderer *g_pRenderer;
extern int g_refreshRate;
// extern geContext *g_Context;

void (*DrawUpdateFunc)() = NULL;
void (*GameUpdateFunc)(double deltaTime) = NULL;



void *MusicUpdateWrapper(void *arg)
{
    return NULL;
}

/**
 * @brief Handles all SDL events every frame.
 *
 * @return true If we should quit or not
 * @return false If we should quit or not
 */
static bool sdlEventLoop()
{
    // Event loop, Handle SDL events.
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            HandleKeyboardEvent(&event);
            break;
        case SDL_CONTROLLERBUTTONUP:
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERDEVICEADDED:
            HandleJoystickEvent(&event);
            break;
        default:
            break;
        }
    }
    return false;
}

static int loop_func()
{
    Uint64 beginFrame = SDL_GetTicks64();
    Uint64 delta = beginFrame - lastFrameMilliseconds;
    msBuildup += delta;
    lastFrameMilliseconds = beginFrame;
    // Handle SDL inputs
    shouldQuit = sdlEventLoop();
    if (shouldQuit)
        return 0;
    // TODO make these static and pass into as ref to stop allocations
    // Initialize time this frame
    double deltaTimeSeconds = 1 / (double)g_refreshRate;
    double deltaTimeMs = 1000 / (double)g_refreshRate;
    if (msBuildup < deltaTimeMs)
        return true;
    geUpdateKeyboard();

    // Run Update and update physics as many times as needed
    while (msBuildup >= deltaTimeMs)
    {
        UpdateSound();
        if (g_pScene)
        {
            gpSceneUpdate(g_pScene, deltaTimeSeconds);
        }
        if (GameUpdateFunc)
        {
            GameUpdateFunc(deltaTimeMs);
        }
        msBuildup -= deltaTimeMs;
    }
    geUpdateControllers();

    SDL_SetRenderDrawColor(g_pRenderer, 100, 100, 100, 255);
    SDL_RenderClear(g_pRenderer);
    if (g_BackgroundAtlas)
    {
        int drawResult = SDL_RenderCopy(g_pRenderer, g_BackgroundAtlas, &g_backgroundDrawRect, &g_backgroundDrawRect);

        if (drawResult != 0)
        {
            LogError("Did not draw properly, Error %s\n", SDL_GetError());
        }
    }
    if (DrawUpdateFunc)
    {
        DrawUpdateFunc();
    }

    SDL_RenderPresent(g_pRenderer);
}

int Play()
{
    while (!shouldQuit)
    {
        loop_func();
    }
}
int GnInitializeEngine()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        fprintf(stderr, "Could not Initialize SDL!\nError: %s", SDL_GetError());
        return 1;
    }
    if (!IMG_Init(IMG_INIT_PNG))
    {
        fprintf(stderr, "Could not Initialize SDL Image!\nError: %s", IMG_GetError());
        return 1;
    }

    geInitializeKeyboard();
    geInitializeJoysticks();
    InitializeDebugLogFile();
}

void geSetCurrentScene(void *scene)
{
    g_pScene = scene;
}

void geGameSetDrawFunc(void (*drawFunc)())
{
    DrawUpdateFunc = drawFunc;
}
void geGameSetUpdateFunc(void (*updateFunc)(double deltaTime))
{
    GameUpdateFunc = updateFunc;
}
