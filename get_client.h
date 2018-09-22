void get_file(string ip_port, string file_request, string new_file){
    string ip;
    int portno;
    size_t pos = ip_port.find(":");
    if(pos!=std::string::npos){
        string temp = ip_port.substr(pos+1);
        portno = stoi(temp);
        ip = ip_port.substr(0,pos);
    }else{
            cout<<"Invalid IP and PORT NO"<<endl;
            return;
        }


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
    n = send(sockfd, file_request.data(), file_request.size(), 0);
    if(n<0)
        cout<<"error in sending from client"<<endl;
    else
        cout<<"sent : "<<file_request.data()<<endl;
    string s;
    char buf[1024];
    // n = recv(sockfd, buf, sizeof(buf), 0);
    while((n = recv(sockfd, buf, sizeof(buf), 0)) > 0)
        s.append(buf, buf + n);
    // cout<<s<<endl;
    ofstream ff(new_file);
    ff<<s;
    ff.close();
    close(sockfd);
}
