#include "MultSphereMeshObjBuilder.h"
#include <QOpenGLFunctions>
#include <sstream>

namespace {
static GLfloat VertexData[] = {
    -0.500000, 0.0, 0.0,
    -0.353553, -0.353553, 0.0,
    -0.353553, 0.0, -0.353553,
    -0.353553, 0.0, 0.353553,
    -0.353553, 0.353553, 0.0,
    -0.250000, -0.353553, -0.250000,
    -0.250000, -0.353553, 0.250000,
    -0.250000, 0.353553, -0.250000,
    -0.250000, 0.353553, 0.250000,
    0.0, 0.0, 0.500000,
    0.0, -0.353553, 0.353553,
    0.0, 0.353553, 0.353553,
    0.0, -0.500000, 0.0,
    0.0, 0.500000, 0.0,
    0.0, -0.353553, -0.353553,
    0.0, 0.353553, -0.353553,
    0.0, 0.0, -0.500000,
    0.250000, -0.353553, -0.250000,
    0.250000, -0.353553, 0.250000,
    0.250000, 0.353553, -0.250000,
    0.250000, 0.353553, 0.250000,
    0.353553, -0.353553, 0.0,
    0.353553, 0.0, -0.353553,
    0.353553, 0.0, 0.353553,
    0.353553, 0.353553, 0.0,
    0.500000, 0.0, 0.0,
};
static GLint Indices[] = {
    14, 25, 20,
    25, 26, 23,
    25, 23, 20,
    26, 22, 18,
    26, 18, 23,
    22, 13, 18,
    14, 20, 16,
    20, 23, 17,
    20, 17, 16,
    23, 18, 15,
    23, 15, 17,
    18, 13, 15,
    14, 16, 8,
    16, 17, 3,
    16, 3, 8,
    17, 15, 6,
    17, 6, 3,
    15, 13, 6,
    14, 8, 5,
    8, 3, 1,
    8, 1, 5,
    3, 6, 2,
    3, 2, 1,
    6, 13, 2,
    14, 5, 9,
    5, 1, 4,
    5, 4, 9,
    1, 2, 7,
    1, 7, 4,
    2, 13, 7,
    14, 9, 12,
    9, 4, 10,
    9, 10, 12,
    4, 7, 11,
    4, 11, 10,
    7, 13, 11,
    14, 12, 21,
    12, 10, 24,
    12, 24, 21,
    10, 11, 19,
    10, 19, 24,
    11, 13, 19,
    14, 21, 25,
    21, 24, 26,
    21, 26, 25,
    24, 19, 22,
    24, 22, 26,
    19, 13, 22

};
}


MultSphereMeshObjBuilder::MultSphereMeshObjBuilder()
{

}

void MultSphereMeshObjBuilder::clear()
{
    _spheres.clear();
}

void MultSphereMeshObjBuilder::add(float x, float y, float z, float Scale, float r, float g, float b)
{
    int idx = r*255+ (int(g*255) << 8) + (int(b*255)<<16);
    _spheres[idx].push_back(Vector4F(x,y,z,Scale));
}

std::pair<std::string,std::string> MultSphereMeshObjBuilder::exportToObj(QString mtlLib) const
{
    std::stringstream strMaterial;
    std::stringstream final;

    const int VERTCOUNT = sizeof(VertexData)/sizeof(VertexData[0]);
    const int IDXCOUNT = sizeof(Indices)/sizeof(Indices[0]);
    int count = 0;

    final << "mtllib " << mtlLib.toStdString() << ".mtl" << std::endl;

    for( const auto& sphereMap : _spheres)
    {
        std::stringstream strVertices;
        std::stringstream strIndices;

        for( const auto& sphere : sphereMap.second)
        {
            Vector4F v4;
            for( int v =0; v < VERTCOUNT; v+=3)
            {
                v4.x = VertexData[v] * sphere.w + sphere.x;
                v4.y = VertexData[v+1] * sphere.w + sphere.y;
                v4.z = VertexData[v+2] * sphere.w + sphere.z;
                strVertices << "v " << v4.x << " " << v4.y << " " << v4.z;
                strVertices << std::endl;
            }
        }

        const int sphereCount = sphereMap.second.size();

        for( int i=0;i < sphereCount; ++i)
        {
            for( int idx = 0; idx < IDXCOUNT; idx+=3)
            {
                strIndices << "f "
                           << ( Indices[idx]  + (count*VERTCOUNT/3))
                           << " "
                           << ((Indices[idx+1]) + (count*VERTCOUNT/3))
                        << " "
                        << ((Indices[idx+2]) + (count*VERTCOUNT/3))
                        << std::endl;
            }

            count++;
        }

        strIndices << std::endl;
        final << strVertices.str() << std::endl
              << "usemtl mtl_" << count << std::endl
              << "o obj_" << count << std::endl
              << "g sphere_" << count << std::endl
              << "s off" << std::endl
              << strIndices.str();

        strMaterial << "newmtl mtl_" << count << std::endl;
        strMaterial << "\tNs32" << std::endl;
        strMaterial << "\td 1" << std::endl;
        strMaterial << "\tillum 2" << std::endl;
        int rColor = sphereMap.first&0xFF;
        int gColor = sphereMap.first >> 8 & 0xFF;
        int bColor = sphereMap.first >> 16 & 0xFF;
        strMaterial << "\tKd "
                    << rColor/255.0f << " "
                    << gColor/255.0f << " "
                    << bColor/255.0f << " "
                    << std::endl << std::endl;
    }

    return std::make_pair(final.str(), strMaterial.str());
}
