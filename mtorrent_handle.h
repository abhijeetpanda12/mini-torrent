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

int create_m_torrent(string file_path, string filename, string ip,int port, string ip_port){
    /*
    the listen port is different for the clients. change it while sending to server. dont send server listen port.
    */
    unsigned char hash[SHA_DIGEST_LENGTH];
    ifstream ff(file_path);
    int count=0;
    string h="";
    while (ff)
    {
        char cc [1048576];//1048576 = 1024x1024 bytes
        memset(cc,0,sizeof(cc));
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


    // share to tracker
    string hsh = create_sha1_string(h);
    string message = "share "+ip_port+" "+hsh+" "+file_path;
    cout<<"IP : "<<ip<<endl;
    cout<<"msg : "<<message<<endl;
    string msg = send_tracker(message, ip, port);
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
