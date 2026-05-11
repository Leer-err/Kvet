#ifndef GRAPHICS_CONFIG_H
#define GRAPHICS_CONFIG_H

#include <cstdint>

namespace Config {

enum class BufferingMode {
    NoBuffering,
    VSyncNoBuffering,
    DoubleBuffering,
    TripleBuffering
};

struct Graphics {
    uint32_t render_width;
    uint32_t render_height;

    BufferingMode buffering_mode;
};

};  // namespace Config

#endif  // GRAPHICS_CONFIG_H