#ifndef SIMPLEPHYSICSENGINE_OBJECT_H

#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Vector3.h"
#include <vector>

class Object {
public:
    //Vars for physics
    utils::Vector3 position;
    utils::Vector3 rotation;
    utils::Vector3 velocity;
    utils::Vector3 forces;
    utils::Vector3 color;
    float mass;

    Object(utils::Vector3 pos = utils::Vector3{ 0,0,0 }, utils::Vector3 rot = utils::Vector3{ 0,0,0 }, utils::Vector3 vel = utils::Vector3{ 0,0,0 }, float ms = 0) : position(pos), rotation(rot), velocity(vel), mass(ms) {}
    ~Object() {}

    //Rendering for OpenGL
    virtual void draw() const {};   
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


protected:
    //Rendering for OpenGL
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;    
    std::vector<float> interleavedVertexAttrib;

    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> getFaceNormal(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3);
};


#endif //SIMPLEPHYSICSENGINE_OBJECT_H