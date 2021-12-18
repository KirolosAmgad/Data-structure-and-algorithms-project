
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
