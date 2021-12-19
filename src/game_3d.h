#ifndef GOPHER_MEDIA_GAME_3D
#define GOPHER_MEDIA_GAME_3D
#include "gopher_media.h"

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
        void SetInputF(std::string name, Eigen::MatrixXf matrix);
        void SetInputI(std::string name, Eigen::VectorXi matrix); //vector only
        
        void SetInputUI(std::string name, std::vector<unsigned int> vector);
        void SetInputF(std::string name, std::vector<float> vector);
        void SetInputI(std::string name, std::vector<int> vector);

        void SetInputF(std::string name, std::array<float, 1> array);
        void SetInputF(std::string name, std::array<float, 2> array);
        void SetInputF(std::string name, std::array<float, 3> array);
        void SetInputF(std::string name, std::array<float, 4> array);
        void SetInputI(std::string name, std::array<int, 1> array);
        void SetInputI(std::string name, std::array<int, 2> array);
        void SetInputI(std::string name, std::array<int, 3> array);
        void SetInputI(std::string name, std::array<int, 4> array);
        void SetInputUI(std::string name, std::array<unsigned int, 1> array);
        void SetInputUI(std::string name, std::array<unsigned int, 2> array);
        void SetInputUI(std::string name, std::array<unsigned int, 3> array);
        void SetInputUI(std::string name, std::array<unsigned int, 4> array);

        void SetInputF(std::string name, ColorF color);
        void SetInputUC(std::string name, ColorUC color);

        void SetInputI(std::string name, RectangleI color);

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
        void LookAt(Point3F position, Point3F target, Vector3F up);
    private:
        Eigen::Matrix4f view_matrix;
        Eigen::Matrix4f projection_matrix;
    friend class Game3D;
    };
    class Object {
    public:
        Object(const float* vertices, unsigned int num_vertices, const unsigned int* indices,
                 unsigned int num_indices);
        void SetPosition(Point3F arg_position);
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
    Game3D(std::string title, RectangleI rectangle, float frame_rate_arg);
    void Update();
    void Delete();
    void RenderObject(Object& object, BaseShader& shader, Camera& camera);
    void ShowOutput();
    void StopLoop();
};

#endif