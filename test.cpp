#include "gopher_media.h"
#include <iostream>

int main() {
    Game game("gopher_media", 100, 100, 1024, 768, 60.0f);
    Game::Image gopher_image = game.CreateImage("/home/andrew/gophers-master/GO_PARIS.png");
    Game::Font test_font = game.CreateFont("/home/andrew/Documents/Programs/C++/GopherMedia/test.ttf", 60);
    image_flip curr_flip = none;
    if (game.GetInitSuccess()) {
        while (game.GetIsRunning()) {
            game.Update();
            gopher_image.Render(0, 0, gopher_image.GetWidth() / 4, gopher_image.GetHeight() / 4, 0.0f, {gopher_image.GetWidth() / 16, gopher_image.GetHeight() / 16}, curr_flip);
            game.Rectangle(0, 0, 100, 100, {255, 0, 0, 100});
            test_font.Render("hi", 100, 100, {255, 255, 0, 200});
            game.ShowOutput();
        }
    }
    game.Shutdown();
    return 0;
}
