#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;
struct Vertex
{
    float x, y, z;
};
class Object
{

public:
    void setVertexes(const char *f);
    struct Vertex *vertices;
    int n_vertexes;
    float *colors;

private:
    void getfaces(string s,string del);
    struct Vertex getvertex(string s,string del);
};