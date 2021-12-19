#ifndef GOPHER_MEDIA_BASE_GAME
#define GOPHER_MEDIA_BASE_GAME
#include "gopher_media.h"

class BaseGame {
public:
    void Update();
    void Shutdown();
    void ShowOutput();
    bool GetSuccess();
    void StopLoop();
    bool GetIsRunning();
    bool GetKeyboardState(int keycode);
    Vector2I GetMousePosition();
    bool GetMouseButton(MouseButton button);
private:
    void ProcessInput();
    SDL_Window* window;
    Uint32 ticks_count;
    float frame_rate;
    bool initialized;
    bool is_running;
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    Vector2I mouse_position;
    Uint32 mouse_buttons;
friend class Game2D;
friend class Game3D;
};

#endif