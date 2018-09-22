vector<string> split_str_semi(string s){
    vector<string> c;
    size_t pos = 0;
    string token;
    while ((pos = s.find(";")) != std::string::npos) {
        token = s.substr(0, pos);
        c.push_back(token);
        s.erase(0, pos + 1);
    }
    c.push_back(s);

    return c;
}

vector<string> split_str_pipe(string s){
    vector<string> c;
    size_t pos = 0;
    string token;
    while ((pos = s.find("|")) != std::string::npos) {
        token = s.substr(0, pos);
        c.push_back(token);
        s.erase(0, pos + 1);
    }
    c.push_back(s);

    return c;
}

string send_tracker(string message, string ip, int portno)
{
    int sockfd, newsockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    //socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        cout<<"Error opening socket"<<endl;
    }

    server = gethostbyname(ip.c_str());
    if(server == NULL){
        fprintf(stderr, "Error No such Host\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char * )server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);


    // connect
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        cout<<"Connectin Failed"<<endl;
    }


    //communicate
    n = send(sockfd, message.data(), message.size(), 0);
    if(n<0)
        cout<<"error in sending from client"<<endl;
    else
        cout<<"sent : "<<message.data()<<endl;
    string s;
    char buf[256];

    n = recv(sockfd, buf, sizeof(buf), 0);
    // while((n = recv(sockfd, buf, sizeof(buf), 0)) > 0)
        s.append(buf, buf + n);

    close(sockfd);
    return s;
}


void test_socket(string ip, int port){
    // cout<<"IP : "<<ip<<endl;
    // cout<<"PORT : "<<port<<endl;
    //working
    // string msg = send_tracker("fetch aa9a37871ebe5298e34cb609d87f4546a1481a45", ip, port);
    // cout<<"GOT : "<<msg<<endl;
    // cout<<"NEW message"<<endl;
    // vector<string> v = split_str_semi(msg);
    // for (int i = 0; i < v.size(); ++i)
    // {
    //     cout<<v[i]<<endl;
    // }

    //working
    // string msg = send_tracker("share "+ip+" "+"aa9a37871ebe5298e34cb609d87f4546a1481a45"+" "+"a.txt", ip, port);
    // cout<<"GOT : "<<msg<<endl;
    // cout<<"NEW message"<<endl;

    //working
    // string msg = send_tracker("remove aa9a37871ebe5298e34cb609d87f01546a1481a45", ip, port);
    // cout<<"GOT : "<<msg<<endl;
    // cout<<"NEW message"<<endl;

    //working
    // string msg = send_tracker("get me the boy", ip, port);
    // cout<<"GOT : "<<msg<<endl;
    // cout<<"NEW message"<<endl;

}


string get_ip_of_file(string hash, string ip, int port){
    string message = "fetch "+hash;
    string msg = send_tracker(message, ip, port);
    cout<<"GOT : "<<msg<<endl;
    cout<<"NEW message"<<endl;
    return msg;
}
