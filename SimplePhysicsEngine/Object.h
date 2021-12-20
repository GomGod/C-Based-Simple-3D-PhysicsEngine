#ifndef SIMPLEPHYSICSENGINE_OBJECT_H
#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Vector3.h"

namespace utils {
    class Object {
    public:
        utils::Vector3 Position;
        utils::Vector3 Rotation;
        utils::Vector3 Velocity;
        utils::Vector3 Forces;
        float Mass;

        Object(Vector3 pos, Vector3 rot, Vector3 velocity, float mass);
    };
}

#endif //SIMPLEPHYSICSENGINE_OBJECT_H
