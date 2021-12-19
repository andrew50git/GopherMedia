#include "../src/gopher_media.h"
#include <iostream>

void test_Game2D() {
    Game2D game("gopher_media", RectangleI(100, 100, 1024, 768), 60.0f);
    Game2D::Image gopher_image = game.CreateImage("../tests/GO_PARIS.png");
    Game2D::Font test_font = game.CreateFont("../tests/test.ttf", 60);
    ImageFlip curr_flip = none;
    if (game.GetSuccess()) {
        while (game.GetIsRunning()) {
            game.Update();
            gopher_image.Render(RectangleI(0, 0, gopher_image.GetWidth() / 4, gopher_image.GetHeight() / 4), 10.0f, 
                                {gopher_image.GetWidth() / 8, gopher_image.GetHeight() / 8}, curr_flip);
            game.RenderRectangleI(RectangleI(100, 100, 100, 200), ColorUC(255, 0, 0, 100));
            std::string mouse_pos_str = "Mouse position: " + std::to_string(game.GetMousePosition().x) + " " + std::to_string(game.GetMousePosition().y);
            test_font.Render(mouse_pos_str, Vector2I(100, 100), ColorUC(255, 255, 0, 200));
            game.ShowOutput();
        }
    }
    game.Delete();
}

int main() {
    test_Game2D();
    Quit();
    return 0;
}
