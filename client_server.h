/*
Name : Abhijeet Panda
Roll No : 2018201044
*/
void file_request_handler(string client_ip, int client_port, string log_file){
    ofstream log(log_file);
    log<<"Created log file"<<endl;
    // Listening server
    // create socket
    int sockfd, newsockfd, portno, n;
    char buff[255];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        log<<"ERROR : "<<"Error opening socket"<<endl;
    }



    // BIND
    struct sockaddr_in serv_addr;
    bzero((char * ) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(client_port);

    // bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        log<<"ERROR : "<<"Binding Failed"<<endl;
    }

    // LISTEN
    listen:
    char choice;

    log<<"Listening on "<<client_ip<<":"<<client_port<<endl;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    listen(sockfd, 20);
    clilen = sizeof(cli_addr);


    // ACCEPT
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd<0){
        log<<"ERROR : "<<"Error on accept"<<endl;
    }
    else {
        string s;
        char buf[256];
        // Recieve the file name
        n = recv(newsockfd, buf, sizeof(buf), 0);
        s.append(buf, buf + n);
        log<<"Recieved : "<<s<<endl;
        ifstream ff(s);
        while (ff)
        {
            char cc [1024];//1048576 = 1024x1024 bytes
            memset(cc,0,sizeof(cc));
            ff.read(cc, sizeof(cc));
            n = send(newsockfd, cc, sizeof(cc), 0);
        }
        ff.close();
    }
    close(newsockfd);


    goto listen;
    close(sockfd);
    log.close();
}
