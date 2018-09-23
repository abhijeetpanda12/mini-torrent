/*
Name : Abhijeet Panda
Roll No : 2018201044
*/
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
#include <thread>

using namespace std;

#include "tracker_util.h"
#include "request_util.h"

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

    // bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        error("Binding failed");
    }

    // LISTEN
    listen:
    char choice;

    cout<<"Listening . . ."<<endl;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    listen(sockfd, 20);
    clilen = sizeof(cli_addr);


    // ACCEPT
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd<0){
        error("Error on accept");
    }
    thread t1(request_handler,newsockfd, seederlist_file);
    t1.detach();
    // communicate

    // request_handler(newsockfd, seederlist_file);

    goto listen;


    close(sockfd);
    return 0;
}
