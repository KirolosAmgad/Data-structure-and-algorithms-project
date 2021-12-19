
void XML_indent(vector<string>& OutputSpaces, vector<string>& XML_Fixed, unsigned int SizeOfXML)
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
