#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include "Object.h"

void tokenize(string s, string del = " ")
{
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        cout << s.substr(start, end - start) << endl;
        start = end + del.size();
        end = s.find(del, start);
    }
    cout << s.substr(start, end - start) << endl;
}
void Object::setVertex(const char *f)
{
    ifstream fp(f);
    if (!fp.is_open())
    {
        perror("Failed: ");
        cout << f;
        exit(1);
    }
    else
    {
        string line;
        while (getline(fp, line))
        {
            // Output the text from the file
            //cout << line+"\n";
            tokenize(line," ");
        }
        fp.close();
    }
}