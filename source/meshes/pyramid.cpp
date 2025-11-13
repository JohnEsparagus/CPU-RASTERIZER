#include <rasterizer/mesh.hpp>
#include <rasterizer/pyramid.hpp>
namespace rasterizer
{

  static vector3f pyramid_positions[] =
  {
    // Base (Square face)
    {-1.f, -1.f,  0.f}, // 0
     { 1.f, -1.f,  0.f}, // 1
     { 1.f,  1.f,  0.f}, // 2
     {-1.f,  1.f,  0.f}, // 3

    // Apex
     { 0.f,  0.f,  2.f}, // 4
  };

  static vector4f pyramid_colors[] =
  {
    // Base (Square face)
    {1.f, 1.f, 0.f, 1.f}, // Yellow
    {1.f, 1.f, 0.f, 1.f}, // Yellow
    {1.f, 1.f, 0.f, 1.f}, // Yellow
    {1.f, 1.f, 0.f, 1.f}, // Yellow

    // Sides (Triangle faces)
    {1.f, 0.f, 0.f, 1.f}, // Red
    {0.f, 1.f, 0.f, 1.f}, // Green
    {0.f, 0.f, 1.f, 1.f}, // Blue
    {1.f, 0.f, 1.f, 1.f}, // Magenta
  };

  static std::uint32_t pyramid_indices[] =
  {
    // Base (Square face)
    0,  1,  2,
    2,  3,  0,

    // Side Triangles
    0,  1,  4, // Triangle 1
    1,  2,  4, // Triangle 2
    2,  3,  4, // Triangle 3
    3,  0,  4, // Triangle 4
  };

  const mesh pyramid
  {
    .positions = {pyramid_positions},
    .color = {pyramid_colors},
    .count = 18,
    .indices = pyramid_indices
  };

}

