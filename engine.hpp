// NOLINT(legal/copyright)
#pragma once
#include <atomic>

namespace Engine {
extern std::atomic<bool> finish;
// called at main init to properly begin the engine thread. given a pointer to
// the board.
void start(unsigned int *in_board);
}  // namespace Engine
