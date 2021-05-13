#include <stdlib.h>
#include <iostream>
class Object
{
    struct Vertex
    {
        float x, y, z;
    };
    struct Vertex *vertices;
    float *colors;

public:
    void setVertex(const char *f);
};