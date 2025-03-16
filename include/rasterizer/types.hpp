#pragma once // preprocessor directive to avoid including the headers twice
#include <cstdint> // provides fixed-width integer types
#include <algorithm>

namespace rasterizer
{

    struct color4ub
    {
        std::uint8_t r, g, b, a;
    };

    struct vector4f
    {
        float x, y, z, w;
    };
    struct vector3f
    {
        float x, y, z, w;
    };
    inline color4ub to_color4ub(vector4f const & c)
    {
        color4ub result;

        result.r = std::max(0.f, std::min(255.f, c.x * 255.f));
        result.g = std::max(0.f, std::min(255.f, c.y * 255.f));
        result.b = std::max(0.f, std::min(255.f, c.z * 255.f));
        result.a = std::max(0.f, std::min(255.f, c.w * 255.f));

        return result;
    }

    inline vector4f as_vector(vector3f const & v)
    {
        return {v.x, v.y, v.z, 0.f};
    }

    inline vector4f as_point(vector3f const & v)
    {
        return {v.x, v.y, v.z, 1.f};
    }
    
    inline vector4f operator-(const vector4f& a, const vector4f& b) { // this wasn't in the original code
        return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    }
    inline vector4f operator+(const vector4f& a, const vector4f& b) { // this wasn't in the original code
        return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    }
    inline vector4f operator*(const float& a, const vector4f& b) { // this wasn't in the original code
        return {a*b.x, a*b.y, a*b.z, a*b.w};
    }
    inline float det2D(vector4f const & v0, vector4f const & v1)
    {
        return v0.x * v1.y - v0.y * v1.x;
    }


    struct matrix4x4f
    {
        float values[16];
    
        static matrix4x4f identity()
        {
            return matrix4x4f{
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f,
            };
        }
    };
    
    inline vector4f operator * (matrix4x4f const & m, vector4f const & v)
    {
        vector4f result{0.f, 0.f, 0.f, 0.f};
    
        result.x = m.values[ 0] * v.x + m.values[ 1] * v.y + m.values[ 2] * v.z + m.values[ 3] * v.w;
        result.y = m.values[ 4] * v.x + m.values[ 5] * v.y + m.values[ 6] * v.z + m.values[ 7] * v.w;
        result.z = m.values[ 8] * v.x + m.values[ 9] * v.y + m.values[10] * v.z + m.values[11] * v.w;
        result.w = m.values[12] * v.x + m.values[13] * v.y + m.values[14] * v.z + m.values[15] * v.w;
    
        return result;
    }
}