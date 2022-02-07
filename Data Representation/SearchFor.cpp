vector<string> SearchFor(const string &xml, string tag)
{
    vector<string> collection;
    long pos = 0, start = 0;

    while (true)
    {
        start = xml.find("<" + tag, pos);
        if (start == string::npos)
            return collection;
        start = xml.find(">", start);
        start++;

        pos = xml.find("</" + tag, start);
        if (pos == string::npos)
            return collection;
        collection.push_back(xml.substr(start, pos - start));
    }
}
