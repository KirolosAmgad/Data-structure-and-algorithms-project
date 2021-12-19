
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
