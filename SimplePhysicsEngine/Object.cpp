#include "Object.h"

namespace utils {
    Object::Object(Vector3 pos, Vector3 rot, Vector3 velocity, float mass) : Position{ pos }, Rotation{ rot }, Velocity{ velocity },
        Forces{ Vector3{0,0,0} }, Mass{ mass }{}
}
