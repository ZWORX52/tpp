// NOLINT(legal/copyright)
#include "./grid.hpp"

#include <span>

#include <cstdint>
#include <string>
#include <vector>

constexpr unsigned int Grid::width = 10;

Grid::Grid() {
    const std::string vertex_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 in_pos;\n"
        "out vec3 out_pos;\n"
        "void main() {\n"
        "    out_pos = vec4(in_pos, 0.0f)"
        "    gl_Position = vec4(in_pos, 0.0f);\n"
        "}\n";
    const std::string fragment_shader = "#version 410\n"
                                        "in vec3 out_pos;\n"
                                        "uniform uint board[220];\n"
                                        "uniform vec4 colors[8];\n"
                                        "void main() {\n"
                                        "    "
}
