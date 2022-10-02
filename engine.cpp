// NOLINT(legal/copyright)
#include "./engine.hpp"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

#include "./keybinds.hpp"

extern GLFWwindow *window;

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

struct Input {
    int key;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::duration pressed_for;
    bool released;  // has this key been released?
    bool press_handled;
};

// For the tetrimino that is currently being moved around
std::vector<std::vector<Block>> active_board;
// For the blocks that persist after pieces are locked in
std::vector<std::vector<Block>> locked_board;
std::array<std::array<Block, 4>, 4> active_piece;
// Why? So I can handle many many inputs played back very fast when I get to
// TASing simply by loading this deqeue.
std::deque<Input> inputs;
// Good luck getting rid of this one -- you can only get the keys in the main
// thread.
std::mutex input_mutex;

unsigned int *board;
std::atomic<bool> finish;  // When this is set, start should recognize that and
                           // return when it's ready.

// handling
// ...stl (if there's any more concise way of saying this, please tell me :) )
std::chrono::system_clock::duration DAS_threshold =
    std::chrono::duration_cast<std::chrono::system_clock::duration>(
        std::chrono::milliseconds(125));

std::chrono::system_clock::time_point DAS_time;

std::chrono::system_clock::duration ARR =
    std::chrono::duration_cast<std::chrono::system_clock::duration>(
        std::chrono::milliseconds(50));

void key_callback(GLFWwindow *, int key, int /* scancode */, int action,
                  int /* mods */) {
    input_mutex.lock();
    if (action == GLFW_PRESS) {
        inputs.push_back(Input{key, std::chrono::system_clock::now(),
                               std::chrono::system_clock::duration(), false,
                               false});
    } else {
        if (action == GLFW_RELEASE) {
            for (auto i = inputs.rbegin(); i != inputs.rend(); i++) {
                if (i->key == key && !i->released) {
                    i->pressed_for =
                        std::chrono::system_clock::now() - i->start_time;
                    i->released = true;
                    break;
                }
            }
        }
    }
    input_mutex.unlock();
}

void start(unsigned int *in_board) {
    board = in_board;
    while (!finish) {
        /* std::cout << "current input queue = {"; */
        input_mutex.lock();
        // using unordered_set bc constant time find
        auto now = std::chrono::system_clock::now();
        for (auto &input : inputs) {
            // update duration just before using
            if (!input.released) {
                input.pressed_for = now - input.start_time;
            }
            /* std::cout << glfwGetKeyName(input.key, 0) << ": [" */
            /*           << input.start_time.time_since_epoch() << ", " */
            /*           << input.pressed_for << ", " << std::boolalpha */
            /*           << input.released << std::noboolalpha << "], "; */
            if (input.key == Keybinds::move_left ||
                input.key == Keybinds::move_right) {
                bool move_right = input.key == Keybinds::move_right;
                if (!input.press_handled) {
                    // this time through the loop, this key is brand new
                    // so don't worry about DAS and move the piece immediately
                    // (so that tapping works)
                    std::cout << "move piece "
                              << (move_right ? "right" : "left") << std::endl;
                    input.press_handled = true;
                } else {
                    if (input.pressed_for > DAS_threshold) {
                        // start handling das
                        /* std::cout << "das activated, left" << std::endl; */
                        if (std::chrono::system_clock::now() > DAS_time) {
                            // move piece left, update das time (add ARR)
                            std::cout << "move piece "
                                      << (move_right ? "right" : "left")
                                      << " (das)" << std::endl;
                            DAS_time = std::chrono::system_clock::now() + ARR;
                        }
                    }
                }
            }
        }
        auto res = std::remove_if(inputs.begin(), inputs.end(),
                                  [](Input v) { return v.released; });
        if (res != inputs.end())
            inputs.erase(res);
        /* std::cout << "}[1A" << std::endl; */
        input_mutex.unlock();
    }
}
}  // namespace Engine
