#include "functions.h"

void unpair_errors(vector<my_structure> &structs)
{
    bool debug=0;
    for (int i = 0; i < structs.size(); i++)
    {   
        bool rm = 0;
        string current_string = structs[i].get_str();
        
        if ((current_string.front() == '<') && (current_string.back() == '>'))
        {
            for (int j = 0; j < structs.size(); j++)
            {
                if (i == j) continue;
                string cmpstr = structs[j].get_str();

                if(debug) cout << current_string << "  compared with  " << cmpstr << endl;
                if (current_string.substr(1) == cmpstr.substr(1)
                || current_string.substr(2) == cmpstr
                || current_string.substr(1, current_string.size() - 2) == cmpstr.substr(2)
                || current_string.substr(2, current_string.size() - 3) == cmpstr.substr(1) )
                    rm = 1;
            }
        }
        
        if (rm)
        {
            if(debug) cout << "i will erase " << structs[i].get_str() << endl;
            structs.erase(structs.begin() + i);
            i--;
        }
    }
}