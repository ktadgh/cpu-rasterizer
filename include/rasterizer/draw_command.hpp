#pragma once
#include <rasterizer/mesh.hpp>

namespace rasterizer
{

    struct draw_command
    {
        struct mesh mesh;
        matrix4x4f transform = matrix4x4f::identity();

    };
}