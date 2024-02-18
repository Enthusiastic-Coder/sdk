#ifndef MESHDATA_H
#define MESHDATA_H
#include <jibbs/vector/vector3.h>
#include <QString>

struct vertexData{
    Vector3F position;
    Vector3F normal;
    Vector3F tangent;
    Vector3F color;
	float U,V;
};

struct textureData{
    unsigned int id;
};

#endif
