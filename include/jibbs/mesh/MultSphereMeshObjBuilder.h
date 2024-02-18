#ifndef MULTSPHEREMESHOBJBUILDER_H
#define MULTSPHEREMESHOBJBUILDER_H

#include <map>
#include <vector>
#include <string>
#include <jibbs/vector/vector4.h>
#include <QString>

class MultSphereMeshObjBuilder
{
public:
    MultSphereMeshObjBuilder();

    void clear();
    void add(float x, float y, float z, float Scale, float r, float g, float b);
    std::pair<std::string, std::string> exportToObj(QString mtlLib) const;

private:
    std::map<int, std::vector<Vector4F>> _spheres;
};

#endif // MULTSPHEREMESHOBJBUILDER_H
