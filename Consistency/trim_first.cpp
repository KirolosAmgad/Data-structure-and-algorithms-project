#include "functions.h"

string trim_first(string str)
{

    string output = "";
    bool flag = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == ' ' && !flag) ;
        else
        {
            flag = 1;
            output = output + str[i];
        }
    }
    return output;
}