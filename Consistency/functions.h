#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stack>

using namespace std;

struct my_structure
{

private:
    string str;
    int index;

public:
    my_structure(string str, int index)
    {
        this->str = str;
        this->index = index;
    }
    string get_str() { return this->str; }
    void set_str(string str) { this->str = str; }
    int get_index() { return this->index; }
};

vector<my_structure> initial_filteration(vector<my_structure> structs);

vector<my_structure> Check_Consistency(vector<string> &strings, string &str);

vector<string> split_string(const string &str, const string &delimiter);

string trim_first(string str);

string getFile(string filename);

string vector_to_string(vector<string> vec);

void fix_syn_error(vector<my_structure> &structs);

void re_fix_syn_error(vector<my_structure> &structs);

void fix_xml(vector<string> &strings, vector<my_structure> &structs, string &str);

void unpair_errors(vector<my_structure> &structs);

void XML_Minify(vector<string> &XML_fixed);

void xml_cutter(vector<string> &lines);