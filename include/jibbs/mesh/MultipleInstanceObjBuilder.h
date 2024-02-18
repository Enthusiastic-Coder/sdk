#ifndef MULTIPLEINSTANCEOBJBUILDER_H
#define MULTIPLEINSTANCEOBJBUILDER_H

#include "ObjLoader.h"
#include <jibbs/vector/vector3.h>
#include <QString>

class ObjLoader;

class MultipleInstanceObjBuilder
{
public:
    MultipleInstanceObjBuilder(ObjLoader* objLoader);

    void clear();
    void add(const Vector3F& offset, float scale);
    void edit(int index, const Vector3F& offset, float scale);
    bool empty() const;

    void reserve(int count);

    std::vector<ObjLoader::Vertex> getVertices() const;
    std::vector<unsigned short> getIndices() const;

    int getSizeOfVertexElement() const
    {
        Q_ASSERT(!_vertices.empty());
        return sizeof(_vertices[0]);
    }

    int getBaseVertexSize() const;
    int getBaseIndexSize() const;

private:
    ObjLoader* _obj = nullptr;
    std::vector<ObjLoader::Vertex> _vertices;
    std::vector<unsigned short> _indices;
};

#endif // MULTIPLEINSTANCEOBJBUILDER_H
