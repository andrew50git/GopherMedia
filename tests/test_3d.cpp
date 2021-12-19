#include "../src/gopher_media.h"
#include <iostream>

void test_Game3D() {
    Game3D game("gopher_media", RectangleI(100, 100, 1024, 512), 60.0f);
    const float vertex_buffer[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    const unsigned int index_buffer[] = {
        0, 1, 2,
        2, 3, 0
    };
    Game3D::Object object(vertex_buffer, 4, index_buffer, 6);
    Game3D::CustomShader shader("../shaders/test_shader.vert", "../shaders/test_shader.frag");
    Game3D::Camera camera(100, 0.5, 1, 1000);
    object.SetPosition(Point3F(0, 0, 10));
    float rotation = 0;
    if (shader.GetSuccess() && game.GetSuccess()) {
        while (game.GetIsRunning()) {
            game.Update();
            object.SetRotation(x, rotation);
            shader.SetInputF("color", ColorF(1.0, 0.5, 0.0, 0.5));
            game.RenderObject(object, shader, camera);
            rotation += 0.1;
            game.ShowOutput();
        }
    }
    object.Delete();
    shader.Delete();
    game.Delete();
}

int main() {
    test_Game3D();
    Quit();
    return 0;
}
