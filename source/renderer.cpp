#include <rasterizer/renderer.hpp>
#include <rasterizer/viewport.hpp>

#include <cmath>
#include <algorithm>

namespace rasterizer
{

    void clear(image_view const & color_buffer, vector4f const & color)
    {
        auto ptr = color_buffer.pixels;
        auto size = color_buffer.width * color_buffer.height;
        std::fill_n(ptr, size, to_color4ub(color));
    }


    void draw(image_view const & color_buffer, draw_command const & command)
    {
        for (std::uint32_t vertex_index = 0;
            vertex_index + 2 < command.mesh.vertex_count;
            vertex_index += 3)
        {
            auto v0 = command.transform * as_point(command.mesh.positions[vertex_index + 0]);
            auto v1 = command.transform * as_point(command.mesh.positions[vertex_index + 1]);
            auto v2 = command.transform * as_point(command.mesh.positions[vertex_index + 2]);
            v0 = apply(viewport, v0);
            v1 = apply(viewport, v1);
            v2 = apply(viewport, v2);
            auto c0 = command.mesh.colors[vertex_index + 0];
            auto c1 = command.mesh.colors[vertex_index + 1];
            auto c2 = command.mesh.colors[vertex_index + 2];

            std::int32_t xmin = std::max<std::int32_t>(viewport.xmin, 0);
            std::int32_t xmax = std::min<std::int32_t>(viewport.xmax, color_buffer.width) - 1;
            std::int32_t ymin = std::max<std::int32_t>(viewport.ymin, 0);
            std::int32_t ymax = std::min<std::int32_t>(viewport.ymax, color_buffer.height) - 1;

            xmin = std::max<std::int32_t>(0, xmin);
            xmax = std::min<std::int32_t>(color_buffer.width - 1, xmax);
            ymin = std::max<std::int32_t>(0, ymin);
            ymax = std::min<std::int32_t>(color_buffer.height - 1, ymax);
            for (std::int32_t y = ymin; y <= ymax; ++y)
            {
                for (std::int32_t x = xmin; x <= xmax; ++x)
                {
                    vector4f p{x + 0.5f, y + 0.5f, 0.f, 0.f};
            
                    float det01p = det2D(v1 - v0, p - v0);
                    float det12p = det2D(v2 - v1, p - v1);
                    float det20p = det2D(v0 - v2, p - v2);


                    if (det01p >= 0.f && det12p >= 0.f && det20p >= 0.f){
                        float det0 = det2D(v2 - v1, v0 - v1);
                        float det1 = det2D(v0 - v2, v1 - v2);
                        float det2 = det2D(v1 - v0, v2 - v0);

                        float l0 = det12p / det0;
                        float l1 = det20p / det1;
                        float l2 = det01p / det2;
                        color_buffer.at(x, y) = to_color4ub(l0 * c0 + l1 * c1 + l2 * c2);
                    }
                    else if (det01p <= 0.f && det12p <= 0.f && det20p <= 0.f){
                        float det0 = det2D(v2 - v1, v0 - v1);
                        float det1 = det2D(v0 - v2, v1 - v2);
                        float det2 = det2D(v1 - v0, v2 - v0);

                        float l0 = det12p / det0;
                        float l1 = det20p / det1;
                        float l2 = det01p / det2;
                        color_buffer.at(x, y) = to_color4ub(l0 * c0 + l1 * c1 + l2 * c2);
                    }

                }
            }
        }
    }
}