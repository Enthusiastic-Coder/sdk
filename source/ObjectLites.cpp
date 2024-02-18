#include "ObjectLites.h"

void ObjectLites::addQuadLite(meshData &md, float x, float y, float z, float r, float g, float b, float radius, float zFactor)
{
    md.addColor(r, g, b, 1.0f);
    md.addVertex(x-radius, y, (z+radius)/zFactor);

    md.addColor(r, g, b, 1.0f);
    md.addVertex(x+radius, y, (z+radius)/zFactor);

    md.addColor(r, g, b, 1.0f);
    md.addVertex(x-radius, y, (z-radius)/zFactor);

    //////////////////////////////


    md.addColor(r, g, b, 1.0f);
    md.addVertex(x+radius, y, (z+radius)/zFactor);

    md.addColor(r, g, b, 1.0f);
    md.addVertex(x+radius, y, (z-radius)/zFactor);

    md.addColor(r, g, b, 1.0f);
    md.addVertex(x-radius, y, (z-radius)/zFactor);


}
