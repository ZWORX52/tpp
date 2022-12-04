// NOLINT(legal/copyright)
#include "./tetris_grid.hpp"

#include <span>

#include <cstdint>
#include <string>
#include <vector>

TetrisGrid::TetrisGrid() {
    const std::string vertex_shader = "#version 410\n"
                                      "layout (location = 0) in vec3 in_pos;\n"
                                      "out vec3 out_pos;\n"
                                      "void main() {\n"
                                      "    gl_Position = vec4(in_pos, 0.0f);\n"
                                      "}\n";
    const std::string fragment_shader =
        "#version 410\n"
        "in vec3 out_pos;\n"
        "uniform uint board[220];\n"
        "uniform vec4 colors[8];\n"
        "out vec4 fragment_color;\n"
        "void main() {\n"
        "    ivec2 idxs = (clamp(floor((out_pos.x + 1) * 5), 0, 9), "
        "                  clamp(floor((out_pos.y + 1) * 11), 0, 21));\n"
        "    fragment_color = colors[board[idxs.x + idxs.y * 10]];\n"
        "}\n";
}
