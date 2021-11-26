#include "../gopher_media.h"

/*
 * Game2D Methods
*/

Game2D::Game2D(std::string title, Eigen::AlignedBox2i rectangle, float frame_rate_arg) {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    int sdl_result = SDL_Init(SDL_INIT_VIDEO);
    int sdl_ttf_result = TTF_Init();
    if (sdl_result != 0) {
        SDL_Log("SDL initialization error: %s", SDL_GetError());
        initialized = false;
        return;
    }
    if (sdl_ttf_result != 0) {
        SDL_Log("SDL font initialization error: %s", SDL_GetError());
        initialized = false;
        return;
    }
    window = SDL_CreateWindow(
        title.c_str(),
        rectangle.min()(0),
        rectangle.min()(1),
        rectangle.max()(0) - rectangle.min()(0),
        rectangle.max()(1) - rectangle.min()(1),
        0
    );
    if (!window) {
        SDL_Log("Window creation error: %s", SDL_GetError());
        initialized = false;
        return;
    }
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    is_running = true;
    ticks_count = 0;
    frame_rate = frame_rate_arg;
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


void Game2D::ShowOutput() {
    SDL_RenderPresent(renderer);
}

void Game2D::WaitUntilFrame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticks_count + 1000.0f / frame_rate));
    float delta_time = (SDL_GetTicks() - ticks_count) / 1000.0f;
    if (delta_time > 0.05f) {
        delta_time = 0.05f;
    }
}

void Game2D::Delete() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    initialized = false;
}

/*
 * Subclasses: Image, Font
*/

void Game2D::Image::Render(Eigen::AlignedBox2i rectangle) {
    if (!initialized) {
        SDL_Log("Attempted to render broken image");
        return;
    }
    if (width < 0) {
        SDL_Log("Width is less than 0: %d", width);
        return;
    }
    if (height < 0) {
        SDL_Log("Height is less than 0: %d", height);
        return;
    }
    SDL_Rect rect {
        rectangle.min()(0),
        rectangle.min()(1),
        rectangle.max()(0) - rectangle.min()(0),
        rectangle.max()(1) - rectangle.min()(1)
    };
    SDL_RenderCopyEx(renderer, SDL_image, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
}

void Game2D::Image::Render(Eigen::AlignedBox2i rectangle, const double angle, Eigen::Vector2i rotation_point, 
                           ImageFlip flip) {
    if (!initialized) {
        SDL_Log("Attempted to render broken image");
        return;
    }
    if (width < 0) {
        SDL_Log("Width is less than 0: %d", width);
        return;
    }
    if (height < 0) {
        SDL_Log("Height is less than 0: %d", height);
        return;
    }
    SDL_Rect rect {
        rectangle.min()(0),
        rectangle.min()(1),
        rectangle.max()(0) - rectangle.min()(0),
        rectangle.max()(1) - rectangle.min()(1)
    };
    SDL_Point SDL_rotation_point {
        rotation_point(0),
        rotation_point(1)
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

void Game2D::Font::Render(std::string text, Eigen::Vector2i text_pos, Color color) {
    if (!initialized) {
        SDL_Log("Attempted to render broken font");
        return;
    }
    SDL_Color SDL_color = {color.r, color.g, color.b, color.a};
    SDL_Surface* SDL_text_surface = TTF_RenderText_Solid(SDL_font, text.c_str(), SDL_color);
    SDL_Texture* SDL_text_image = SDL_CreateTextureFromSurface(renderer, SDL_text_surface);
    int text_width, text_height;
    SDL_QueryTexture(SDL_text_image, NULL, NULL, &text_width, &text_height);
    SDL_Rect rect {
        text_pos(0),
        text_pos(1),
        text_width,
        text_height
    };
    SDL_RenderCopyEx(renderer, SDL_text_image, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
}

void Game2D::Font::Render(std::string text, Eigen::Vector2i text_pos, const double angle, Eigen::Vector2i rotation_point, 
                          ImageFlip flip, Color color) {
    if (!initialized) {
        SDL_Log("Attempted to render broken font");
        return;
    }
    SDL_Color SDL_color = {color.r, color.g, color.b, color.a};
    SDL_Surface* SDL_text_surface = TTF_RenderText_Solid(SDL_font, text.c_str(), SDL_color);
    SDL_Texture* SDL_text_image = SDL_CreateTextureFromSurface(renderer, SDL_text_surface);
    int text_width, text_height;
    SDL_QueryTexture(SDL_text_image, NULL, NULL, &text_width, &text_height);
    SDL_Rect rect {
        text_pos(0),
        text_pos(1),
        text_width,
        text_height
    };
    SDL_Point SDL_rotation_point {
        rotation_point(0),
        rotation_point(1)
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

void Game2D::RenderRectangle(Eigen::AlignedBox2i rectangle, Color color) {
    if (rectangle.max()(0) - rectangle.min()(0) < 0) {
        SDL_Log("Width is less than 0: %d", rectangle.max()(0) - rectangle.min()(0));
        return;
    }
    if (rectangle.max()(1) - rectangle.min()(1) < 0) {
        SDL_Log("Height is less than 0: %d", rectangle.max()(1) - rectangle.min()(1));
        return;
    }
    SDL_Rect rect {
        rectangle.min()(0),
        rectangle.min()(1),
        rectangle.max()(0) - rectangle.min()(0),
        rectangle.max()(1) - rectangle.min()(1)
    };
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );
    SDL_RenderFillRect(renderer, &rect);
}

Game2D::Image Game2D::CreateImage(std::string file_path) {
    Image res_img;
    res_img.initialized = true;
    res_img.renderer = renderer;
    res_img.SDL_image = IMG_LoadTexture(renderer, file_path.c_str());
    if (res_img.SDL_image == NULL) {
        res_img.initialized = false;
        SDL_Log("Could not create image from %s: %s", file_path.c_str(), SDL_GetError());
        return res_img;
    }
    SDL_QueryTexture(res_img.SDL_image, NULL, NULL, &res_img.width, &res_img.height);
    return res_img;
}

Game2D::Font Game2D::CreateFont(std::string file_path, int size) {
    Font res_font;
    res_font.initialized = true;
    res_font.renderer = renderer;
    res_font.SDL_font = TTF_OpenFont(file_path.c_str(), size);
    if (res_font.SDL_font == NULL) {
        res_font.initialized = false;
        SDL_Log("Could not create font from %s: %s", file_path.c_str(), SDL_GetError());
        return res_font;
    }
    return res_font;
}

void Game2D::Image::Delete() {
    SDL_DestroyTexture(SDL_image);
    initialized = false;
}

void Game2D::Font::Delete() {
    TTF_CloseFont(SDL_font);
    initialized = false;
}

