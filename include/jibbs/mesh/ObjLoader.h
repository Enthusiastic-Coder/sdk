#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <QString>

class ObjLoader
{
public:
    ObjLoader();

    struct Vertex {
        struct Position {
            float x, y, z;
        } position;

        struct Normal {
            float x, y, z;
        } normal;

        struct TextureCoord {
            float u, v;
        } textureCoord;

        bool operator==(const Vertex& other) const {
            return position.x == other.position.x &&
                   position.y == other.position.y &&
                   position.z == other.position.z &&
                   normal.x == other.normal.x &&
                   normal.y == other.normal.y &&
                   normal.z == other.normal.z &&
                   textureCoord.u == other.textureCoord.u &&
                   textureCoord.v == other.textureCoord.v;
        }
    };

    bool load(QString filename);

    int getMaxVertices() const;

    std::vector<Vertex>  getVertices() const;
    std::vector<unsigned short> getIndices() const;

private:
    int _maxVertices = 0;
    std::vector<Vertex> _vertices;
    std::vector<unsigned short> _indices;

};

#endif // OBJLOADER_H
