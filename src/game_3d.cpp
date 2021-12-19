#include "game_3d.h"

Game3D::Game3D(std::string title, RectangleI rectangle, float frame_rate_arg) {
    int sdl_result = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_result != 0) {
        SDL_Log("SDL initialization error: %s", SDL_GetError());
        initialized = false;
        return;
    }
    window = SDL_CreateWindow(
        title.c_str(),
        rectangle.x,
        rectangle.y,
        rectangle.width,
        rectangle.height,
        SDL_WINDOW_OPENGL
    ); 
    if (!window) {
        SDL_Log("Window creation error: %s", SDL_GetError());
        initialized = false;
        return;
    }
    is_running = true;
    ticks_count = 0;
    frame_rate = frame_rate_arg;
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glewExperimental = GL_TRUE;
    int glewResult = glewInit();
    if (glewResult != GLEW_OK) {
        SDL_Log("Could not init glew");
        initialized = false;
        return;
    }
    glGetError();
    initialized = true;
}

void Game3D::Delete() {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
}

void Game3D::Update() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ProcessInput();
}

void Game3D::RenderObject(Object& object, BaseShader& shader, Camera& camera) {
    shader.SetActive();
    glBindVertexArray(object.vertex_array.vertex_array);
    shader.SetInputF("world_transform", object.world_transform);
    shader.SetInputF("view_projection", camera.view_matrix * camera.projection_matrix);
    glDrawElements(
        GL_TRIANGLES,
        object.vertex_array.GetNumIndices(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

void Game3D::ShowOutput() {
    SDL_GL_SwapWindow(window);
}

unsigned int Game3D::VertexArray::GetNumVertices() {
    return num_vertices;
}

unsigned int Game3D::VertexArray::GetNumIndices() {
    return num_indices;
}


Game3D::VertexArray::VertexArray(const float* vertices, unsigned int arg_num_vertices, 
                                 const unsigned int* indices, unsigned int arg_num_indices) {
    num_vertices = arg_num_vertices;
    num_indices = arg_num_indices;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        num_vertices * 3 * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );
    
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        num_indices * sizeof(unsigned int),
        indices,
        GL_STATIC_DRAW
    );
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 3,
        0
    );
}

void Game3D::VertexArray::Delete() {
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteVertexArrays(1, &vertex_array);
}



void Game3D::BaseShader::SetActive() {
    glUseProgram(shader_program);
}

bool Game3D::BaseShader::Compile(std::string content, GLenum type, GLuint& out) {
    const char* shader_content_char = content.c_str();
    out = glCreateShader(type);
    glShaderSource(out, 1, &shader_content_char, nullptr);
    glCompileShader(out);
    if (!CheckShader(out)) {
        SDL_Log("Compilation of failed");
        return false;
    }
    return true;
}

bool Game3D::BaseShader::CheckShader(GLuint arg_shader) {
    GLint status;
    glGetShaderiv(arg_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char buffer[2048];
        memset(buffer, 0, 2048);
        glGetShaderInfoLog(arg_shader, 2047, nullptr, buffer);
        SDL_Log("Compilation failed for shader %s", buffer);
        return false;
    }
    return true;
}

bool Game3D::BaseShader::CheckShaderProgram(GLuint arg_shader_program) {
    GLint status;
    glGetProgramiv(arg_shader_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        char buffer[2048];
        memset(buffer, 0, 2048);
        glGetProgramInfoLog(arg_shader_program, 2047, nullptr, buffer);
        SDL_Log("Compilation failed for program %s", buffer);
        return false;
    }
    return true;
}

bool Game3D::BaseShader::GetSuccess() {
    return program_success;
}


void Game3D::BaseShader::Delete() {
    glDeleteProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Game3D::BaseShader::SetInputF(std::string name, Eigen::MatrixXf matrix) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    int rows = matrix.rows();
    int cols = matrix.cols();
    if (rows == 2 && cols == 2) {
        glUniformMatrix2fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 2 && cols == 3) {
        glUniformMatrix2x3fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 3 && cols == 2) {
        glUniformMatrix3x2fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 3 && cols == 3) {
        glUniformMatrix3fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 3 && cols == 4) {
        glUniformMatrix3x4fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 4 && cols == 3) {
        glUniformMatrix4x3fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 4 && cols == 4) {
        glUniformMatrix4fv(input_loc, 1, GL_TRUE, matrix.data());
    } else if (rows == 2 && cols == 1) {
        glUniform2f(input_loc, matrix(0), matrix(1));
    } else if (rows == 3 && cols == 1) {
        glUniform3f(input_loc, matrix(0), matrix(1), matrix(2));
    } else if (rows == 4 && cols == 1) {
        glUniform4f(input_loc, matrix(0), matrix(1), matrix(2), matrix(3));
    } else {
        SDL_Log("Tried to set input to invalid Eigen matrix/vector size");
    }
}

void Game3D::BaseShader::SetInputI(std::string name, Eigen::VectorXi vector) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    int rows = vector.rows();
    int cols = vector.cols();
    if (rows == 2 && cols == 1) {
        glUniform2f(input_loc, vector(0), vector(1));
    } else if (rows == 3 && cols == 1) {
        glUniform3f(input_loc, vector(0), vector(1), vector(2));
    } else if (rows == 4 && cols == 1) {
        glUniform4f(input_loc, vector(0), vector(1), vector(2), vector(3));
    } else {
        SDL_Log("Tried to set input to invalid Eigen vector size");
    }
}

void Game3D::BaseShader::SetInputUI(std::string name, std::vector<unsigned int> vector) {
    switch(vector.size()) {
        case 1: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform1ui(input_loc, vector[0]);
            break;
        }
        case 2: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform2ui(input_loc, vector[0], vector[1]);
            break;
        }
        case 3: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform3ui(input_loc, vector[0], vector[1], vector[2]);
            break;
        }
        case 4: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform4ui(input_loc, vector[0], vector[1], vector[2], vector[3]);
            break;
        }
        default: {
            SDL_Log("Tried to set input to invalid vector size");
        }
    }
}

void Game3D::BaseShader::SetInputF(std::string name, std::vector<float> vector) {
    switch(vector.size()) {
        case 1: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform1f(input_loc, vector[0]);
            break;
        }
        case 2: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform2f(input_loc, vector[0], vector[1]);
            break;
        }
        case 3: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform3f(input_loc, vector[0], vector[1], vector[2]);
            break;
        }
        case 4: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform4f(input_loc, vector[0], vector[1], vector[2], vector[3]);
            break;
        }
        default: {
            SDL_Log("Tried to set input to invalid vector size");
        }
    }
}

void Game3D::BaseShader::SetInputI(std::string name, std::vector<int> vector) {
    switch(vector.size()) {
        case 1: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform1i(input_loc, vector[0]);
            break;
        }
        case 2: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform2i(input_loc, vector[0], vector[1]);
            break;
        }
        case 3: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform3i(input_loc, vector[0], vector[1], vector[2]);
            break;
        }
        case 4: {
            GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
            glUniform4i(input_loc, vector[0], vector[1], vector[2], vector[3]);
            break;
        }
        default: {
            SDL_Log("Tried to set input to invalid vector size");
        }
    }
}

void Game3D::BaseShader::SetInputF(std::string name, std::array<float, 1> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform1f(input_loc, array[0]);
}

void Game3D::BaseShader::SetInputF(std::string name, std::array<float, 2> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform2f(input_loc, array[0], array[1]);
}

void Game3D::BaseShader::SetInputF(std::string name, std::array<float, 3> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform3f(input_loc, array[0], array[1], array[2]);
}

void Game3D::BaseShader::SetInputF(std::string name, std::array<float, 4> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4f(input_loc, array[0], array[1], array[2], array[3]);
}

void Game3D::BaseShader::SetInputI(std::string name, std::array<int, 1> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform1i(input_loc, array[0]);
}

void Game3D::BaseShader::SetInputI(std::string name, std::array<int, 2> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform2i(input_loc, array[0], array[1]);
}

void Game3D::BaseShader::SetInputI(std::string name, std::array<int, 3> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform3i(input_loc, array[0], array[1], array[2]);
}

void Game3D::BaseShader::SetInputI(std::string name, std::array<int, 4> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4i(input_loc, array[0], array[1], array[2], array[3]);
}

void Game3D::BaseShader::SetInputUI(std::string name, std::array<unsigned int, 1> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform1ui(input_loc, array[0]);
}

void Game3D::BaseShader::SetInputUI(std::string name, std::array<unsigned int, 2> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform2ui(input_loc, array[0], array[1]);
}

void Game3D::BaseShader::SetInputUI(std::string name, std::array<unsigned int, 3> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform3ui(input_loc, array[0], array[1], array[2]);
}

void Game3D::BaseShader::SetInputUI(std::string name, std::array<unsigned int, 4> array) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4ui(input_loc, array[0], array[1], array[2], array[3]);
}

void Game3D::BaseShader::SetInputF(std::string name, ColorF color) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4f(input_loc, color.r, color.g, color.b, color.a);
}

void Game3D::BaseShader::SetInputUC(std::string name, ColorUC color) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4f(input_loc, (float) color.r / 255, (float) color.g / 255, (float) color.b / 255, (float) color.a / 255);
}

void Game3D::BaseShader::SetInputI(std::string name, RectangleI rectangle) {
    GLuint input_loc = glGetUniformLocation(shader_program, name.c_str());
    glUniform4i(input_loc, rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}


Game3D::CustomShader::CustomShader(std::string vertex_file_path, std::string fragment_file_path) {
    std::ifstream vertex_fin(vertex_file_path);
    std::string vertex_content((std::istreambuf_iterator<char>(vertex_fin)),
                       (std::istreambuf_iterator<char>()));
    bool vert_status = Compile(vertex_content, GL_VERTEX_SHADER, vertex_shader);
    
    std::ifstream fragment_fin(fragment_file_path);
    std::string fragment_content((std::istreambuf_iterator<char>(fragment_fin)),
                       (std::istreambuf_iterator<char>()));
    bool frag_status = Compile(fragment_content, GL_FRAGMENT_SHADER, fragment_shader);
    
    if (!vert_status || !frag_status) {
        program_success = false;
        return;
    }
    shader_program = glCreateProgram();
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, vertex_shader);
    glLinkProgram(shader_program);
    if (!CheckShaderProgram(shader_program)) {
        program_success = false;
        return;
    }
    program_success = true;
}

std::string Game3D::CustomShader::PrintName() {
    return "CustomShader";
}

Game3D::Object::Object(const float* vertices, unsigned int num_vertices, 
                           const unsigned int* indices, unsigned int num_indices) {
    vertex_array = VertexArray(vertices, num_vertices, indices, num_indices);
    rotation_x = 0;
    rotation_y = 0;
    rotation_z = 0;
    world_transform.setIdentity();
    scale = 1;
    position = {0, 0, 0};
}

void Game3D::Object::Delete() {
    vertex_array.Delete();
}

void Game3D::Object::SetPosition(Point3F arg_position) {
    position = {arg_position.x, arg_position.y, arg_position.z};
    ComputeWorldTransform();
}

void Game3D::Object::SetRotation(Axis axis, float angle) {
    if (axis == x) {
        rotation_x = angle;
    } else if (axis == y) {
        rotation_y = angle;
    } else if (axis == z) {
        rotation_z = angle;
    }
    ComputeWorldTransform();
}

void Game3D::Object::SetScale(float new_scale) {
    scale = new_scale;
    ComputeWorldTransform();
}

void Game3D::Object::ComputeWorldTransform() {
    Eigen::Transform<float, 3, Eigen::Affine> temp_transform;
    temp_transform = Eigen::Translation3f(position) * 
                    Eigen::AngleAxisf(rotation_x, Eigen::Vector3f::UnitX()) * 
                    Eigen::AngleAxisf(rotation_y, Eigen::Vector3f::UnitY()) * 
                    Eigen::AngleAxisf(rotation_z, Eigen::Vector3f::UnitZ()) * 
                    Eigen::Scaling(scale);
    world_transform = temp_transform.matrix();
}

//Roses are red, violets are blue, https://stackoverflow.com/a/13786235/12620352, thank you.

Game3D::Camera::Camera(float fov, float height_to_width, float near, float far) {
    projection_matrix.setIdentity();
    view_matrix.setZero();
    float range = far - near;
    float invtan = 1.0 / tan(fov/2);
    projection_matrix(0,0) = invtan * height_to_width;
    projection_matrix(1,1) = invtan;
    projection_matrix(2,2) = far / range;
    projection_matrix(3,2) = 1;
    projection_matrix(2,3) = -1 * near * far / range;
    projection_matrix(3,3) = 0;
    LookAt(Point3F(0, 0, 0), Point3F(0, 0, 1), Vector3F(0, 1, 0));
}


//Roses are red, violets are blue, https://stackoverflow.com/a/13786235/12620352, thank you.

void Game3D::Camera::LookAt(Point3F position, Point3F target, Vector3F up) {
    Eigen::Matrix3f R;
    R.col(2) = (position.ConvertToEigen()-target.ConvertToEigen()).normalized();
    R.col(0) = up.ConvertToEigen().cross(R.col(2)).normalized();
    R.col(1) = R.col(2).cross(R.col(0));
    view_matrix.topLeftCorner<3,3>() = R.transpose();
    view_matrix.topRightCorner<3,1>() = -R.transpose() * position.ConvertToEigen();
    view_matrix(3,3) = 1.0f;
}
