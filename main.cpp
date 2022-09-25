// NOLINT(legal/copyright)
#include "./main.hpp"

#include <glad/glad.h>
// Space is required because otherwise some formatter reorganizes them and they
// have a required order so L
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const int rectangle_indices[] = {0, 1, 3, 1, 2, 3};
// how else do you propose I do this?
int screen_width = 640, screen_height = 480;

void glfw_error_callback(int code, const char *msg) {
    std::cout << "GLFW Error #" << code << ": " << msg << std::endl;
}

void glfw_framebuffer_size_callback(__attribute__((unused)) GLFWwindow *window,
                                    int width, int height) {
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}

unsigned int load_shader(const std::string &path_vertex,
                         const std::string &path_fragment) {
    std::string vertex_code, fragment_code;
    std::ifstream vertex_file, fragment_file;

    // throw exceptions please
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertex_file.open(path_vertex);
        fragment_file.open(path_fragment);
        std::stringstream vertex_stream, fragment_stream;
        // read files
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();
        vertex_file.close();
        fragment_file.close();
        vertex_code = vertex_stream.str();
        fragment_code = fragment_stream.str();
    } catch (const std::ifstream::failure &e) {
        std::cout << "Failed reading shader files " << path_vertex << " and "
                  << path_fragment << " (error code #" << e.code()
                  << "):" << std::endl
                  << e.what() << std::endl;
        return 0;
    }

    int success;
    char log[1024];

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // Because you can't take the address of an rvalue :P
    const char *vertex_code_c_str = vertex_code.c_str();
    glShaderSource(vertex_shader, 1, &vertex_code_c_str, NULL);
    glCompileShader(vertex_shader);
    // any compile errors?
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log);
        std::cout << "Vertex shader compilation failed: " << log << std::endl;
        return 0;
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_code_c_str = fragment_code.c_str();
    glShaderSource(fragment_shader, 1, &fragment_code_c_str, NULL);
    glCompileShader(fragment_shader);
    // any compile errors here?
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 1024, NULL, log);
        std::cout << "Fragment shader compilation failed: " << log << std::endl;
        return 0;
    }

    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    // any linking errors?
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id, 1024, NULL, log);
        std::cout << "Program linking failed: " << log << std::endl;
        return 0;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program_id;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    GLFWwindow *window = glfwCreateWindow(640, 480, "T++", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        int error = glfwGetError(NULL);
        if (error == GLFW_VERSION_UNAVAILABLE) {
            std::cout << "Hint: your computer doesn't appear to support OpenGL "
                         "version 4.1!"
                      << std::endl;
        }
        return 1;
    }
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }
    glfwSwapInterval(1);
    glViewport(0, 0, 640, 480);
    glEnable(GL_DEBUG_OUTPUT);

    // gimme dat shader plz
    unsigned int program = load_shader("../main.vert", "../main.frag");
    if (!program) {
        return 1;
    }
    float board_attributes[] = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                                -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
    // i know that uint is inefficient. stfu.
    unsigned int board[] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(board_attributes), board_attributes,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices),
                 rectangle_indices, GL_STATIC_DRAW);

    // position attributes!
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glUseProgram(program);
    glUniform2i(glGetUniformLocation(program, "board_dims"), screen_width / 2,
                screen_height / 2);
    glUniform2i(glGetUniformLocation(program, "board_loc"), screen_width / 4,
                screen_height / 4);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform1uiv(glGetUniformLocation(program, "board"), 200, board);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
