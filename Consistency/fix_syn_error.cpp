
#include "functions.h"

void fix_syn_error(vector<my_structure> &structs)
{
    int no_spaces = 0;
    for (int i = 0; i < structs.size(); i++)
    {
        // in case of unmatched string set the string to " " to be dealt with later
        if (structs[i].get_str().front() == '<' && structs[i].get_str().back() == '>')
            structs[i].set_str(" "); // function by zenzen to add the missing '<' or '>'
        string mazboot2 = "";
        if (structs[i].get_str()[0] == '<')
        {
            string mazboot = "";
            int j;
            for (j = 0; j < structs[i].get_str().size(); j++)
            {
                mazboot += structs[i].get_str()[j];
                continue;
            }
            if (mazboot[j] != '>')
            {
                mazboot2 = mazboot + '>';
                structs[i].set_str(mazboot2);
            }
        }
        else if (structs[i].get_str()[structs[i].get_str().size() - 1] == '>')
        {
            string mazboot = "";
            mazboot = '<' + structs[i].get_str();
            structs[i].set_str(mazboot);
        }
        else
            continue;
    }
}

void re_fix_syn_error(vector<my_structure> &structs)
{
    for(int i=0; i<structs.size(); i++){
        
        string cur_str = structs[i].get_str();
        if (cur_str.front() == '<' && cur_str.back() == '>') structs[i].set_str(" "); 
        else if(cur_str.front() != '<' && cur_str.back() == '>') structs[i].set_str('<' + cur_str);
        else if(cur_str.front() == '<' && cur_str.back() != '>') structs[i].set_str(cur_str + '>');
    }
}
