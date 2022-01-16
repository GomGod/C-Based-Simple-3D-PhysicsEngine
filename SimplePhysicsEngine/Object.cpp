#include "Object.h"

namespace SimplePhysicsEngine
{   
    void Object::UpdateTranform(utils::Vector3 position, utils::Vector3 rotation)
    {
        transform->position = position;
        transform->rotation = rotation;
    }

    void Object::UpdatePhysics(utils::Vector3 velocity, utils::Vector3 forces)
    {        
        rigidBody->velocity = velocity;
        rigidBody->forces = forces;
    }

    void Object::buildInterleavedVertices()
    {
        std::vector<float>().swap(interleavedVertexAttrib);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            interleavedVertexAttrib.push_back(vertices[i]);
            interleavedVertexAttrib.push_back(vertices[i + 1]);
            interleavedVertexAttrib.push_back(vertices[i + 2]);

            interleavedVertexAttrib.push_back(normals[i]);
            interleavedVertexAttrib.push_back(normals[i + 1]);
            interleavedVertexAttrib.push_back(normals[i + 2]);
        }

        buildVAO();
    }

    void Object::clearArrays()
    {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<unsigned int>().swap(indices);
    }

    void Object::addVertex(float x, float y, float z)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    void Object::addNormal(float x, float y, float z)
    {
        normals.push_back(x);
        normals.push_back(y);
        normals.push_back(z);
    }

    void Object::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
    {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    std::vector<float> Object::getFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }
    void Object::buildVAO()
    {        
        glGenVertexArrays(1, &VAO); //Gen VAO
        glBindVertexArray(VAO); // Bind VAO

        //Set Vertex Data
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, getInterleavedVertexAttribSize(), getInterleavedVertexAttrib(), GL_STATIC_DRAW);

        //Set Draw Indicies
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndicies(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        //Enable Vertex Attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO        
    }

    void Object::draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection)
    {
        activateShader();
        glBindVertexArray(VAO);
        auto shader = getShader();

        shader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setVec3("material.ambient", material->ambient.x, material->ambient.y, material->ambient.z);
        shader->setVec3("material.diffuse", material->diffuse.x, material->diffuse.y, material->diffuse.z);
        shader->setVec3("material.specular", material->specular.x, material->specular.y, material->specular.z);
        shader->setVec3("material.color", material->color.x, material->color.y, material->color.z);
        glm::mat4 model = glm::mat4(1.0);        

        model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, transform->position.z));
       // model = glm::rotate(model, glm::radians(1.0f), glm::vec3(rotation.x, rotation.y, rotation.z));
        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}