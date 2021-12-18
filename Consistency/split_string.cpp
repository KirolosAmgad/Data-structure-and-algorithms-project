#include "functions.h"

vector<string> split_string(const string &str, const string &delimiter)
{
    vector<string> strings;

    size_t pos = 0;
    size_t prev = 0;
    while ((pos = str.find(delimiter, prev)) != string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }
    strings.push_back(str.substr(prev));
    return strings;
}