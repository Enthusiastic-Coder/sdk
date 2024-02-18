#ifndef OBJECTLITES_H
#define OBJECTLITES_H

#include "MeshData.h"

class ObjectLites
{
protected:
    void addQuadLite(meshData& md, float x, float y, float z, float r, float g, float b, float radius=1.0f, float zFactor=1.0f);

    const int _idxStride = 6*3;
};

#endif // OBJECTLITES_H
