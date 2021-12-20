#ifndef SIMPLEPHYSICSENGINE_VECTOR3_H
#define SIMPLEPHYSICSENGINE_VECTOR3_H

#include <cmath>

namespace utils
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

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
    };
}
#endif //SIMPLEPHYSICSENGINE_VECTOR3_H
