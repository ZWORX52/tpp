// NOLINT(legal/copyright)
#pragma once

#include <span>

#include <cstdint>
#include <array>

class TetrisGrid {
 public:
    typedef uint8_t value_type;

 private:
    // grid that displays itself to the screen and manages all of that... yes
    // always flattened, I hope
    std::array<value_type, 220> grid;

 public:
    Grid();

    inline std::span<value_type> operator[](unsigned int index) {
        return std::span(grid.data() + (index / width) * width, width);
    }

    inline std::span<value_type> at(unsigned int index) {
        ;
    }
};
