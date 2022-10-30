// NOLINT(legal/copyright)
#include <glad/glad.h>
// spaces are required because otherwise neoformat will sort them
// lexicographically and that angers cpplint because of the header order. it's
// also more organized so that's a plus
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "./engine.hpp"

const int rectangle_indices[] = {0, 1, 3, 1, 2, 3};
const int START_SCREEN_WIDTH = 240, START_SCREEN_HEIGHT = 480;
// how else do you propose I do this?
int screen_width = START_SCREEN_WIDTH, screen_height = START_SCREEN_HEIGHT;
GLFWwindow *window;

void glfw_error_callback(int code, const char *msg) {
    std::cout << "GLFW Error #" << code << ": " << msg << std::endl;
}

void glfw_framebuffer_size_callback(__attribute__((unused)) GLFWwindow *window,
                                    int width, int height) {
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}

// nyeh

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    window = glfwCreateWindow(START_SCREEN_WIDTH, START_SCREEN_HEIGHT, "T++",
                              NULL, NULL);
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
    glfwSetKeyCallback(window, Engine::key_callback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return 1;
    }
    glfwSwapInterval(1);
    glViewport(0, 0, START_SCREEN_WIDTH, START_SCREEN_HEIGHT);
    glEnable(GL_DEBUG_OUTPUT);

    unsigned int board[220] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    std::thread engine(Engine::start, board);

    // gimme dat shader plz
    /*
    unsigned int program = load_shader("../main.vert", "../main.frag");
    if (!program) {
        return 1;
    }
    // starting top right, going clockwise.
    float board_attributes[] = {0.8f, 0.8f,  0.0f, 0.8,   -0.8f, 0.0f,
                                -0.8, -0.8f, 0.0f, -0.8f, 0.8f,  0.0f};
    float background_attributes[] = {1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f,
                                     -1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f};

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(board_attributes), board_attributes,
                 GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices),
                 rectangle_indices, GL_STATIC_DRAW);

    // position attributes!
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    unsigned int background_vao, background_vbo;
    glGenVertexArrays(1, &background_vao);
    glGenBuffers(1, &background_vbo);

    glBindVertexArray(background_vao);
    glBindBuffer(GL_ARRAY_BUFFER, background_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background_attributes),
                 background_attributes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glUseProgram(program);

    */

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glUniform1uiv(glGetUniformLocation(program, "board"), 220, board);
        glUniform2i(glGetUniformLocation(program, "board_dims"),
                    (screen_width * 4) / 5, (screen_height * 4) / 5);
        glUniform2i(glGetUniformLocation(program, "board_loc"),
                    screen_width / 10, screen_height / 10);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Engine::finish = true;
    engine.join();

    glfwTerminate();
}
