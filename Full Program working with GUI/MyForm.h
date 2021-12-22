#pragma once
#pragma once
#include <iostream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <msclr/marshal_cppstd.h>
#include <stdlib.h>
#include "stdc++.h"
#include <queue>
#include <string.h>
#include <sstream>
#include <map>
#include <unordered_map>

namespace XMLEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
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

	vector<my_structure> initial_filteration(vector<my_structure> structs)
	{
		vector<my_structure> unique_struct;
		bool matched;
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
		return unique_struct;
	}

	void fill_empty_strings(vector<string>& strings) {
		for (int i = 0; i < strings.size(); i++) {
			if (strings[i] == "") {
				strings[i] = "..........";
			}
		}
	}

	void erase_empty_strings(vector < string >& strings) {
		for (int i = 0; i < strings.size(); i++) {
			if (strings[i] == "" || strings[i].find_first_not_of(' ') == strings[i].npos) {
				strings.erase(strings.begin() + i);
			}

		}
	}

	vector<string> split_string(const string& str, const string& delimiter = "\n")

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

	string vector_to_string(vector<string> vec)
	{

		string output = "";
		for (int i = 0; i < vec.size(); i++)
		{
			if (i == vec.size() - 1) output += vec[i];
			else output += vec[i] + "\n";
		}
		return output;
	}

	string trim_first(string str)
	{



		string output = "";
		bool flag = 0;
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] == ' ' && !flag)
				;
			else
			{
				flag = 1;
				output = output + str[i];
			}
		}
		return output;
	}

	void unpair_errors(vector<my_structure>& structs)
	{
		bool debug = 0;
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

					if (debug) cout << current_string << "  compared with  " << cmpstr << endl;
					if (current_string.substr(1) == cmpstr.substr(1)
						|| current_string.substr(2) == cmpstr
						|| current_string.substr(1, current_string.size() - 2) == cmpstr.substr(2)
						|| current_string.substr(2, current_string.size() - 3) == cmpstr.substr(1))
						rm = 1;
				}
			}

			if (rm)
			{
				if (debug) cout << "i will erase " << structs[i].get_str() << endl;
				structs.erase(structs.begin() + i);
				i--;
			}
		}
	}

	void xml_cutter(vector<string>& lines) {

		vector<string> unique_lines;
		string input, line, d_q = " ";
		int start, end;

		for (int j = 0; j < lines.size(); j++)
		{
			input = trim_first(lines[j]);
			for (int i = 0; i < input.length(); i++)
			{
				if (input[i] == '<')
				{
					start = i;
					for (i; i < input.length(); i++)
					{
						if (input[i] == '>')
						{
							end = i;
							break;
						}
					}
				}
				else
				{
					start = i;
					for (i; i < input.length(); i++)
					{
						if (input[i] == '<')
						{
							end = i - 1;
							i--;
							break;
						}
						else
						{
							end = i;
						}
					}
				}
				line = input.substr(start, (end - start + 1));
				if (line[0] != '<')
					for (int k = 0; k < line.length(); k++)
					{
						if (line[k] == '"')
							line.replace(k, 1, d_q);
					}
				if (line[0] != ' ' || line[1] != ' ' || line[2] != ' ')
					unique_lines.push_back(line);
			}
		}
		lines = unique_lines;
	}


	vector<my_structure> Check_Consistency(vector<string>& strings, string& str)
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
					cout << temp_stk_index.top() << "-> " << temp_stk.top() << endl;
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
			cout << (consistent ? "File is consistent" : "File is not consistent") << endl;
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
				cout << structs[i].get_index() << "-> " << structs[i].get_str() << endl;
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
				&& ((trim_first(strings[i + 1]).front() == '<' && trim_first(strings[i + 1])[1] == '/')
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

		if (consistent) str = "File doesn't contain erros";
		else str = vector_to_string(xml_wt_error);

		return unique_struct;
	}
	
	void fix_syn_error(vector<my_structure>& structs)
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

	void re_fix_syn_error(vector<my_structure>& structs)
	{
		for (int i = 0; i < structs.size(); i++) {

			string cur_str = structs[i].get_str();
			if (cur_str.front() == '<' && cur_str.back() == '>') structs[i].set_str(" ");
			else if (cur_str.front() != '<' && cur_str.back() == '>') structs[i].set_str('<' + cur_str);
			else if (cur_str.front() == '<' && cur_str.back() != '>') structs[i].set_str(cur_str + '>');
		}
	}


	void fix_xml(vector<string>& strings, vector<my_structure>& unique_struct, string& str)
	{



		bool error, error2, debug = 0;
		vector<string> post_fix;



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
				&& post_fix[k - 1].front() == '<' && post_fix[k - 1][1] != '/'
				&& post_fix[k + 1].front() == '<' && post_fix[k + 1][1] == '/'
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



		if (debug) {
			cout << "________________fixing errors_______________" << endl;
			for (int k = 0; k < post_fix.size(); k++)
			{
				cout << post_fix[k] << endl;
			}
		}
		str = vector_to_string(post_fix);
	}

	string XML_Minify(const vector<string>& XML_fixed)
	{

		string output = "";
		bool strip;

		for (int i = 0; i < XML_fixed.size(); i++)
		{
			strip = 1;
			for (int j = 0; j < XML_fixed[i].size(); j++)
			{
				if (XML_fixed[i][j] == ' ' && strip)
					continue;
				else
				{
					if (XML_fixed[i][j] == '<')
						strip = 1;
					else
						strip = 0;
				}
				output += XML_fixed[i][j];
			}
		}
		return output;

	}

	void indent(vector<string>& space, int& index, int NumOfSpaces)
	{
		for (int i = 0; i < NumOfSpaces; i++)
		{
			space[index] = space[index] + " ";
		}
		index++;
	}

	void XML_indent(vector<string>& OutputSpaces, vector<string>& XML_Fixed, unsigned int SizeOfXML)  //big-O -> O(n*m) where n is the number of lines in the xml and m is number of spaces added in each indentation addition
	{
		char start;
		char end;
		char anotherend;
		bool flag = false;
		unsigned int LineIndex = 0;
		int size = 0;

		string temp;
		string t;
		int j = 0;
		stack <string> OpenAngleStack;

		while (LineIndex < SizeOfXML)			// if we use vector.size() it will compare it with the number allocated to it 6000 in our case
		{
			temp = XML_Fixed[(LineIndex)];
			LineIndex++;

			if ((temp == "") || (((temp[1] == '?') || (temp[1] == '!')) && temp[0] == '<') || ((temp.find("<") == -1) && (temp.find(">") != -1)) || ((temp.find(">") == -1) && (temp.find("<") != -1)))
			{
				indent(OutputSpaces, size, j);
				continue;
			}

			start = temp.find("<") + 1;
			end = temp.find(">");
			if (temp[start] == '/')
			{
				t = temp.substr(start, end - start);
				OpenAngleStack.pop();
				j = j - 4;

			}

			else
			{

				if ((end != -1) && (temp[end - 1] == '/'))						// SELF CLOSING TAGS
				{
					indent(OutputSpaces, size, j);
					continue;
				}

				anotherend = temp.find(" ");
				if (anotherend > start && anotherend < end)
				{
					t = temp.substr(start, anotherend - start);
				}
				else
				{
					t = temp.substr(start, end - start);
				}
				if (t == "frame")
				{
					indent(OutputSpaces, size, j);
					continue;
				}
				OpenAngleStack.push(t);
				j = j + 4;
				temp = temp.substr(end + 1);

				if (temp != "") {										// check if the closing angle is in the same string if so parse it

					start = temp.find("<") + 1;
					end = temp.find(">");
					t = temp.substr(start + 1, end - (start + 1));
					OpenAngleStack.pop();
					j = j - 4;

				}
				else
				{
					indent(OutputSpaces, size, j - 4);
					flag = true;
				}
			}
			if (flag == false)
			{
				indent(OutputSpaces, size, j);
			}
			else
			{
				flag = false;
			}

		}
	}


	/////////////////////////////////////////////xml to json////////////////////////////////////////


	static inline void ltrim(string& s) {
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !isspace(ch);
			}));
	}
	static inline void rtrim(string& s) {
		s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !isspace(ch);
			}).base(), s.end());
	}

	static inline void trim(string& s) {
		ltrim(s);
		rtrim(s);
	}
	int getDuplicate(vector<string> &v, string &key) {

		map<string, int> dup;

		for_each(v.begin(), v.end(), [&dup](string val) { dup[val]++; });
		return dup[key];
	}
	struct Node
	{
		string key;
		string value;
		string attr;
		vector<Node*> child;
		Node* parent;

	};

	class GeneralTree {
		Node* root;

	public:
		vector<string> start;
		Node* nodePointer = nullptr;
		int iBeautify = 0;
		int counter = 0;
		int counter2 = 0;
		bool checker = false;
		stringstream json;
		string prevTag = "";
		vector<string> tmpTag;
		vector<string> v;
		string return_string;



		GeneralTree();
		void newNode(Node*& node, bool rootCheck);

		string beautify() {
			iBeautify = 0;
			return_string = "";
			string returnString2;
			vector<string>::iterator it = start.begin();
			int size = start.size();
			for (it; it != start.end(); it++) {
				returnString2 += *it;
				size--;
				if (size != 0)
					returnString2 += '\n';

			}

			returnString2 += beautifyPriv(this->root);
			return  returnString2;

		}
		string beautifyPriv(Node* node)
		{
			Node* tmp = node;
			Node* nodePointerBeautify = tmp;
			if (tmp == NULL)
			{
				return "";
			}
			string att;




			return_string += '\n' + string(iBeautify, '\t') + "<" + tmp->key + tmp->attr + ">" + tmp->value;
			att = "";
			iBeautify++;
			vector<Node*>::iterator it = (tmp->child).begin();
			int size = tmp->child.size();

			if (size != 0)
			{
				while (size != 0)
				{
					beautifyPriv(*it);
					it++;
					size--;
					iBeautify--;


				}
				return_string += '\n' + string(iBeautify - 1, '\t') + "</" + nodePointerBeautify->key + ">";

			}
			else
			{
				return_string += "</" + tmp->key + ">";
				nodePointerBeautify = nodePointerBeautify->parent;


			}
			return return_string;

		}


		string minify() {
			iBeautify = 0;
			return_string = "";
			string returnString2;
			vector<string>::iterator it = start.begin();
			int size = start.size();
			for (it; it != start.end(); it++) {
				returnString2 += *it;
				size--;
				if (size != 0)
					returnString2 += '\n';

			}

			returnString2 += minifyPriv(this->root);
			return  returnString2;

		}
		string minifyPriv(Node* node)
		{
			Node* tmp = node;
			Node* nodePointerBeautify = tmp;
			if (tmp == NULL)
			{
				return "";
			}
			string att;


			return_string += "<" + tmp->key + tmp->attr + ">" + tmp->value;
			att = "";
			iBeautify++;
			vector<Node*>::iterator it = (tmp->child).begin();
			int size = tmp->child.size();

			if (size != 0)
			{
				while (size != 0)
				{
					minifyPriv(*it);
					it++;
					size--;
					iBeautify--;


				}
				return_string += "</" + nodePointerBeautify->key + ">";

			}
			else
			{
				return_string += "</" + tmp->key + ">";
				nodePointerBeautify = nodePointerBeautify->parent;


			}
			return return_string;

		}




		/////////////////////////////////////conversion///////////////////////////////////////////////////////
		string convert_to_json() {
			json.str("");
			iBeautify = 0;
			json << "{";
			convert_to_jsonPriv(this->root);
			json << endl << "}" << endl;
			return json.str();
		}
		string convert_to_jsonPriv(Node* node)
		{

			Node* tmp = node;
			Node* nodePointerBeautify = tmp;
			int tmpCounter;
			if (tmp == NULL)
				return 0;


			if (tmp->key != prevTag) {
				json << endl << string(iBeautify + 1, '\t') << "\"" << tmp->key << "\":";
			}
			if (tmp != root && counter < tmp->parent->child.size() - 1 && tmp->key == tmp->parent->child[counter + 1]->key && tmp->key != prevTag) {
				json << "[";


				counter = 0;
				prevTag = tmp->key;
				checker = true;


			}
			else {
				checker = false;
			}

			if ((tmp->child).size() != 0) {


				string str = tmp->attr;
				rtrim(str);
				ltrim(str);
				str.insert(0, "\"_");
				int len = str.length() - 1;
				for (int i = 0; i < len; i++) {
					if (str[i] == ' ' && str[i + 1] == ' ') {
						str.erase(i, 1);
						i--;
						len--;
					}
				}
				//cout<<str<<endl;
				for (int i = 0; i < str.length(); i++) {
					if (str[i] == '=') {

						str[i] = ':';
						str.insert(i, "\"");
						if (str[i - 1] == ' ') {
							str.erase(i - 1, 1);
						}
					}
					if (str[i] == ' ' && str[i - 1] == '\"') {
						str[i] = ',';
						str.insert(i + 1, "\"_");
						str.insert(i + 1, string(iBeautify + 2, '\t'));
						str.insert(i + 1, "\n");
					}
				}
				str.insert(str.end(), ',');

				json << "{" << tmp->value;

				for (int i = 0; tmp->child.size() != 0 && i < tmp->child.size(); i++) {
					v.push_back(tmp->child[i]->key);
				}

				if (tmp->attr.length() > 1) {
					json << endl << string(iBeautify + 2, '\t') << str;
				}
				tmpCounter = counter;
				counter = 0;

				tmpTag.push_back(prevTag);
				prevTag = "";

			}
			else {

				json << "\"" << tmp->value << "\"";

				if (counter != (tmp->parent->child).size() ) {

					if (tmp->key == prevTag && counter == getDuplicate(v, tmp->key) - 1) {

						json << "]";
					}

					json << ",";

					counter++;
				}
			}

			iBeautify++;

			vector<Node*>::iterator it = (tmp->child).begin();
			int size = tmp->child.size();

			if (size != 0)
			{
				while (size != 0)
				{
					convert_to_jsonPriv(*it);
					it++;
					size--;
					iBeautify--;
				}
				json << endl << string(iBeautify, '\t') << "}";

				counter = tmpCounter + 1;

				prevTag = tmpTag[tmpTag.size() - 1];
				tmpTag.pop_back();
				if (tmp->key == prevTag && counter == (tmp->parent->child).size()) json << "]";
				if (nodePointerBeautify != root)
					if (counter2 != (nodePointerBeautify->parent->child).size() - 1 && nodePointerBeautify->parent == root) {
						json << ",";
						v.clear();
						counter2++;
					}
					else if (counter < (tmp->parent->child).size()) {
						json << ",";

					}
			}
			else
			{

				nodePointerBeautify = nodePointerBeautify->parent;
			}
			//cout<<ss.str();
			return json.str();
		}

	};

	GeneralTree::GeneralTree() {
		root = NULL;
		nodePointer = root;
	}

	void GeneralTree::newNode(Node*& node, bool rootCheck)
	{
		Node* tmp = node;
		tmp->parent = nodePointer;
		if (nodePointer != NULL)
			nodePointer->child.push_back(tmp);
		nodePointer = tmp;


		if (rootCheck)
			this->root = tmp;


	}

	GeneralTree createTree(string xmlCode) {

		GeneralTree tree;
		vector<string> seglist1;
		vector<string> seglist2;

		string segment;
		stringstream ss;
		vector<string> att;
		string attr;


		bool rootTag = true;
		for (int i = 0; i < xmlCode.length(); i++)
		{
			if (xmlCode[i] == '<' && xmlCode[i + 1] == '?')
			{
				int endTag;
				string startValue;
				endTag = xmlCode.find('>', i);
				startValue = xmlCode.substr(i, endTag + 1);
				i = endTag;
				tree.start.push_back(startValue);
			}
			if (xmlCode[i] == '<' && xmlCode[i + 1] != '/')
			{
				int endTag;

				string tagNameAttr;
				string tagName;
				endTag = xmlCode.find('>', i);
				tagNameAttr = xmlCode.substr(i + 1, endTag - i - 1);
				stringstream ss(tagNameAttr);
				string segment;
				vector<string> seglist;


				while (getline(ss, segment, ' '))
				{
					seglist.push_back(segment);
				}
				vector<string>::iterator it = seglist.begin();
				tagName = *it;
				it++;

				while (it != seglist.end())
				{
					att.push_back(*it);
					it++;

				}
				vector<string>::iterator it2 = att.begin();


				while (it2 != att.end())
				{
					attr += " ";
					attr += *it2;

					it2++;
				}

				//            }

				Node* node = new Node;

				node->key = tagName;
				node->attr = attr;
				i = endTag;
				if (rootTag)
				{
					tree.newNode(node, true);
					rootTag = false;
					seglist.clear();

					att.clear();
					attr = "";
				}
				else
				{
					tree.newNode(node, false);
					seglist.clear();

					attr = "";

					att.clear();
				}
			}
			else if (xmlCode[i] == '<' && xmlCode[i + 1] == '/')
			{
				int endTag;
				endTag = xmlCode.find('>', i);
				i = endTag;
				tree.nodePointer = tree.nodePointer->parent;

			}
			else if (xmlCode[i] != '<' && xmlCode[i - 1] == '>')
			{
				int endTag;
				string tagValue;
				endTag = xmlCode.find('<', i);
				tagValue = xmlCode.substr(i, endTag - i);
				ltrim(tagValue);
				rtrim(tagValue);
				tree.nodePointer->value = tagValue;
				i = endTag - 1;
			}



		}

		return tree;
	}

	string xml_to_json(string xmlCode) {
		GeneralTree tree;

		rtrim(xmlCode);
		ltrim(xmlCode);
		tree = createTree(xmlCode);
		vector<string> strings = split_string(tree.convert_to_json());
		for (int i = 0; i < strings.size() - 1; i++) {
			string s = strings[i + 1];
			ltrim(s);
			if (strings[i].back() == ',' && s.front() == '}') {
				strings[i].pop_back();
			}
		}
		return vector_to_string(strings);
	}


	//////////////////////////////////////////////// encode //////////////////////////////////////

	vector<int> encoding(string inputStr)
	{
		map<string, int> codeTable;
		for (int i = 0; i <= 255; i++) {
			string ch = "";
			ch += char(i);
			codeTable[ch] = i;
		}
		string p = "", c = "";
		p += inputStr[0];
		int code = 256;
		vector<int> output_code;
		for (int i = 0; i < inputStr.length(); i++) {
			if (i != inputStr.length() - 1)
				c += inputStr[i + 1];
			if (codeTable.find(p + c) != codeTable.end()) {
				p = p + c;
			}
			else {
				output_code.push_back(codeTable[p]);
				codeTable[p + c] = code;
				code++;
				p = c;
			}
			c = "";
		}
		output_code.push_back(codeTable[p]);
		return output_code;
	}

	string decoding(vector<int> encodeStr)
	{
		map<int, string> codeTable;
		for (int i = 0; i <= 255; i++) {
			string ch = "";
			ch += char(i);
			codeTable[i] = ch;
		}
		int tmp = encodeStr[0], n;
		string s = codeTable[tmp];
		string c = "";
		string out = "<";
		c += s[0];
		int count = 256;
		for (int i = 0; i < encodeStr.size() - 1; i++) {
			n = encodeStr[i + 1];
			if (codeTable.find(n) == codeTable.end()) {
				s = codeTable[tmp];
				s = s + c;
			}
			else {
				s = codeTable[n];
			}
			c = "";
			c += s[0];
			codeTable[count] = codeTable[tmp] + c;
			

			count++;
			tmp = n;
			out += codeTable[tmp];
		}
		return out;
	}

	//////////////////////////////////////// compress and decompress /////////////////////////////////////////////


	class c_node {
	private:
		char char_;                          // char of the cnode
		c_node* left;                         // left child if any
		c_node* right;                        // right child  if any
		std::string huff_code;               //new huffmancode to assign to the char
							   // how many time this char is occupy in whole document
	public:
		int frequency;
		c_node(char char_);
		c_node(int freq);
		c_node();
		~c_node();
		friend class Fcompress;
		friend class Compare;

	}; typedef c_node* nptr;

	class Compare {                       //compare fn for min heap queue
	public:
		bool operator()(const c_node* x, const c_node* y)const
		{
			return (x->frequency > y->frequency);
		}
	};

	class Fcompress {

	private:
		//private attributes
		char char_;                          //buffer to read file
		c_node* code_table[128];             //table of code indexed with ascii index (0 --> 127)
		c_node* root;
		std::priority_queue<nptr, std::vector<nptr>, Compare> min_Q;
		std::string r_filename;
		std::fstream r_file;
		std::fstream w_file;
		string converted_output_path;
		// private methods
		void free_resources(c_node* p);
		void count_frq();                      // count frequencies of all charachters in document
		void create_tree();                                     // create huffman code tree using min heap (priority Q)
		void create_tree(std::string code, char char_);          // create tree from compressed file header
		void calculate_leaves(c_node* p, std::string code);
		void fill_table();                                      // fill generated huffman codes to table
		int Bin_t_Dec(std::string b);
		std::string Dec_t_Bin(int d);
		void write_to_file();
		void create_tree_from_header();
		void read_from_file();

	public:
		Fcompress(std::string rfilename);
		~Fcompress();
		void compress();
		void decompress();
		void save_as();
		void save_as_xml();




	};

	c_node::c_node(char char_)
	{
		this->char_ = char_;
		this->frequency = 0;
		this->left = NULL;
		this->right = NULL;
	}
	c_node::c_node(int freq)
	{
		this->frequency = freq;
		this->left = NULL;
		this->right = NULL;
	}

	c_node::c_node() {
		this->left = NULL;
		this->right = NULL;
	}
	c_node::~c_node() {

	}



	Fcompress::Fcompress(std::string rfilename) {
		for (int i = 0; i < 128; i++)
		{
			this->code_table[i] = new c_node((char)i);
		}
		this->r_filename = rfilename;
		//this->w_filename = wfilename;
	}
	Fcompress::~Fcompress() {
		free_resources(this->root);
	}

	void Fcompress::save_as() {
		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
		saveFileDialog1->InitialDirectory = "c:\\";
		saveFileDialog1->Filter = "Compressed File (*.txt)|*.txt";
		saveFileDialog1->FilterIndex = 1;
		saveFileDialog1->RestoreDirectory = true;
		saveFileDialog1->Title = "Choose Compressed File Location Destination";

		if (System::Windows::Forms::DialogResult::OK != saveFileDialog1->ShowDialog()) return;

		converted_output_path = msclr::interop::marshal_as< string >(saveFileDialog1->FileName);


	}

	void Fcompress::save_as_xml() {
		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
		saveFileDialog1->InitialDirectory = "c:\\";
		saveFileDialog1->Filter = "XML File (*.xml)|*.xml";
		saveFileDialog1->FilterIndex = 1;
		saveFileDialog1->RestoreDirectory = true;
		saveFileDialog1->Title = "Choose Decompressed File Location Destination";

		if (System::Windows::Forms::DialogResult::OK != saveFileDialog1->ShowDialog()) return;

		converted_output_path = msclr::interop::marshal_as< string >(saveFileDialog1->FileName);


	}


	void Fcompress::free_resources(c_node* p) {
		if (p)
		{
			if (p->left) {
				free_resources(p->left);
			}
			if (p->right)
			{
				free_resources(p->right);
			}
			delete p;
		}
	}

	void Fcompress::count_frq() {
		r_file.open(r_filename, std::ios::in);


		do {
			r_file.get(char_);
			code_table[(int)char_]->frequency++;

		} while (!r_file.eof());
		r_file.close();

	}
	void Fcompress::create_tree(std::string code, char chara) {
		c_node* parent = this->root;
		for (unsigned int i = 0; i < code.size(); i++)
		{
			if (code[i] == '1')
			{
				if (parent->right == NULL)
					parent->right = new c_node;
				parent = parent->right;
			}
			else if (code[i] == '0')
			{
				if (parent->left == NULL)
					parent->left = new c_node;
				parent = parent->left;
			}
		}
		parent->char_ = chara;
	}
	void Fcompress::create_tree() {
		//creat min heap
		for (int i = 0; i < 128; i++)
		{
			if (code_table[i]->frequency)   //execluding non occupied characters from heap
				min_Q.push(code_table[i]);
		}
		// create the huffman tree
		std::priority_queue<nptr, std::vector<nptr>, Compare> temp(min_Q);
		while (temp.size() > 1) {
			root = new c_node;
			root->left = temp.top();
			temp.pop();
			root->right = temp.top();
			temp.pop();
			root->frequency = (root->left->frequency) + (root->right->frequency);
			temp.push(root);
		}
	}
	void Fcompress::calculate_leaves(c_node* p, std::string code) {
		if (p->left == NULL && p->right == NULL)
		{
			p->huff_code = code;
		}
		else
		{
			calculate_leaves(p->left, code + '0');
			calculate_leaves(p->right, code + '1');
		}
	}

	void Fcompress::fill_table() {
		calculate_leaves(root, "");
	}
	int Fcompress::Bin_t_Dec(std::string b)
	{
		int n = b.length();
		int sum = 0;

		for (int i = n - 1; i >= 0; i--)
		{
			int temp = (int)(b[n - i - 1] - '0') * (pow(2, i));
			sum += temp;

		}
		return sum;
	}

	std::string Fcompress::Dec_t_Bin(int d)
	{
		std::string temp = "", B = "";
		while (d > 0) {
			temp = (char)(d % 2 + '0') + temp;
			d = d / 2;
		}
		B.assign(8 - temp.length(), '0');
		B.append(temp);
		return B;
	}

	void Fcompress::write_to_file() {
		std::string to_write = "";
		std::string code = "";
		to_write += (char)min_Q.size();
		std::priority_queue<nptr, std::vector<nptr>, Compare> temp(min_Q);

		while (!temp.empty())
		{  // Save table in header of compressed file
			c_node* node = temp.top();
			to_write += node->char_;
			code.assign(127 - node->huff_code.size(), '0');    //put 0's to fill 127 bit chunck
			code += '1';                              //put 1 just before the real code (help us when reading it)
			code.append(node->huff_code);
			do {
				to_write += (char)Bin_t_Dec(code.substr(0, 8));
				code = code.substr(8);
			} while (code.length());
			temp.pop();
		}
		code.clear();
		// compress file data
		r_file.open(r_filename, std::ios::in);
		w_file.open(converted_output_path, std::ios::out | std::ios::binary);

		//    do {
		//        r_file.get(char_);
		//        code+=code_table[(int)char_]->huff_code;
		//        // packing huffman code to file as bytes
		//        while (code.size()>8)
		//        {
		//            to_write+=(char)Bin_t_Dec(code.substr(0,8));
		//            code=code.substr(8);
		//        }

		//       }while (!r_file.eof());
		r_file.get(char_);
		while (!r_file.eof())
		{

			code += code_table[(int)char_]->huff_code;
			// packing huffman code to file as bytes
			while (code.size() > 8)
			{
				to_write += (char)Bin_t_Dec(code.substr(0, 8));
				code = code.substr(8);
			}
			r_file.get(char_);
		}
		int zeros = 8 - code.size();
		//    for (int i=0;i<zeros;i++)
		//    {
		//        code='0'+code;
		//    }

		to_write += (char)Bin_t_Dec(code);
		to_write += (char)zeros;


		w_file.write(to_write.c_str(), to_write.size());
		r_file.close();
		w_file.close();
	}

	void Fcompress::create_tree_from_header() {
		r_file.open(r_filename, std::ios::in | std::ios::binary);
		unsigned char len;
		r_file.read(reinterpret_cast<char*>(&len), 1); // reinterpret_cast<char*> casting char to char* in cpp;
		root = new c_node;
		for (int i = 0; i < len; i++)
		{// every node saved as 1byte of orignal char and 16 byte of huff code
			char orig;                          //orignal byte
			unsigned char huffcode[16];         // huffman code to that orignal byte
			r_file.read(&orig, 1);
			r_file.read(reinterpret_cast<char*>(&huffcode), 16);
			std::string code = "";             // string representation of huffcode (i.e '10101')
			for (int j = 0; j < 16; j++)
			{
				code += Dec_t_Bin(huffcode[j]);
			}

			while (code[0] != '1')
			{//remove zerose added to 128 bit chunck till '1' we added before
				code = code.substr(1);
			}
			code = code.substr(1);// remove the extra '1' too
			create_tree(code, orig);
		}
		r_file.close();
	}

	void Fcompress::read_from_file() {

		r_file.open(r_filename, std::ios::in | std::ios::binary);
		
		w_file.open(converted_output_path, std::ios::out);
		unsigned char len;          //length of nodes stored in header
		char zeros;                 //number of zeroes in last byte of file
		r_file.read(reinterpret_cast<char*>(&len), 1);
		r_file.seekg(-1, std::ios::end);  // seek to last byte in file that represent zeroes;
		int last = r_file.tellg();
		r_file.read(&zeros, 1);
		r_file.seekg(1 + (17 * len), std::ios::beg);    //seek to first byte of file encoded data




		std::vector<unsigned char> wholetext;
		unsigned char segment;
		r_file.read(reinterpret_cast<char*>(&segment), 1);
		while (!r_file.eof())
		{//get the text byte by byte using unsigned char
			wholetext.push_back(segment);
			r_file.read(reinterpret_cast<char*>(&segment), 1);
		}
		c_node* parent = root;
		std::string code;
		for (unsigned int i = 0; i < wholetext.size() - 1; i++)
		{
			code = Dec_t_Bin(wholetext[i]);
			if (i == wholetext.size() - 2)
				code = code.substr(zeros);
			for (unsigned int j = 0; j < code.size(); j++)
			{
				if (code[j] == '0')
					parent = parent->left;
				else
					parent = parent->right;
				if (parent->left == NULL && parent->right == NULL)
				{
					w_file.put(parent->char_);
					parent = root;
				}
			}
		}
		r_file.close();
		w_file.close();


	}





	void Fcompress::compress() {

		count_frq();
		create_tree();
		fill_table();
		write_to_file();
	}

	void Fcompress::decompress() {
		create_tree_from_header();
		read_from_file();
	}





	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::RichTextBox^ InputBox;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::RichTextBox^ OutputBox;


	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^ saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ newToolStripMenuItem;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ Prettify_button;

	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ compress;
	private: System::Windows::Forms::Button^ decode;
	private: System::Windows::Forms::Button^ Decompress;





	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->InputBox = (gcnew System::Windows::Forms::RichTextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->OutputBox = (gcnew System::Windows::Forms::RichTextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->newToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->Prettify_button = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->compress = (gcnew System::Windows::Forms::Button());
			this->decode = (gcnew System::Windows::Forms::Button());
			this->Decompress = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// InputBox
			// 
			this->InputBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->InputBox->Font = (gcnew System::Drawing::Font(L"Courier New", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InputBox->ForeColor = System::Drawing::SystemColors::InfoText;
			this->InputBox->Location = System::Drawing::Point(23, 77);
			this->InputBox->Name = L"InputBox";
			this->InputBox->Size = System::Drawing::Size(548, 412);
			this->InputBox->TabIndex = 1;
			this->InputBox->Text = L"";
			this->InputBox->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(18, 45);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 29);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Input";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 13.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(665, 45);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(90, 29);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Output";
			// 
			// OutputBox
			// 
			this->OutputBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->OutputBox->Font = (gcnew System::Drawing::Font(L"Courier New", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->OutputBox->Location = System::Drawing::Point(670, 77);
			this->OutputBox->Name = L"OutputBox";
			this->OutputBox->ReadOnly = true;
			this->OutputBox->Size = System::Drawing::Size(672, 412);
			this->OutputBox->TabIndex = 4;
			this->OutputBox->Text = L"";
			this->OutputBox->TextChanged += gcnew System::EventHandler(this, &MyForm::OutputBox_TextChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1354, 28);
			this->menuStrip1->TabIndex = 6;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openToolStripMenuItem1,
					this->saveAsToolStripMenuItem, this->exitToolStripMenuItem, this->newToolStripMenuItem
			});
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(46, 24);
			this->openToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem1
			// 
			this->openToolStripMenuItem1->Name = L"openToolStripMenuItem1";
			this->openToolStripMenuItem1->Size = System::Drawing::Size(143, 26);
			this->openToolStripMenuItem1->Text = L"Open";
			this->openToolStripMenuItem1->Click += gcnew System::EventHandler(this, &MyForm::openToolStripMenuItem1_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->saveAsToolStripMenuItem->Text = L"Save As";
			this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveAsToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::exitToolStripMenuItem_Click);
			// 
			// newToolStripMenuItem
			// 
			this->newToolStripMenuItem->Name = L"newToolStripMenuItem";
			this->newToolStripMenuItem->Size = System::Drawing::Size(143, 26);
			this->newToolStripMenuItem->Text = L"New";
			this->newToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::newToolStripMenuItem_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(421, 506);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(111, 40);
			this->button1->TabIndex = 7;
			this->button1->Text = L"Minify";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// Prettify_button
			// 
			this->Prettify_button->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Prettify_button->Location = System::Drawing::Point(23, 506);
			this->Prettify_button->Name = L"Prettify_button";
			this->Prettify_button->Size = System::Drawing::Size(111, 40);
			this->Prettify_button->TabIndex = 8;
			this->Prettify_button->Text = L"Pretify";
			this->Prettify_button->UseVisualStyleBackColor = true;
			this->Prettify_button->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(140, 506);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(158, 40);
			this->button3->TabIndex = 9;
			this->button3->Text = L"Check for errors";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(304, 506);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(111, 40);
			this->button4->TabIndex = 10;
			this->button4->Text = L"Fix errors";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button5->Location = System::Drawing::Point(710, 506);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(111, 40);
			this->button5->TabIndex = 11;
			this->button5->Text = L"Encode";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button6
			// 
			this->button6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button6->Location = System::Drawing::Point(538, 506);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(166, 40);
			this->button6->TabIndex = 12;
			this->button6->Text = L"Convert to JSON";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->button2->Font = (gcnew System::Drawing::Font(L"Courier New", 19.8F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->button2->Location = System::Drawing::Point(577, 77);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(72, 41);
			this->button2->TabIndex = 13;
			this->button2->Text = L"<--";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click_1);
			// 
			// compress
			// 
			this->compress->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->compress->Location = System::Drawing::Point(944, 506);
			this->compress->Name = L"compress";
			this->compress->Size = System::Drawing::Size(141, 40);
			this->compress->TabIndex = 14;
			this->compress->Text = L"Compress File";
			this->compress->UseVisualStyleBackColor = true;
			this->compress->Click += gcnew System::EventHandler(this, &MyForm::compress_Click);
			// 
			// decode
			// 
			this->decode->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->decode->Location = System::Drawing::Point(827, 506);
			this->decode->Name = L"decode";
			this->decode->Size = System::Drawing::Size(111, 40);
			this->decode->TabIndex = 15;
			this->decode->Text = L"Decode";
			this->decode->UseVisualStyleBackColor = true;
			this->decode->Click += gcnew System::EventHandler(this, &MyForm::decode_Click);
			// 
			// Decompress
			// 
			this->Decompress->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Decompress->Location = System::Drawing::Point(1091, 506);
			this->Decompress->Name = L"Decompress";
			this->Decompress->Size = System::Drawing::Size(163, 40);
			this->Decompress->TabIndex = 16;
			this->Decompress->Text = L"Decompress File";
			this->Decompress->UseVisualStyleBackColor = true;
			this->Decompress->Click += gcnew System::EventHandler(this, &MyForm::Decompress_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(120, 120);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->AutoScroll = true;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(1354, 558);
			this->Controls->Add(this->Decompress);
			this->Controls->Add(this->decode);
			this->Controls->Add(this->compress);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->Prettify_button);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->OutputBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->InputBox);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MinimumSize = System::Drawing::Size(150, 100);
			this->Name = L"MyForm";
			this->Text = L"XML editor";
			this->AutoSizeChanged += gcnew System::EventHandler(this, &MyForm::OutputBox_TextChanged);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

	private: System::Void richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void OutputBox_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		//InputBox->Text
	}
	private: System::Void openToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

		openFileDialog1->InitialDirectory = "c:\\";
		openFileDialog1->Filter = "XML files (*.xml)|*.xml";
		openFileDialog1->FilterIndex = 1;
		openFileDialog1->RestoreDirectory = true;
		openFileDialog1->Title = "Choose XML File";


		if (System::Windows::Forms::DialogResult::OK != openFileDialog1->ShowDialog()) return;
		StreamReader^ sr = gcnew StreamReader(openFileDialog1->FileName);
		InputBox->Text = nullptr;
		try {
			for (;;) {
				String^ line = sr->ReadLine();
				if (line == nullptr) break;
				while (line != nullptr) {
					InputBox->Text += line + "\n";
					line = sr->ReadLine();
				}
			}
		}
		finally {
			sr->Close();
		}
	}
	private: System::Void newToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		InputBox->Text = nullptr;
		OutputBox->Text = nullptr;
	}
	private: System::Void saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;

		saveFileDialog1->InitialDirectory = "c:\\";
		saveFileDialog1->Filter = "XML files (*.xml)|*.xml|JSON files (*.json)|*.json|Encoded File (*.txt)|*.txt";
		saveFileDialog1->FilterIndex = 3;
		saveFileDialog1->RestoreDirectory = true;
		saveFileDialog1->Title = "Choose File Location";

		if (System::Windows::Forms::DialogResult::OK != saveFileDialog1->ShowDialog()) return;
		StreamWriter^ sw = gcnew StreamWriter(saveFileDialog1->FileName);
		try {

			sw->WriteLine(OutputBox->Text);
			sw->Close();

		}
		finally {
			sw->Close();
		}

	}
	private: System::Void exitToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		OutputBox->Text = InputBox->Text;
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		//   Minify button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);
		string output = XML_Minify(split_string(converted_input));
		String^ converted_output = gcnew String(output.c_str());
		OutputBox->Text = converted_output;

	}
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {

		// fix errors button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);

		string output = "";
		vector<string> vecs = split_string(converted_input);
		erase_empty_strings(vecs);
		vector<my_structure> structs = Check_Consistency(vecs, output);
		erase_empty_strings(vecs);
		fix_xml(vecs, structs, output);
		vector<string> output1 = split_string(output);
		erase_empty_strings(output1);

		xml_cutter(output1);
		vector<string> trial(output1.size());
		XML_indent(trial, output1, trial.size());
		for (int i = 0; i < output1.size(); i++) {
			trial[i] += output1[i];
		}
		erase_empty_strings(trial);

		string trial2 = vector_to_string(trial);
		String^ converted_output = gcnew String(trial2.c_str());
		OutputBox->Text = converted_output;

	}
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {

		// check for errors button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);

		string output = "";
		vector<string> vecs = split_string(converted_input);
		erase_empty_strings(vecs);
		Check_Consistency(vecs, output);
		String^ converted_output = gcnew String(output.c_str());
		OutputBox->Text = converted_output;
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {

		// pretify button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);
		vector<string> vecs = split_string(converted_input);
		xml_cutter(vecs);
		vector<string> trial(vecs.size());
		XML_indent(trial, vecs, vecs.size());
		for (int i = 0; i < vecs.size(); i++) {
			trial[i] += vecs[i];
		}
		erase_empty_strings(trial);
		string output = vector_to_string(trial);
		String^ converted_output = gcnew String(output.c_str());
		InputBox->Text = converted_output;
	}
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {

		//   xml to json button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);
		vector<string> convertedinput2 = split_string(converted_input);
		erase_empty_strings(convertedinput2);
		converted_input = vector_to_string(convertedinput2);
		string output = xml_to_json(converted_input);
		String^ converted_output = gcnew String(output.c_str());
		OutputBox->Text = converted_output;
	}
	private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) {
		InputBox->Text = OutputBox->Text;
	}
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {

		//   Encode button

		string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);
		vector<string> vecs = split_string(converted_input);

		vector<int> output1 = encoding(converted_input);
		stringstream output;
		copy(output1.begin(), output1.end(), ostream_iterator<int>(output, " "));
		//erase_empty_strings(vecs);
		//string output = compress(vecs, vecs.size());
		String^ converted_output = gcnew String(output.str().c_str());
		OutputBox->Text = converted_output;
		converted_output = "";
		for (int i = 0; i < output1.size(); i++) {
			converted_output += output1[i] + " ";
		}

		SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog;
		saveFileDialog1->InitialDirectory = "c:\\";
		saveFileDialog1->Filter = "Encoded Files (*.txt)|*.txt";
		saveFileDialog1->FilterIndex = 1;
		saveFileDialog1->RestoreDirectory = true;
		saveFileDialog1->Title = "Choose File Location";

		if (System::Windows::Forms::DialogResult::OK != saveFileDialog1->ShowDialog()) return;
		StreamWriter^ sw = gcnew StreamWriter(saveFileDialog1->FileName);
		try {
			sw->WriteLine(converted_output);
			sw->Close();
		}
		finally {
			sw->Close();
		}
	}
	private: System::Void compress_Click(System::Object^ sender, System::EventArgs^ e) {
		
		//        compress 

		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

		openFileDialog1->InitialDirectory = "c:\\";
		openFileDialog1->Filter = "XML files (*.xml)|*.xml";
		openFileDialog1->FilterIndex = 1;
		openFileDialog1->RestoreDirectory = true;
		openFileDialog1->Title = "Choose XML File";


		if (System::Windows::Forms::DialogResult::OK != openFileDialog1->ShowDialog()) return;
		string converted_input = msclr::interop::marshal_as< string >(openFileDialog1->FileName);


		Fcompress FC(converted_input);
		FC.save_as();
		FC.compress();


	}
private: System::Void decode_Click(System::Object^ sender, System::EventArgs^ e) {

	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->InitialDirectory = "c:\\";
	openFileDialog1->Filter = "Encoded files (*.txt)|*.txt";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;
	openFileDialog1->Title = "Choose Encoded File Location";


	if (System::Windows::Forms::DialogResult::OK != openFileDialog1->ShowDialog()) return;
	//string converted_input = msclr::interop::marshal_as< string >(openFileDialog1->FileName);
	StreamReader^ sr = gcnew StreamReader(openFileDialog1->FileName);
	InputBox->Text = nullptr;
	try {
		for (;;) {
			String^ line = sr->ReadLine();
			if (line == nullptr) break;
			while (line != nullptr) {
				InputBox->Text += line + "\n";
				line = sr->ReadLine();
			}
		}
	}
	finally {
		sr->Close();
	}
	string converted_input = msclr::interop::marshal_as< string >(InputBox->Text);
	stringstream output(converted_input);
	int number;
	std::vector<int> myNumbers;
	while (output >> number) {
		myNumbers.push_back(number);
	}
	String^ converted_output = gcnew String(decoding(myNumbers).c_str());
	//OutputBox->Text = myNumbers[0].ToString();
	OutputBox->Text = converted_output;
}
private: System::Void Decompress_Click(System::Object^ sender, System::EventArgs^ e) {

	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

	openFileDialog1->InitialDirectory = "c:\\";
	openFileDialog1->Filter = "Compressed files (*.txt)|*.txt";
	openFileDialog1->FilterIndex = 1;
	openFileDialog1->RestoreDirectory = true;
	openFileDialog1->Title = "Choose compressed File";


	if (System::Windows::Forms::DialogResult::OK != openFileDialog1->ShowDialog()) return;
	string converted_input = msclr::interop::marshal_as< string >(openFileDialog1->FileName);


	Fcompress FC(converted_input);
	FC.save_as_xml();
	FC.decompress();
}
};
};
