vector<string> split_str(string s){
    vector<string> c;
    size_t pos = 0;
    string token;
    while ((pos = s.find(" ")) != std::string::npos) {
        token = s.substr(0, pos);
        c.push_back(token);
        s.erase(0, pos + 1);
    }
    c.push_back(s);

    return c;
}

vector<string> split_str_tab(string s){
    vector<string> c;
    size_t pos = 0;
    string token;
    while ((pos = s.find("\t")) != std::string::npos) {
        token = s.substr(0, pos);
        c.push_back(token);
        s.erase(0, pos + 1);
    }
    c.push_back(s);

    return c;
}

void share_file(string seeder_file, string ip, string hash, string name){
    ofstream seedr;
    seedr.open(seeder_file, ios_base::app);
    seedr<<ip<<"\t"<<hash<<"\t"<<name<<endl;
    seedr.close();
}


void remove_entry(string seeder_file, string entry){
    string line;
    ifstream fin;
    fin.open(seeder_file);
    ofstream temp;
    temp.open("temp.txt");

    while (getline(fin,line)){
        size_t pos =0;
        pos = line.find(entry);
        if(pos == std::string::npos)
            temp<<line<<endl;
    }
    temp.close();
    fin.close();
    remove(seeder_file.c_str());
    rename("temp.txt",seeder_file.c_str());
}


vector<string> fetch_entry(string seeder_file, string entry){
    vector<string> v;
    string line;
    ifstream fin;
    fin.open(seeder_file);
    vector <string> data;
    while (getline(fin,line)){
        size_t pos =0;
        pos = line.find(entry);
        if(pos != std::string::npos){
            data = split_str_tab(line);
            // line.replace(pos-1,string::npos,"");
            v.push_back(data[0]+"|"+data[data.size()-1]);
            // v.push_back(data[data.size()-1]);
        }
    }
    fin.close();
    return v;
}
