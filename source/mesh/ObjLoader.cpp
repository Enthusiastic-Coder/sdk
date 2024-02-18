#include <jibbs/mesh/ObjLoader.h>
#include <vector>
#include <sstream>

#include <QFile>

ObjLoader::ObjLoader() {}

bool ObjLoader::load(QString filename)
{
    _vertices.clear();
    _indices.clear();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    std::vector<Vertex::Position> positions;
    std::vector<Vertex::TextureCoord> textureCoords;
    std::vector<Vertex::Normal> normals;
    std::vector<unsigned short> indexBuffer;

    while (!file.atEnd())
    {
        QString line = file.readLine();
        std::istringstream iss(line.toStdString());
        std::string type;
        iss >> type;

        if (type == "v")
        {
            Vertex::Position position;
            iss >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (type == "vt")
        {
            Vertex::TextureCoord textureCoord;
            iss >> textureCoord.u >> textureCoord.v;
            textureCoords.push_back(textureCoord);
        }
        else if (type == "vn")
        {
            Vertex::Normal normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; ++i)
            {
                std::string vertexInfo;
                iss >> vertexInfo;

                std::istringstream viss(vertexInfo);
                unsigned int vIndex, tIndex, nIndex;
                char separator;

                viss >> vIndex >> separator >> tIndex >> separator >> nIndex;

                Vertex vertex;
                vertex.position = positions[vIndex - 1];
                vertex.textureCoord = textureCoords[tIndex - 1];
                vertex.normal = normals[nIndex - 1];

                auto it = std::find(_vertices.begin(), _vertices.end(), vertex);
                if (it != _vertices.end())
                {
                    indexBuffer.push_back(static_cast<unsigned short>(std::distance(_vertices.begin(), it)));
                }
                else
                {
                    unsigned short newIndex = static_cast<unsigned short>(_vertices.size());
                    _vertices.push_back(vertex);
                    indexBuffer.push_back(newIndex);
                }
            }
        }
    }

    _indices.insert(_indices.end(), indexBuffer.begin(), indexBuffer.end());

    _maxVertices = std::numeric_limits<unsigned short>::max() - _vertices.size();

    file.close();
    return true;
}

int ObjLoader::getMaxVertices() const
{
    return _maxVertices;
}

std::vector<ObjLoader::Vertex>  ObjLoader::getVertices() const
{
    return _vertices;
}

std::vector<unsigned short> ObjLoader::getIndices() const
{
    return _indices;
}
