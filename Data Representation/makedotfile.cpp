void makedotfile(vector<follow_struct> followstructs, string xml) {
		string* names = id_to_names(xml);
		ofstream myfile;
		myfile.open("nodes.dot");
		myfile << "digraph list {\n node [ shape=\"record\" color = \"blue\"] \n";
		for (auto f : followstructs) {
			myfile << to_string(f.get_my_id()) + "[label= \"{" + names[f.get_my_id()] + "|id:" + to_string(f.get_my_id())+ "}\" ]";
			
			myfile << "\n";
		}
		for (auto f : followstructs) {
			myfile << to_string(f.get_my_id()) + "-> {";

			// for a better printing format
			for (int i = 0; i < f.get_followers_id().size(); i++) {
				if (i == f.get_followers_id().size() - 1) myfile << to_string( f.get_followers_id()[i]) + "}";
				else myfile << to_string(f.get_followers_id()[i]) + ", ";
			}
			myfile << "\n" ;
		}
		myfile << "}";
		myfile.close();
		system("dot -Tpng -O nodes.dot");
		system("nodes.dot.png");
	}
