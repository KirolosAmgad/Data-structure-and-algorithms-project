#include "functions.h"

using namespace std;

vector<my_structure> Check_Consistency(vector<string> &strings, string &str)
{

    bool debug = 0, endofword, error, matched, child_node, consistent = 0;
    vector<my_structure> structs, unique_struct;
    stack<string> stk, temp_stk;
    stack<int> stk_index, temp_stk_index;
    vector<string> xml_wt_error;

    for (int i = 0; i < strings.size(); i++)
    {
        string current_string = trim_first(strings[i]);
        if (current_string.front() != '<' && current_string.back() != '>')
            continue;

        for (int j = 0; j < current_string.size(); j++)
        {
            string tag = "";
            endofword = 0;

            // while loop to form the word
            if (current_string[j] != '<' && j != 0)
                continue;
            while (!endofword && j < current_string.size())
            {
                tag += current_string[j];
                if (current_string[j++] == '>')
                    endofword = 1;
            }

            // if matched tag pop out of stack
            if (!stk.empty() && stk.top() == (tag.front() + tag.substr(2)))
            {
                stk.pop();
                stk_index.pop();
            }

            // else push it into stack for
            else
            {
                stk.push(tag);
                stk_index.push(i + 1);
            }
        }

        if (debug)
        {
            cout << "____________________printing stack contents____________________" << endl;
            temp_stk = stk;
            temp_stk_index = stk_index;
            while (!temp_stk.empty())
            {
                cout << temp_stk_index.top() << "->   " << temp_stk.top() << endl;
                temp_stk.pop();
                temp_stk_index.pop();
            }
        }
    }

    // print file consistency

    if (stk.empty()) consistent = 1;
    if (debug)
    {
        cout << "____________________is consistent____________________" << endl;
        cout << ( consistent? "File is consistent" : "File is not consistent") << endl;
    }

    // fill the elements in the 2 stack(elements, indices) in the stucture
    while (!stk_index.empty())
    {
        my_structure st(stk.top(), stk_index.top());
        structs.push_back(st);
        stk_index.pop();
        stk.pop();
    }


    if (debug)
    {
        cout << "____________________Stack Leftovers______________________" << endl;
        for (int i = 0; i < structs.size(); i++)
        {
            cout << structs[i].get_index() << "->   " << structs[i].get_str() << endl;
        }
    }

    //function to remove pairs with no errors
    unique_struct = initial_filteration(structs);

    if (debug)
    {
        cout << "____________________Stack Leftovers after initial filteration______________________" << endl;
        for (int i = 0; i < unique_struct.size(); i++)
        {
            cout << unique_struct[i].get_index() << "-> " << unique_struct[i].get_str() << endl;
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

    //cout << "________________POINTING OUT THE ERRORS_______________" << endl;

    for (int i = 0; i < strings.size(); i++)
    {
        error = 0, child_node = 0;
        for (int j = 0; j < unique_struct.size(); j++)
        {
            if (i == (unique_struct[j].get_index() - 1))
                error = 1;
        }

        if (i > 0 
        && trim_first(strings[i]).front() != '<' 
        && trim_first(strings[i - 1])[1] != '/' 
        && ((trim_first(strings[i + 1]).front() == '<'  && trim_first(strings[i + 1])[1] == '/') 
        || (trim_first(strings[i + 1]).front() == '/'))
        ) child_node = 1;

        if (error && trim_first(strings[i]).front() != '<')
            xml_wt_error.push_back((strings[i]) + " --------------> ERROR 1: No opening tag");
        else if (error && trim_first(strings[i]).back() == '>')
            xml_wt_error.push_back((strings[i]) + " --------------> ERROR 4: Not matched");
        else if (error)
            xml_wt_error.push_back((strings[i]) + " --------------> ERROR 2: No closeing tag");
        else if (!child_node && (trim_first(strings[i]).front() != '<') && (trim_first(strings[i]).back() != '>') || (strings[0].front() != '<' && !i))
            xml_wt_error.push_back((strings[i]) + " --------------> ERROR 3: No Tag");
        else
            xml_wt_error.push_back((strings[i]));
    }
  
    if(consistent) str = "File doesn't contain erros";
    else str = vector_to_string(xml_wt_error);

    return unique_struct;
}
