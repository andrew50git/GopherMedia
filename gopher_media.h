#ifndef GOPHER_MEDIA_H
#define GOPHER_MEDIA_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <iostream>
#include <map>
#include <vector>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <sstream>
#include <functional>

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
struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

class BaseGame {
public:
    void Update();
    void Shutdown();
    void ShowOutput();
    bool GetSuccess();
    void StopLoop();
    bool GetIsRunning();
    bool GetKeyboardState(int keycode);
    std::array<int, 2> GetMousePos();
    bool GetMouseButton(MouseButton button);
private:
    void ProcessInput();
    SDL_Window* window;
    Uint32 ticks_count;
    float frame_rate;
    bool initialized;
    bool is_running;
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    int mouse_x, mouse_y;
    Uint32 mouse_buttons;
friend class Game2D;
friend class Game3D;
};


class Game2D : public BaseGame {
public:
    class Image {
    public:
        Image() {};
        int GetWidth();
        int GetHeight();
        void Render(int x, int y, int width, int height);
        void Render(int x, int y, int width, int height, const double angle,
                    std::array<int, 2> rotation_point, ImageFlip flip);
        void Delete();
    private:
        int width;
        int height;
        SDL_Texture* SDL_image;
        SDL_Renderer* renderer;
        bool initialized = true;
    friend class Game2D;
    };
    class Font {
    public:
        Font() {};
        void Render(std::string text, int x, int y, Color color);
        void Render(std::string text, int x, int y, const double angle, std::array<int, 2> rotation_point,
                    ImageFlip flip, Color color);
        void Delete();
    private:
        TTF_Font* SDL_font;
        SDL_Renderer* renderer;
        bool initialized = true;
    friend class Game2D;
    };
    Game2D(std::string title, int x, int y, int width, int height, float frame_rate_arg);
    void Update();
    void Delete();
    void ShowOutput();
    void StopLoop();
    Image CreateImage(std::string file_path);
    Font CreateFont(std::string file_path, int size);
    void Rectangle(int x, int y, int width, int height, Color color);
private:
    void WaitUntilFrame();
    SDL_Renderer* renderer;
};

class Game3D : public BaseGame {
private:
    SDL_GLContext gl_context;
    class VertexArray {
    public:
        VertexArray() {};
        VertexArray(const float* vertices, unsigned int num_vertices, const unsigned int* indices,
                    unsigned int num_indices);
        void Delete();
        unsigned int GetNumVertices();
        unsigned int GetNumIndices();
    private:
        unsigned int num_vertices;
        unsigned int num_indices;
        unsigned int vertex_buffer;
        unsigned int index_buffer;
        unsigned int vertex_array;
    friend class Game3D;
    };
public:
    class BaseShader {
    public:
        bool GetSuccess();
        void Delete();
        void SetInput(std::string name, Eigen::MatrixXf matrix);
        void SetInput(std::string name, std::vector<unsigned int> vector);
        void SetInput(std::string name, std::vector<float> vector);
        void SetInput(std::string name, std::vector<int> vector);
        virtual std::string PrintName() = 0;
    private:
        bool Compile(std::string content, GLenum type, GLuint& out);
        bool CheckShader(GLuint shader);
        bool CheckShaderProgram(GLuint shader);
        void SetActive();
        GLuint shader_program;
        GLuint vertex_shader;
        GLuint fragment_shader;
        bool program_success;
    friend class Game3D;
    };
    class CustomShader : public BaseShader {
    public:
        CustomShader(std::string vertex_file_path, std::string fragment_file_path);
        std::string PrintName();
    private:
    friend class Game3D;
    };
    class Camera {
    public:
        Camera(float fov, float height_to_width, float near, float far);
        void LookAt(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up);
    private:
        Eigen::Matrix4f view_matrix;
        Eigen::Matrix4f projection_matrix;
    friend class Game3D;
    };
    class Object {
    public:
        Object(const float* vertices, unsigned int num_vertices, const unsigned int* indices,
                 unsigned int num_indices);
        void SetPosition(float x, float y, float z);
        void SetRotation(Axis axis, float angle);
        void SetScale(float new_scale);
        void Delete();
    private:
        void ComputeWorldTransform();
        VertexArray vertex_array;
        Eigen::Vector3f position;
        float rotation_x;
        float rotation_y;
        float rotation_z;
        float scale;
        Eigen::Matrix4f world_transform;
    friend class Game3D;
    };
    Game3D(std::string title, int x, int y, int width, int height, float frame_rate_arg);
    void Update();
    void Delete();
    void DrawObject(Object& object, BaseShader& shader, Camera& camera);
    void ShowOutput();
    void StopLoop();
};

void Quit();

#endif

