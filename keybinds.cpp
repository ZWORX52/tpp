// NOLINT(legal/copyright)
#include "./keybinds.hpp"

#include <GLFW/glfw3.h>

namespace Keybinds {
// remember: these are defaults, NOT the only possible
int move_left = GLFW_KEY_LEFT;
int move_right = GLFW_KEY_RIGHT;
int soft_drop;
int hard_drop;
int rotate_cw;
int rotate_ccw;
int rotate_180;
int hold;
}  // namespace Keybinds
