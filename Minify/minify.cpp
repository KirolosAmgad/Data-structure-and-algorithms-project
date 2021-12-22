	
/////////////////// O(n.m) 
////////	n numb of lines 
///////        m numb of char in each line 
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
