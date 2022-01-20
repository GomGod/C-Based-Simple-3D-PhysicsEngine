#pragma once
#include"Vector3.h"

namespace utils
{
	class Vector4 : public Vector3
	{
public:
		float w;

        Vector4(float xc = 0, float yc = 0, float zc = 0, float wc = 0) : Vector3(xc, yc, zc)
        {
            w = wc;
        }
        Vector4(const Vector4& origin);

        Vector4 operator + (Vector4 v) const;
        Vector4 operator - () const;
        Vector4 operator - (Vector4 v) const;
        Vector4 operator *(float i) const;
        Vector4 operator /(float i) const;
        Vector4& operator +=(const Vector4& v);
        Vector4& operator -=(const Vector4& v);
        Vector4& operator *=(float i);
        Vector4& operator /=(float i);

        Vector3 xyz() { return Vector3(x, y, z); }	};
}

