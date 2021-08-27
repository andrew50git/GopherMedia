#include "../gopher_media.h"

/*
Base Game
*/

void BaseGame::ProcessInput() {
    SDL_PumpEvents();
    keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running = false;
            break;
        }
    }
    mouse_buttons = SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
}

bool BaseGame::GetKeyboardState(int keycode) {
    return keys[SDL_GetScancodeFromKey(keycode)];
}

std::array<int, 2> BaseGame::GetMousePos() {
    int window_x, window_y;
    SDL_GetWindowPosition(window, &window_x, &window_y);
    return {mouse_x - window_x, mouse_y - window_y};
}

bool BaseGame::GetMouseButton(MouseButton button) {
    if (button == left) {
        return mouse_buttons & SDL_BUTTON_LMASK;
    } else if (button == middle) {
        return mouse_buttons & SDL_BUTTON_MMASK;
    } else {
        return mouse_buttons & SDL_BUTTON_RMASK;
    }
}

bool BaseGame::GetSuccess() {
    return initialized;
}

void BaseGame::StopLoop() {
    is_running = false;
}

bool BaseGame::GetIsRunning() {
    return is_running;
}

void Quit() {
    SDL_Quit();
}
