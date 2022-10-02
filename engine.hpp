// NOLINT(legal/copyright)
#pragma once
#include <GLFW/glfw3.h>

#include <atomic>

namespace Engine {
extern std::atomic<bool> finish;
// called at main init to properly begin the engine thread. given a pointer to
// the board.

void key_callback(GLFWwindow *, int key, int, int action, int);
void start(unsigned int *in_board);
}  // namespace Engine
