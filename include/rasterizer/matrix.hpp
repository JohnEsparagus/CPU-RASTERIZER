#pragma once
#include <cmath>
#include <rasterizer/vector.hpp>
namespace rasterizer
{
  struct matrix4x4f
{
    float values[16];

    static matrix4x4f identity()
    {
      return matrix4x4f{
        1.f,0.f,0.f,0.f,
        0.f,1.f,0.f,0.f,
        0.f,0.f,1.f,0.f,
        0.f,0.f,0.f,1.f,
      };
    }

    static matrix4x4f perspective(float near,float far, float fovY, float aspect_ratio)
    {
      float top = near * tanf(fovY/2.f);
      float right = top * aspect_ratio;

      return matrix4x4f{
        near/right, 0.f, 0.f, 0.f,
        0.f, near / top, 0.f, 0.f,
        0.f, 0.f, -(far+near) / (far - near), -2.f * far * near/ (far-near),
        0.f, 0.f, -1.f, 0.f
      };
    }

    static matrix4x4f translate(vector3f const & s)
    {

      return matrix4x4f{
        1.f,0.f,0.f,s.x,
        0.f,1.f,0.f,s.y,
        0.f,0.f,1.f,s.z,
        0.f,0.f,0.f,1.f,
      };
    }

    static matrix4x4f scale(rasterizer::vector3f const & s)
    {

      return matrix4x4f{
        s.x,0.f,0.f,0.f,
        0.f,s.y,0.f,0.f,
        0.f,0.f,s.z,0.f,
        0.f,0.f,0.f,1.f,
      };

    }
    static matrix4x4f scale(float s)
    {
      return scale(vector3f{s,s,s}); 
    }
    static matrix4x4f rotateXY(float angle)
    {
      float cos = cosf(angle);
      float sin = sinf(angle);
      return matrix4x4f
      {
	cos, -sin, 0.f, 0.f,
	sin, cos, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f,
      };
    }
    static matrix4x4f rotateYZ(float angle)
    {
      float cos = cosf(angle);
      float sin = sinf(angle);

      return matrix4x4f{
        1.f,0.f,0.f,0.f,
        0.f,cos,-sin,0.f,
        0.f,sin,cos,0.f,
        0.f,0.f,0.f,1.f,
      };

    }
    static matrix4x4f rotateZX(float angle)
    {
    
      float cos = cosf(angle);
      float sin = sinf(angle);

      return matrix4x4f{
        cos,0.f,sin,0.f,
        0.f,1.f,0.f,0.f,
        -sin,0.f,cos,0.f,
        0.f,0.f,0.f,1.f,
      };
    }
};
inline matrix4x4f operator * (matrix4x4f const & m1, matrix4x4f const & m2)
{
    matrix4x4f result
    {
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
    };

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k)
                result.values[4 * i + j] += m1.values[4 * i + k] * m2.values[4 * k + j];

    return result;
}
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
