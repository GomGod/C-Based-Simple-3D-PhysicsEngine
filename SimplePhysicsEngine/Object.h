#pragma once
#ifndef SIMPLEPHYSICSENGINE_OBJECT_H
#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Vector3.h"
#include "Shader.h"
#include "MeshCollider.h"

#include "glad/glad.h"		
#include "GLFW/glfw3.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace SimplePhysicsEngine
{
    struct AABB
    {
        float minX, maxX;
        float minY, maxY;
        float minZ, maxZ;

        bool TestAABBCollision(const utils::Vector3& point)
        {
            return (minX < point.x && point.x < maxX) &&
                (minY < point.y && point.y < maxY) &&
                (minZ < point.z && point.z < maxZ);
        }

        bool TestAABBCollision(const AABB& box)
        {   
            if (maxX < box.minX || minX > box.maxX) return false;
            if (maxY < box.minY || minY > box.maxY) return false;
            if (maxZ < box.minZ || minZ > box.maxZ) return false;
            return true;        
        }

        AABB& operator+ (const utils::Vector3& pos)
        {
            return AABB{ minX + pos.x, maxX + pos.x, minY + pos.y, maxY + pos.y, minZ + pos.z, maxZ + pos.z };
        }

        AABB& operator& (const AABB& origin)
        {
            return AABB{ origin.minX , origin.maxX , origin.minY , origin.maxY , origin.minZ, origin.maxZ };
        }
    };

    struct Transform
    {
        utils::Vector3 position;
        utils::Vector3 rotation;
    
        Transform(utils::Vector3 position = utils::Vector3{ 0, 0, 0 }, utils::Vector3 rotation = utils::Vector3{ 0, 0, 0 })
        {
            this->position = position;
            this->rotation = rotation;
        }

        Transform(const Transform& origin)
        {
            position = origin.position;
            rotation = origin.rotation;
        }        
    };

    struct Material
    {
        utils::Vector3 ambient;
        utils::Vector3 diffuse;
        utils::Vector3 specular;
        utils::Vector3 color;
        Shader* shader;

        Material(Shader* shader, utils::Vector3 color = utils::Vector3{ 1.0f, 1.0f, 1.0f }, utils::Vector3 ambient = utils::Vector3{ 0.5f, 0.5f, 0.5f }, utils::Vector3 diffuse = utils::Vector3{ 0.5f, 0.5f, 0.5f }, utils::Vector3 specular = utils::Vector3{ 0.5f, 0.5f, 0.5f }) :shader(shader), color(color), ambient(ambient), diffuse(diffuse), specular(specular)
        {}        
    };

    struct RigidBody
    {
        float mass;
        float gravity;
        utils::Vector3 velocity;
        utils::Vector3 forces;
        bool isKinematic;

        float staticFriction;
        float dynamicFriction;
        float bounciness;

        RigidBody(float mass = 10.0f, float gravity = 1.0f, utils::Vector3 velocity = utils::Vector3{0,0,0}, utils::Vector3 forces= utils::Vector3{ 0,0,0 }, bool isKinematic=false) :mass(mass), gravity(gravity), velocity(velocity), forces(forces), isKinematic(isKinematic)
        {
            staticFriction = 0.5f;
            dynamicFriction = 0.5f;
            bounciness = 0.5f;
        }

        RigidBody(const RigidBody& origin)
        {
            mass = origin.mass;
            gravity = origin.gravity;
            velocity = origin.velocity;
            forces = origin.forces;
            isKinematic = origin.isKinematic;
            staticFriction = origin.staticFriction;
            dynamicFriction = origin.dynamicFriction;
            bounciness = origin.bounciness;
        }
    };

    class Object {
    public:
        //For Manage Object
        int ID;

        //Vars for physics        
        RigidBody* rigidBody;
        MeshCollider* collider;
        AABB* aabb;

        //Vars for render
        Material* material;

               
        Object(Transform* transform, RigidBody* rigidBody, Material* material) : transform(transform), rigidBody(rigidBody), material(material)
        {
            collider = new MeshCollider;            
            aabb = new AABB;
            VAO = 0;
            VBO = 0;
            EBO = 0;
            ID = -1;
        }

        ~Object()
        {
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
            delete transform;
            delete rigidBody;
            delete material;
            delete aabb;
        }

        void Object::UpdateTranform(utils::Vector3 position, utils::Vector3 rotation);
        void Object::UpdatePhysics(utils::Vector3 velocity, utils::Vector3 forces);

        //Rendering for OpenGL   
        void SetShader(Shader* shader) { material->shader = shader; }
        const Shader* GetShader() const { return material->shader; }
        void ActivateShader() { material->shader->use(); }

        const float* GetVertices() const { return vertices.data(); }
        const float* GetNormals() const { return normals.data(); }
        const unsigned int* GetIndicies() const { return indices.data(); }

        const int GetVertexSize() const { return vertices.size() * sizeof(float); }
        const int GetNormalSize() const { return normals.size() * sizeof(float); }
        const int GetIndexSize() const { return indices.size() * sizeof(float); }

        unsigned int GetVertexCount() const { return (unsigned int)vertices.size(); }
        unsigned int GetNormalCount() const { return (unsigned int)normals.size(); }
        unsigned int GetIndexCount() const { return (unsigned int)indices.size(); }

        const float* GetInterleavedVertexAttrib() const { return interleavedVertexAttrib.data(); }
        const int GetInterleavedVertexAttribSize() { return interleavedVertexAttrib.size() * sizeof(float); }
        unsigned int GetInterleavedVertexAttribCount() const { return (unsigned int)interleavedVertexAttrib.size(); }
        unsigned int GetVAO() { return VAO; }

        void Draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

        Transform GetTransform() const { return *transform; }
        Transform SetTransform(Transform tf)
        {
            transform->position = tf.position;
            transform->rotation = tf.rotation;
            UpdateAABB();
        }
        void UpdatePosition(utils::Vector3 pos)
        {
            transform->position = pos;
        }
        void UpdateRotation(utils::Vector3 rot)
        {
            transform->rotation = rot;
            UpdateAABB();
        }


    protected:
        Transform* transform;
        //For Physics
        void UpdateAABB();

        //Rendering for OpenGL    
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned int> indices;
        std::vector<float> interleavedVertexAttrib;
        
        void BuildInterleavedVertices();
        void ClearArrays();
        void AddVertex(float x, float y, float z);
        void AddNormal(float x, float y, float z);
        void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> GetFaceNormal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3);

        void BuildVAO();
        unsigned int VAO, VBO, EBO;        
    };
};

#endif //SIMPLEPHYSICSENGINE_OBJECT_H