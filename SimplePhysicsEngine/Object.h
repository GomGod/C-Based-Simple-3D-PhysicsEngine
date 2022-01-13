#ifndef SIMPLEPHYSICSENGINE_OBJECT_H
#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Vector3.h"
#include "Shader.h"

#include "glad/glad.h"		
#include "GLFW/glfw3.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace SimplePhysicsEngine
{
    class Object {
    public:
        //For Manage Object
        int ID;

        //Vars for physics
        utils::Vector3 position;
        utils::Vector3 rotation;
        utils::Vector3 velocity;
        utils::Vector3 forces;
        utils::Vector3 color;
        float mass;
               
        Object(utils::Vector3 position = utils::Vector3{0,0,0}, utils::Vector3 rotation = utils::Vector3{ 0,0,0 }, utils::Vector3 velocity = utils::Vector3{ 0,0,0 }, utils::Vector3 forces = utils::Vector3{ 0,0,0 }, float mass = 5.0f, bool usePhysics = true, Shader* shader=0) : position(position), rotation(rotation), velocity(velocity), mass(mass), usePhysics(usePhysics), shader(shader)
        {
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
        }
                
        void PhysicsUpdate(utils::Vector3 position, utils::Vector3 rotation, utils::Vector3 velocity, utils::Vector3 forces);

        //Physics
        bool usePhysics;

        //Rendering for OpenGL   
        void setShader(Shader* shader) { this->shader = shader; }
        Shader* getShader() { return shader; }
        void activateShader() { shader->use(); }

        const float* getVertices() const { return vertices.data(); }
        const float* getNormals() const { return normals.data(); }
        const unsigned int* getIndicies() const { return indices.data(); }

        const int getVertexSize() const { return vertices.size() * sizeof(float); }
        const int getNormalSize() const { return normals.size() * sizeof(float); }
        const int getIndexSize() const { return indices.size() * sizeof(float); }

        unsigned int getVertexCount() const { return (unsigned int)vertices.size(); }
        unsigned int getNormalCount() const { return (unsigned int)normals.size(); }
        unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

        const float* getInterleavedVertexAttrib() const { return interleavedVertexAttrib.data(); }
        const int getInterleavedVertexAttribSize() { return interleavedVertexAttrib.size() * sizeof(float); }
        unsigned int getInterleavedVertexAttribCount() const { return (unsigned int)interleavedVertexAttrib.size(); }

        unsigned int getVAO() { return VAO; }

        void draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

    protected:
        //Rendering for OpenGL    
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned int> indices;
        std::vector<float> interleavedVertexAttrib;
        Shader* shader;

        void buildInterleavedVertices();
        void clearArrays();
        void addVertex(float x, float y, float z);
        void addNormal(float x, float y, float z);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> getFaceNormal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3);

        void buildVAO();
        unsigned int VAO, VBO, EBO;
    };
}

#endif //SIMPLEPHYSICSENGINE_OBJECT_H