#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <stdio.h>

using namespace std;

#include "tracker_util.h"

void error(string err_msg){
    cout<<err_msg<<endl;
    exit(1);
}
int main(int argc, char const *argv[])
{

    // arguments
    if(argc<4){
        cout<<"Usage : ./tracker my_port <seederlist_file> <log_file>"<<endl;
        exit(1);
    }
    string seederlist_file = argv[2];
    string log_file = argv[3];

    // check
    string h = "aa9a37871ebe5298e34cb609d87f00546a1481a45";
    // string ipp = "127.0.0.1:8080";
    // share_file(seederlist_file,ipp,h);
    // remove_entry(seederlist_file,h);
    // vector <string> res = fetch_entry(seederlist_file,h);
    // cout<<"results"<<endl;
    // for (int i = 0; i < res.size(); ++i)
    // {
    //     cout<<">>>"<<res[i]<<"<<<"<<endl;
    //     /* code */
    // }
    return 0;




    // create socket
    int sockfd, newsockfd, portno, n;
    char buff[255];


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        error("Error opening socket\n");
    }



    // BIND
    struct sockaddr_in serv_addr;
    bzero((char * ) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Binding failed");
    }

    // LISTEN
    listen:
    char choice;

    cout<<"Listening . . ."<<endl;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);


    // ACCEPT
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd<0){
        error("Error on accept");
    }
    // communicate

    // recieve request
    string s;
    char buf[256];
    n = recv(newsockfd, buf, sizeof(buf), 0);
    s.append(buf, buf + n);


    // process request
    string message;
    vector<string> req = split_str(s);
    if(req[0].compare("fetch")==0)
        message = "got a fetch request";
    else if(req[0].compare("share")==0)
        message = "got a share request";
    else if(req[0].compare("remove")==0)
        message = "got a remove request";
    else
        message = "Invalid Request";


    // answer request
    n = send(newsockfd, message.data(), message.size(), 0);

    close(newsockfd);
    goto listen;


    close(sockfd);
    return 0;
}
