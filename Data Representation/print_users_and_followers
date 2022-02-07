struct follow_stuct{
    private:
        int my_id;
        vector<int> followers_id;
    
    public:
        follow_stuct(int my_id, vector<int> followers_id){
            this->my_id = my_id;
            this->followers_id = followers_id;
        }
        void set_my_id(int my_id){this->my_id = my_id;}
        void set_followers_id(vector<int> followers_id){this->followers_id = followers_id;}
        int get_my_id(){return this->my_id;}
        vector<int> get_followers_id(){return this->followers_id;}
};



string* id_to_names(string xml){

    vector<string> users_vec = SearchFor(xml, "user");
    string* names = new string[users_vec.size()+1];
    //string* names = new string[9999999];

    for(auto f:users_vec){
        vector<string> names_vec = SearchFor(f, "name");
        string curr_name = SearchFor(f, "name")[0];
        int curr_id = stoi(SearchFor(f, "id")[0]);
        //cout << curr_id << " -> " << curr_name << endl; 
        names[curr_id] = curr_name;
    }

    return names;
}

string print_users_and_followers_version_2(vector<follow_stuct> follow_stucts, string xml){
    
    string* names = id_to_names(xml);
    string str = "";
    
    for(auto f:follow_stucts){
        str += names[f.get_my_id()] + " (id:";
        str += to_string(f.get_my_id());
        str += ") ----------> {";
        
        // for a better printing format
        for(int i=0; i<f.get_followers_id().size(); i++){
            if(i == f.get_followers_id().size()-1) {
                str += names[f.get_followers_id()[i]] + " (id:"; 
                str += to_string(f.get_followers_id()[i]) + ")}\n\n";
            }
            else { 
                str += names[f.get_followers_id()[i]] + " (id:";
                str += to_string(f.get_followers_id()[i]) + "),";
            }
        }
    }
    return str;

}
