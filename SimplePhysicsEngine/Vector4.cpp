#include "Vector4.h"

namespace utils
{
    Vector4::Vector4(const Vector4& origin)
    {
        this->x = origin.x;
        this->y = origin.y;
        this->z = origin.z;
        this->w = origin.w;
    }

    Vector4 Vector4::operator+(Vector4 v) const
    {
        return Vector4(x+v.x, y+v.y, z+v.z, w + v.w);
    }

    Vector4 Vector4::operator-() const
    {
        return *this * -1;
    }

    Vector4 Vector4::operator-(Vector4 v) const
    {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vector4 Vector4::operator*(float i) const
    {
        return Vector4(x*i,y*i,z*i,w*i);
    }

    Vector4 Vector4::operator/(float i) const
    {
        return Vector4(x/i, y/i, z/i, w/i);
    }

    Vector4& Vector4::operator+=(const Vector4& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.z;
        return *this;
    }

    Vector4& Vector4::operator-=(const Vector4& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.z;
        return *this;
    }

    Vector4& Vector4::operator*=(float i)
    {
        x *= i;
        y *= i;
        z *= i;
        w *= i;
        return *this;
    }

    Vector4& Vector4::operator/=(float i)
    {
        x /= i;
        y /= i;
        z /= i;
        w /= i;
        return *this;
    }

}

