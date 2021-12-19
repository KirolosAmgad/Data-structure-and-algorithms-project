
void indent(vector<string>& space, int& index, int NumOfSpaces)
	{
		for (int i = 0; i < NumOfSpaces; i++)
		{
			space[index] = space[index] + " ";
		}
		index++;
	}
