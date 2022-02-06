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

        //Position by forces
        glm::vec3 gravity;
        glm::vec3 netForce; //ÈûÀÇ ÃÑÇÕ (constraints solve results)
        glm::vec3 velocity;

        //Rotation by forces
        glm::vec3 netTorque; //ÅäÅ©
        glm::vec3 angularVelocity;
        glm::mat3 inertia; //°ü¼º

        bool isKinematic;

        //Frictions
        float staticFriction;
        float dynamicFriction;

        //Bounciness(Restitution)
        float bounciness;

        float getMassInverse() { return 1.0f / mass; };

        RigidBody(float mass = 10.0f, float gravity = 1.0f, glm::vec3 velocity = glm::vec3(0), glm::vec3 netForce = glm::vec3(0), bool isKinematic = false) :mass(mass), gravity(gravity), velocity(velocity), netForce(netForce), isKinematic(isKinematic) {
            inertia = glm::mat3(1.0f);
            netTorque = glm::vec3(0);            
            angularVelocity = glm::vec3(0);
            
            staticFriction = 0.5f;
            dynamicFriction = 0.5f;
            bounciness = 0.5f;            
        }

        RigidBody(const RigidBody& origin){
            mass = origin.mass;
            gravity = origin.gravity;
            velocity = origin.velocity;
            netForce = origin.netForce;
            isKinematic = origin.isKinematic;
            staticFriction = origin.staticFriction;
            dynamicFriction = origin.dynamicFriction;
            bounciness = origin.bounciness;
            angularVelocity = origin.angularVelocity;
            inertia = origin.inertia;
            netTorque = origin.netTorque;
        }
    };
}