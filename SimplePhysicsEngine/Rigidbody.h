#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimplePhysicsEngine
{
    class RigidBody
    {
    public:
        float mass;
        float gravity;
        glm::vec3 velocity;
        glm::vec3 angularVelocity;
        glm::vec3 forces;
        bool isKinematic;

        float staticFriction;
        float dynamicFriction;
        float bounciness;

        RigidBody(float mass = 10.0f, float gravity = 1.0f, glm::vec3 velocity = glm::vec3(0), glm::vec3 forces = glm::vec3(0), bool isKinematic = false) :mass(mass), gravity(gravity), velocity(velocity), forces(forces), isKinematic(isKinematic) {
            angularVelocity = glm::vec3(0);
            staticFriction = 0.5f;
            dynamicFriction = 0.5f;
            bounciness = 0.5f;
        }

        RigidBody(const RigidBody& origin){
            mass = origin.mass;
            gravity = origin.gravity;
            velocity = origin.velocity;
            forces = origin.forces;
            isKinematic = origin.isKinematic;
            staticFriction = origin.staticFriction;
            dynamicFriction = origin.dynamicFriction;
            bounciness = origin.bounciness;
            angularVelocity = origin.angularVelocity;
        }
    };
}