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

        explicit Vector3(float xc = 0, float yc = 0, float zc = 0);
        Vector3 operator + (Vector3& v) const;
        Vector3 operator - (Vector3& v) const;
        Vector3 operator *(const Vector3& v) const; //Inner Product
        Vector3 operator *(float i) const;
        Vector3 operator /(float i) const;
        Vector3& operator +=(const Vector3& v);
        Vector3& operator -=(const Vector3& v);
        float GetVectorSize() const;
        static float GetDistance(Vector3 v1, Vector3 v2);
        static Vector3 Normalize(Vector3& v);
        static Vector3 ExternalProduct(const Vector3& v1, const Vector3& v2); //External Product
        friend std::ostream& operator<<(std::ostream& os, const Vector3& v);
    };
}
#endif //SIMPLEPHYSICSENGINE_VECTOR3_H
