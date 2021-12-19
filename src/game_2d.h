#ifndef GOPHER_MEDIA_GAME_2D
#define GOPHER_MEDIA_GAME_2D

#include "gopher_media.h"

class Game2D : public BaseGame {
public:
    class Image {
    public:
        Image() {};
        int GetWidth();
        int GetHeight();
        void Render(RectangleI rectangle);
        void Render(RectangleI rectangle, const double angle,
                    Point2I rotation_point, ImageFlip flip);
        void Delete();
    private:
        int width;
        int height;
        SDL_Texture* SDL_image;
        SDL_Renderer* renderer;
        bool initialized = false;
    friend class Game2D;
    };
    class Font {
    public:
        Font() {};
        void Render(std::string text, Point2I text_pos, ColorUC color);
        void Render(std::string text, Point2I text_pos, const double angle, Point2I rotation_point,
                    ImageFlip flip, ColorUC color);
        void Delete();
    private:
        TTF_Font* SDL_font;
        SDL_Renderer* renderer;
        bool initialized = false;
    friend class Game2D;
    };
    Game2D(std::string title, RectangleI rectangle, float frame_rate_arg);
    void Update();
    void Delete();
    void ShowOutput();
    void StopLoop();
    Image CreateImage(std::string file_path);
    Font CreateFont(std::string file_path, int size);
    void RenderRectangleI(RectangleI rectangle, ColorUC color);
private:
    void WaitUntilFrame();
    SDL_Renderer* renderer;
};

#endif