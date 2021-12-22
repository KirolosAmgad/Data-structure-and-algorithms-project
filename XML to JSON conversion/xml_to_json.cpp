
static inline void ltrim(string& s) {             //////////////////O(n)///////////////////
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !isspace(ch);
			}));
	}
	static inline void rtrim(string& s) {          //////////////////O(n)///////////////////
		s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !isspace(ch);
			}).base(), s.end());
	}

	static inline void trim(string& s) {         //////////////////O(n)///////////////////
		ltrim(s);
		rtrim(s);
	}
	int getDuplicate(vector<string> &v, string &key) {              //////////////////O(n)///////////////////

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

		/* string beautify() {
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

		}   */




		/////////////////////////////////////conversion///////////////////////////////////////////////////////
		string convert_to_json() {            //////////////////O(n)///////////////////
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

	GeneralTree createTree(string xmlCode) {       //////////////////O(n)///////////////////

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

	string xml_to_json(string xmlCode) {       //////////////////O(n)///////////////////
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
