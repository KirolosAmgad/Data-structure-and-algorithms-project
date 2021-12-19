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
