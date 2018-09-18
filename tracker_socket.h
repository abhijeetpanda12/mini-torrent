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
    string msg = send_tracker("fetch me the boy", ip, port);
    cout<<"GOT : "<<msg<<endl;
    cout<<"NEW message"<<endl;

    msg = send_tracker("share me the boy", ip, port);
    cout<<"GOT : "<<msg<<endl;
    cout<<"NEW message"<<endl;

    msg = send_tracker("remove me the boy", ip, port);
    cout<<"GOT : "<<msg<<endl;
    cout<<"NEW message"<<endl;

    msg = send_tracker("get me the boy", ip, port);
    cout<<"GOT : "<<msg<<endl;
    cout<<"NEW message"<<endl;

}


string get_ip_from_file(string file, string hash){
    // string msg = send_tracker("HI", ip, port);
    // cout<<"recieved : ";
    return "hi";
}
