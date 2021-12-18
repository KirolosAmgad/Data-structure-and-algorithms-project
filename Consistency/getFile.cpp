#include "functions.h"


string getFile(string filename)
{
    string buffer;
    char c;

    ifstream in(filename);
    if (!in)
    {
        cout << filename << " not found";
        exit(1);
    }

    while (in.get(c))
        buffer += c;

    in.close();

    return buffer;
}