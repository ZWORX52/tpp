// NOLINT(legal/copyright)
#include "./main.hpp"

void glfw_error_callback(int code, const char * msg) {
    std::cout << "GLFW Error #" << code << ": " << msg << std::endl;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!" << std::endl;
        return 1;
    }

    glfwTerminate();
}

