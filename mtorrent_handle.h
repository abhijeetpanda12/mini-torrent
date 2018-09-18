string create_sha1_string(string s){
    string h="";
    unsigned char hash[SHA_DIGEST_LENGTH];
    char * cstr = new char [s.length()+1];
    strcpy (cstr, s.c_str());
    unsigned char* c = reinterpret_cast<unsigned char*>(cstr);
    char buff[4];
    SHA1(c, sizeof(c), hash);
    for (int i = 0; i < 20; i++) {
        snprintf(buff,sizeof(buff),"%02x", hash[i]);
        h+=buff;
    }
    return h;
}

int create_m_torrent(string file_path, string filename){
    unsigned char hash[SHA_DIGEST_LENGTH];
    ifstream ff(file_path);
    int count=0;
    string h="";
    while (ff)
    {
        char cc [1000000];
        ff.read(cc, sizeof(cc));
        unsigned char* c = reinterpret_cast<unsigned char*>(cc);
        char buff[4];
        SHA1(c, sizeof(cc), hash);
        for (int i = 0; i < 20; i++) {
            snprintf(buff,sizeof(buff),"%02x", hash[i]);
            h+=buff;
        }
        count++;
    }

    ff.close();
    ofstream mtor(filename+".mtorrent");
    mtor<<"trackerIP:port"<<endl;
    mtor<<"trackerIP:port"<<endl;
    mtor<<file_path<<endl;
    mtor<<h;
    mtor.close();
    cout<<"File Split into "<<count<<" pieces."<<endl;
    return 0;
}

string get_sha1(string filename){
    ifstream ff(filename);
    string s,h;
    for(int i=0;i<4;i++){
        getline(ff, s);
        if (i==3)
            h=create_sha1_string(s);
    }
    return h;
}
// string get_ip_from_file(string filename, string hash){

// }
