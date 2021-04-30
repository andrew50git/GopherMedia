#ifndef GOPHER_MEDIA_H
#define GOPHER_MEDIA_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <array>
#include <iostream>
#include <map>
#include <vector>

enum image_flip {
    vertical,
    horizontal,
    none
};


class Game {
public:
    class Image {
    public:
        Image() {};
        int GetWidth();
        int GetHeight();
        void Render(int x, int y, int width, int height);
        void Render(int x, int y, int width, int height, const double angle, std::array<int, 2> rotation_point, image_flip flip);
    private:
        int width;
        int height;
        SDL_Texture* SDL_image;
        SDL_Renderer* renderer;
    friend class Game;
    };
    class Font {
    public:
        Font() {};
        void Render(std::string text, int x, int y, std::array<Uint8, 4> color);
        void Render(std::string text, int x, int y, const double angle, std::array<int, 2> rotation_point, image_flip flip, std::array<Uint8, 4> color);
    private:
        TTF_Font* SDL_font;
        SDL_Renderer* renderer;
    friend class Game;
    };
    class Animation {
    public:
        Animation() {};
        void Render(int x, int y, int width, int height);
        void SwitchState(std::string new_state);
        void AddState(std::string name, std::vector<Game::Image> images_src);
    private:
        std::map<std::string, std::vector<Game::Image>> images;
    friend class Game;
    };
    Game(const char* title, int x, int y, int width, int height, float frame_rate);
    void Update();
    void Shutdown();
    void Rectangle(int startx, int starty, int endx, int endy, std::array<Uint8, 4> color);
    void ShowOutput();
    bool GetInitSuccess();
    void StopLoop();
    bool GetIsRunning();
    Uint8* GetKeyboardState();
    Image CreateImage(const char* file_path);
    Font CreateFont(const char* file_path, int size);
private:
    void ProcessInput();
    void UpdateGame();
    SDL_Window* window;
    Uint32 ticks_count;
    float frame_rate_attr;
    Uint8* keyboard_state;
    SDL_Renderer* renderer;
    bool init_success;
    bool is_running;
};

#endif
