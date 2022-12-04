// NOLINT(legal/copyright)
#pragma once

#include <span>

#include <cstdint>
#include <array>

class TetrisGrid {
    // TODO(ZWORX52): make deadicated shader class

 public:
    typedef uint8_t value_type;

 private:
    // grid that displays itself to the screen and manages all of that... yes
    // always flattened, I hope
    std::array<value_type, 220> grid;

    unsigned int shader_id;

 public:
    TetrisGrid();

    inline std::span<value_type> operator[](unsigned int index) {
        return std::span(grid.data() + (index / 10) * 10, 10);
    }

    /* inline std::span<value_type> at(unsigned int index) { */
    /*     ; */
    /* } */
};
