#ifndef OBJSCOMPS_H
#define OBJSCOMPS_H
#include <iostream>
struct Vertex
{
    float x, y, z;
    bool operator==(struct Vertex const &b) const
    {
        //cout<<b.x<< " "<<this->x<< std::endl;
        return this->x == b.x && this->y == b.y && this->z == b.z;
    }
    inline bool operator<(const Vertex &a) const
    {
        if (x < a.x)
        {
            return true;
        }
        if (x == a.x && y < a.y)
        {
            return true;
        }
        if (x == a.x && y == a.y && z < a.z)
            return true;
        return false;
    }
    string tostring()
    {
        return to_string(x) + " " + to_string(y) + " " + to_string(z);
    }
    Vertex(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vertex()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
};
struct Faces
{
    struct Vertex v[3];
    struct Vertex c;
    struct Vertex vt[3];
    string tostring()
    {
        return v[0].tostring() + " " + v[1].tostring() + " " + v[2].tostring() + " ";
    }
    bool equals(struct Faces b)
    {
        return v[0].operator==(b.v[0]) && v[1].operator==(b.v[1]) && v[2].operator==(b.v[2]);
    }
};

struct Texture{
    float x,y;
    Texture(float x,float y){
        this->x=x;
        this->y=y;
    }
    Texture(){x=0;y=0;}
};
#endif