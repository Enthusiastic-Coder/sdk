#include "MultipleInstanceObjBuilder.h"

MultipleInstanceObjBuilder::MultipleInstanceObjBuilder(ObjLoader *objLoader):
    _obj(objLoader)
{}

void MultipleInstanceObjBuilder::clear()
{
    _vertices.clear();
    _indices.clear();
}

void MultipleInstanceObjBuilder::add(const Vector3F &offset, float scale)
{
    Q_ASSERT(_obj != nullptr);

    const auto& vertices = _obj->getVertices();

    Q_ASSERT(!vertices.empty());

    if(vertices.empty())
        return;

    if( _vertices.size() >= _obj->getMaxVertices())
        return;

    for( auto& vertex : vertices)
    {
        _vertices.push_back(vertex);

        auto& current = _vertices.back();

        current.position.x *= scale;
        current.position.y *= scale;
        current.position.z *= scale;

        current.position.x += offset.x;
        current.position.y += offset.y;
        current.position.z += offset.z;
    }

    const auto& indices = _obj->getIndices();

    const int insertIndex = static_cast<int>(_vertices.size() / vertices.size()) -1;

    for(auto &index : indices)
    {
        _indices.push_back(index);

        auto& current = _indices.back();

        current += vertices.size() * insertIndex;
    }
}

void MultipleInstanceObjBuilder::edit(int index, const Vector3F &offset, float scale)
{
    int offsetIndex = _obj->getVertices().size() * index;

    for(int i=0; i < _obj->getVertices().size(); ++i)
    {
        auto& current = _vertices[offsetIndex+i];

        current = _obj->getVertices()[i];

        current.position.x *= scale;
        current.position.y *= scale;
        current.position.z *= scale;

        current.position.x += offset.x;
        current.position.y += offset.y;
        current.position.z += offset.z;
    }
}

bool MultipleInstanceObjBuilder::empty() const
{
    return _vertices.empty();
}

void MultipleInstanceObjBuilder::reserve(int count)
{
    _vertices.reserve(count*_obj->getVertices().size());
    _indices.reserve(count*_obj->getIndices().size());
}

std::vector<ObjLoader::Vertex> MultipleInstanceObjBuilder::getVertices() const
{
    return _vertices;
}

std::vector<unsigned short> MultipleInstanceObjBuilder::getIndices() const
{
    return _indices;
}

int MultipleInstanceObjBuilder::getBaseVertexSize() const
{
    return _obj->getVertices().size();
}

int MultipleInstanceObjBuilder::getBaseIndexSize() const
{
    return _obj->getIndices().size();
}
