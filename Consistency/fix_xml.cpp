#include "functions.h"

void fix_xml(vector<string> &strings, vector<my_structure> &structs, string &str)
{

    bool error, error2, error3, matched, debug = 0;
    vector<string> post_fix;
    vector<my_structure> unique_struct;

    //loop to return the pair of elements that have an error or the unmatched elements from array
    for (int i = 0; i < structs.size(); i++)
    {
        matched = 0;
        for (int j = 0; j < structs.size(); j++)
        {
            if ((structs[i].get_str() == (structs[j].get_str()[0] + structs[j].get_str().substr(2)) || structs[j].get_str() == (structs[i].get_str()[0] + structs[i].get_str().substr(2))) && i != j)
                matched = 1;
        }
        if (!matched)
        {
            my_structure st(structs[i].get_str(), structs[i].get_index());
            unique_struct.push_back(st);
        }
    }

    // function to remove the correct element out of the pair that contains an actual error from the vector
    unpair_errors(unique_struct);

    if (debug)
    {
        cout << "________________after unpair_errors_______________" << endl;
        for (int i = 0; i < unique_struct.size(); i++)
        {
            cout << unique_struct[i].get_index() << "-> " << unique_struct[i].get_str() << endl;
        }
    }

    //function to fix the syntax error by adding '<' at the begining or '>' at the end
    //or mark the unmatched elements by making them an empty string " "
    re_fix_syn_error(unique_struct);

    if (debug)
    {
        cout << "________________after fix_syn_error_______________" << endl;
        for (int i = 0; i < unique_struct.size(); i++)
        {
            cout << unique_struct[i].get_index() << "-> " << unique_struct[i].get_str() << endl;
        }
    }

    // loop to
    int k = unique_struct.size();
    for (int i = 0; i < strings.size(); i++)
    {
        error = 0;
        for (int j = 0; j < unique_struct.size(); j++)
        {
            if (i == (unique_struct[j].get_index() - 1))
                error = 1;
        }
        if (error)
            post_fix.push_back(unique_struct[--k].get_str());
        else
        {
            post_fix.push_back(trim_first(strings[i]));
        }
    }

    //loop to delete any error in the format ( ERROR 3: No Tag )
    for (int k = 0; k < post_fix.size(); k++)
    {
        error2 = 1;
        //error2 sets to 0 before a child node
        if (k > 1 
        && post_fix[k].front() != '<' 
        && post_fix[k - 1].front() == '<'  && post_fix[k - 1][1] != '/'  
        && post_fix[k + 1].front() == '<'  && post_fix[k + 1][1] == '/'
        ) error2 = 0;

        if (post_fix[k].front() != '<' && error2)post_fix.erase(post_fix.begin() + k);
    }

    //loop to delete any empty lines to correct errors in the format ( ERROR 4: Not matched )
    for (int k = 0; k < post_fix.size(); k++)
    {
        if (post_fix[k] == " ")
        {
            post_fix.erase(post_fix.begin() + k);
        }
    }

    if(debug){
        cout << "________________fixing errors_______________" << endl;
        for (int k = 0; k < post_fix.size(); k++)
        {
            cout << post_fix[k] << endl;
        }
    }
    str = vector_to_string(post_fix);
}