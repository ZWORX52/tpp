// NOLINT(legal/copyright)
#include "./engine.hpp"

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <array>

namespace Engine {
enum Block {
    Block_Z,
    Block_L,
    Block_O,
    Block_S,
    Block_J,
    Block_I,
    Block_T,
    Block_None,
    Block_Z_Ghost,
    Block_L_Ghost,
    Block_O_Ghost,
    Block_S_Ghost,
    Block_J_Ghost,
    Block_I_Ghost,
    Block_T_Ghost
};

// For the tetrimino that is currently being moved around
std::vector<std::vector<Block>> active_board;
// For the blocks that persist after pieces are locked in
std::vector<std::vector<Block>> locked_board;
std::array<std::array<Block, 4>, 4> active_piece;
unsigned int *board;
std::atomic<bool> finish;  // When this is set, start should recognize that and
                           // return when it's ready.

void start(unsigned int *in_board) {
    board = in_board;
    std::chrono::time_point start_time = std::chrono::system_clock::now();
    while (!finish) {
        std::cout << "hey now i would tick but i'm not lmao "
                  << std::chrono::system_clock::now() - start_time << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
}  // namespace Engine
