#ifndef GOPHER_MEDIA_BASIC
#define GOPHER_MEDIA_BASIC
#include "gopher_media.h"

enum ImageFlip {
    vertical,
    horizontal,
    none
};

enum MouseButton {
    left,
    middle,
    right
};

enum Axis {
    x,
    y,
    z
};

enum ShaderType {
    solid_color,
    textured,
    solid_color_lighting,
    textured_lighting
};

struct RectangleI {
    RectangleI(int arg_x, int arg_y, int arg_width, int arg_height);
    RectangleI();
    int x;
    int y;
    int width;
    int height;
};

struct ColorUC {
    ColorUC(unsigned char arg_r, unsigned char arg_g, unsigned char arg_b, unsigned char arg_a);
    ColorUC();
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct ColorF {
    ColorF(float arg_r, float arg_g, float arg_b, float arg_a);
    ColorF();
    float r;
    float g;
    float b;
    float a;
};

struct Vector2I {
    Vector2I(int arg_x, int arg_y);
    Vector2I();
    int x;
    int y;
private:
    Eigen::Vector2i ConvertToEigen();
friend class Game3D;
friend class Game2D;
};

struct Vector3I {
    Vector3I(int arg_x, int arg_y, int arg_z);
    Vector3I();
    int x;
    int y;
    int z;
private:
    Eigen::Vector3i ConvertToEigen();
friend class Game3D;
friend class Game2D;
};

struct Vector3F {
    Vector3F(float arg_x, float arg_y, float arg_z);
    Vector3F();
    float x;
    float y;
    float z;
private:
    Eigen::Vector3f ConvertToEigen();
friend class Game3D;
friend class Game2D;
};

void Quit();


typedef Vector2I Point2I;
typedef Vector3I Point3I;
typedef Vector3F Point3F;

#endif