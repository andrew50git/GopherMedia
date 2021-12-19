#include "basic.h"

RectangleI::RectangleI(int arg_x, int arg_y, int arg_width, int arg_height) {
    x = arg_x;
    y = arg_y;
    width = arg_width;
    height = arg_height;
}

RectangleI::RectangleI() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

ColorUC::ColorUC(unsigned char arg_r, unsigned char arg_g, unsigned char arg_b, unsigned char arg_a) {
    r = arg_r;
    g = arg_g;
    b = arg_b;
    a = arg_a;
}

ColorUC::ColorUC() {
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}

ColorF::ColorF(float arg_r, float arg_g, float arg_b, float arg_a) {
    r = arg_r;
    g = arg_g;
    b = arg_b;
    a = arg_a;
}

ColorF::ColorF() {
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}

Vector2I::Vector2I(int arg_x, int arg_y) {
    x = arg_x;
    y = arg_y;
}

Vector2I::Vector2I() {
    x = 0;
    y = 0;
}

Eigen::Vector2i Vector2I::ConvertToEigen() {
    return Eigen::Vector2i({x, y});
}

Vector3I::Vector3I(int arg_x, int arg_y, int arg_z) {
    x = arg_x;
    y = arg_y;
    z = arg_z;
}

Vector3I::Vector3I() {
    x = 0;
    y = 0;
    z = 0;
}

Eigen::Vector3i Vector3I::ConvertToEigen() {
    return Eigen::Vector3i({x, y, z});
}

Vector3F::Vector3F(float arg_x, float arg_y, float arg_z) {
    x = arg_x;
    y = arg_y;
    z = arg_z;
}

Vector3F::Vector3F() {
    x = 0;
    y = 0;
    z = 0;
}

Eigen::Vector3f Vector3F::ConvertToEigen() {
    return Eigen::Vector3f({x, y, z});
}

void Quit() {
    SDL_Quit();
}

