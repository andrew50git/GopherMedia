#include "../gopher_media.h"
#include <iostream>

void test_Game2D() {
    Game2D game("gopher_media", 100, 100, 1024, 768, 60.0f);
    Game2D::Image gopher_image = game.CreateImage("/home/andrew/gophers-master/GO_PARIS.png");
    Game2D::Font test_font = game.CreateFont("/home/andrew/Documents/Programs/C++/GopherMedia/tests/test.ttf", 60);
    ImageFlip curr_flip = none;
    if (game.GetSuccess()) {
        while (game.GetIsRunning()) {
            game.Update();
            gopher_image.Render(0, 0, gopher_image.GetWidth() / 4, gopher_image.GetHeight() / 4, 0.0f, 
            {gopher_image.GetWidth() / 16, gopher_image.GetHeight() / 16}, curr_flip);
            game.Rectangle(0, 0, 100, 100, {255, 0, 0, 100});
            test_font.Render("hi", 100, 100, {255, 255, 0, 200});
            game.ShowOutput();
        }
    }
    game.Delete();
}

int main() {
    test_Game2D();
    return 0;
}
