#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
//#include "Object.h"
#include "Plataform.h"
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

Plataform::Plataform(const char *f)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    this->setVertexes(f, v);
    this->setTexture(0.2, 0.2, 0.2);
    this->setBodyCollider();
}
void Plataform::setBodyCollider()
{
    std::vector<struct Vertex> v(vertex, vertex + n_vertexes);
    float dis = *next(X.begin(), 1) - *next(X.begin(), 0);
    // cout << dis<< " "<<dis2 << std::endl;
    for (int i = 0; i < v.size(); i++)
    {
        struct Vertex vno = *next(v.begin(), i), vne = *next(v.begin(), i), vse = *next(v.begin(), i), vso = *next(v.begin(), i);
        vno.z = vno.z + dis;
        vno.x = vno.x + dis;
        vne.z = vne.z + dis;
        vne.x = vne.x - dis;
        vso.z = vso.z - dis;
        vso.x = vso.x + dis;
        vse.z = vse.z - dis;
        vse.x = vse.x - dis;
        //cout<<v.at(i).x<<" "<<v.at(i).y<<" "<<v.at(i).z<<std::endl;
        //cout<<(find(v.begin(), v.end(), vno)==v.end())<<" "<<(find(v.begin(), v.end(), vso)==v.end())<<" "<< (find(v.begin(), v.end(), vne)==v.end())<<" "<< (find(v.begin(), v.end(), vse)==v.end())<<std::endl;
        if (find(v.begin(), v.end(), vno) != v.end() || find(v.begin(), v.end(), vne) != v.end() || find(v.begin(), v.end(), vso) != v.end() || find(v.begin(), v.end(), vse) != v.end())
        {
            if (find(bodyCollider.begin(), bodyCollider.end(),v.at(i)) == bodyCollider.end())
            {
                cout << v.at(i).x << " " << v.at(i).y << " " << v.at(i).z << std::endl;
                bodyCollider.push_back(v.at(i));
            }
        }
    }
    cout << bodyCollider.size() << std::endl;
}
