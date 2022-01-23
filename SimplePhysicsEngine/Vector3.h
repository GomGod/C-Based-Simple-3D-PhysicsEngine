#pragma once

#ifndef SIMPLEPHYSICSENGINE_VECTOR3_H
#define SIMPLEPHYSICSENGINE_VECTOR3_H

#include <cmath>
#include <iostream>

namespace utils
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3(const Vector3& origin)
        {
            this->x = origin.x;
            this->y = origin.y;
            this->z = origin.z;
        }

        Vector3(float xc = 0, float yc = 0, float zc = 0);
        Vector3 operator + (Vector3 v) const;
        Vector3 operator - () const;
        Vector3 operator - (Vector3 v) const;                
        Vector3 operator *(float i) const;
        Vector3 operator /(float i) const;
        Vector3& operator +=(const Vector3& v);
        Vector3& operator -=(const Vector3& v);
        Vector3& operator *=(float i);
        Vector3& operator /=(float i);
        
        
        float GetVectorSize() const;
        float GetMagnitude() const;

        static Vector3 Lerp(Vector3 depart, Vector3 dest, float rate);
        static float GetDistance(Vector3 v1, Vector3 v2);
        static Vector3 Normalize(Vector3& v);
        static float DotProduct(const Vector3& v1, const Vector3& v2);
        static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

        friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
    };
}
#endif //SIMPLEPHYSICSENGINE_VECTOR3_H
