#pragma once
#include <cstdint>
#include <algorithm>
#include <cmath>
namespace rasterizer
{
  struct vector4f
  {
    float x,y,z,w;
  };

  struct vector3f
  {
    float  x,y,z;
  };

  inline vector4f perspective_divide(vector4f a)
  {
    a.x = a.x / a.w;
    a.y = a.y / a.w;
    a.z = a.z / a.w;
    a.w = 1.f / a.w;
    return a;
  }

  inline float dot(vector4f const & v0, vector4f const & v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
  }
  inline float dot(vector3f const & v0, vector3f const & v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  inline vector3f cross_product(const vector3f& a,  vector3f const& b){
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
  }
  inline vector3f operator - (vector3f const& v0, vector3f const& v1)
  {
    return {v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};
  }
  inline vector3f operator+(const vector3f& a , const vector3f& b)
  {
    return {a.x + b.x, a.y+b.y, a.z + b.z};
  }
  inline vector3f normalize(const vector3f &a){
    float length = sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    if (length == 0.f) return vector3f(0.f,0.f,0.f);
    return vector3f{a.x / length, a.y / length, a.z / length};
  }
  inline vector3f operator*(float scalar, const vector3f& v)
  {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
  }

  inline vector3f operator*(const vector3f& v, float scalar)
  {
    return scalar * v;
  }

  inline vector4f operator*(float scalar, const vector4f& v)
  {
    return {v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
  }

  inline vector4f operator*(const vector4f& v, float scalar)
  {
    return scalar * v;
  }

  inline vector4f operator+(const vector4f& a , const vector4f& b)
  {
    return {a.x + b.x, a.y+b.y, a.z + b.z, a.w + b.w};
  }

  inline vector4f as_vector(vector3f const& v)
  {
    return {v.x, v.y, v.z, 0.f};
  }

  inline vector4f as_point(vector3f const& v)
  {
    return {v.x,v.y,v.z,1.f};
  }
  inline vector4f operator - (vector4f const& v0, vector4f const& v1)
  {
    return {v0.x - v1.x, v0.y - v1.y, v0.z - v1.z, v0.w- v1.w};
  }

  inline float det2D(vector4f const& v0, vector4f const& v1)
  {
    return v0.x * v1.y - v0.y*v1.x;
  }
}

