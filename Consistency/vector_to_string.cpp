#include "functions.h"

string vector_to_string(vector<string> vec)
{

    string output ="";
    for (int i = 0; i < vec.size(); i++)
    {
        if(i == vec.size()-1) output +=  vec[i];
        else output +=  vec[i] + "\n";
    }
    return output;
}