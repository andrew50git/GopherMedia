#include "../gopher_media.h"

/*
 * Game2D Methods
*/

Game2D::Game2D(std::string title, int x, int y, int width, int height, float frame_rate) {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    int sdlTTFResult = TTF_Init();
    if (sdlResult != 0) {
        SDL_Log("SDL initialization error: %s", SDL_GetError());
        init_success = false;
        return;
    }
    if (sdlTTFResult != 0) {
        SDL_Log("SDL font initialization error: %s", SDL_GetError());
        init_success = false;
        return;
    }
    window = SDL_CreateWindow(
        title.c_str(),
        x,
        y,
        width,
        height,
        0
    );
    if (!window) {
        SDL_Log("Window creation error: %s", SDL_GetError());
        init_success = false;
        return;
    }
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    init_success = true;
    is_running = true;
    ticks_count = 0;
    frame_rate_attr = frame_rate;
}

void Game2D::Update() {
    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );
    SDL_RenderClear(renderer);
    ProcessInput();
    WaitUntilFrame();
}

void Game2D::WaitUntilFrame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_count + 1000.0f / frame_rate_attr));
    float delta_time = (SDL_GetTicks() - ticks_count) / 1000.0f;
    if (delta_time > 0.05f) {
        delta_time = 0.05f;
    }
}

void Game2D::Delete() {
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
}

/*
 * Subclasses: Image, Font
*/

void Game2D::Image::Render(int x, int y, int width, int height) {
    SDL_Rect rect {
        x,
        y,
        width,
        height,
    };
    SDL_RenderCopyEx(renderer, SDL_image, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
}

void Game2D::Image::Render(int x, int y, int width, int height, const double angle, std::array<int, 2> rotation_point, 
                           ImageFlip flip) {
    SDL_Rect rect {
        x,
        y,
        width,
        height
    };
    SDL_Point SDL_rotation_point {
        rotation_point[0],
        rotation_point[1]
    };
    SDL_RendererFlip SDL_flip = SDL_FLIP_NONE;
    if (flip == horizontal) {
        SDL_flip = SDL_FLIP_HORIZONTAL;
    }
    if (flip == vertical) {
        SDL_flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(renderer, SDL_image, NULL, &rect, angle, &SDL_rotation_point, SDL_flip);
}

int Game2D::Image::GetWidth() {
    return width;
}

int Game2D::Image::GetHeight() {
    return height;
}

void Game2D::Font::Render(std::string text, int x, int y, std::array<Uint8, 4> color) {
    SDL_Color SDL_color = {color[0], color[1], color[2], color[3]};
    SDL_Surface* SDL_text_surface = TTF_RenderText_Solid(SDL_font, text.c_str(), SDL_color);
    SDL_Texture* SDL_text_image = SDL_CreateTextureFromSurface(renderer, SDL_text_surface);
    int text_width, text_height;
    SDL_QueryTexture(SDL_text_image, NULL, NULL, &text_width, &text_height);
    SDL_Rect rect {
        x,
        y,
        text_width,
        text_height
    };
    SDL_RenderCopyEx(renderer, SDL_text_image, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
}

void Game2D::Font::Render(std::string text, int x, int y, const double angle, std::array<int, 2> rotation_point, 
                          ImageFlip flip, std::array<Uint8, 4> color) {
    SDL_Color SDL_color = {color[0], color[1], color[2], color[3]};
    SDL_Surface* SDL_text_surface = TTF_RenderText_Solid(SDL_font, text.c_str(), SDL_color);
    SDL_Texture* SDL_text_image = SDL_CreateTextureFromSurface(renderer, SDL_text_surface);
    int text_width, text_height;
    SDL_QueryTexture(SDL_text_image, NULL, NULL, &text_width, &text_height);
    SDL_Rect rect {
        x,
        y,
        text_width,
        text_height
    };
    SDL_Point SDL_rotation_point {
        rotation_point[0],
        rotation_point[1]
    };
    SDL_RendererFlip SDL_flip = SDL_FLIP_NONE;
    if (flip == horizontal) {
        SDL_flip = SDL_FLIP_HORIZONTAL;
    }
    if (flip == vertical) {
        SDL_flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(renderer, SDL_text_image, NULL, &rect, angle, &SDL_rotation_point, SDL_flip);
}

/*
 * Graphics methods
*/

void Game2D::Rectangle(int startx, int starty, int endx, int endy, std::array<Uint8, 4> color) {
    SDL_Rect rect {
        startx,
        starty,
        endx,
        endy
    };
    SDL_SetRenderDrawColor(
        renderer,
        color[0],
        color[1],
        color[2],
        color[3]
    );
    SDL_RenderFillRect(renderer, &rect);
}

Game2D::Image Game2D::CreateImage(std::string file_path) {
    Image res_img;
    res_img.renderer = renderer;
    res_img.SDL_image = IMG_LoadTexture(renderer, file_path.c_str());
    SDL_QueryTexture(res_img.SDL_image, NULL, NULL, &res_img.width, &res_img.height);
    return res_img;
}

Game2D::Font Game2D::CreateFont(std::string file_path, int size) {
    Font res_font;
    res_font.renderer = renderer;
    res_font.SDL_font = TTF_OpenFont(file_path.c_str(), size);
    return res_font;
}

void Game2D::ShowOutput() {
    SDL_RenderPresent(renderer);
}
