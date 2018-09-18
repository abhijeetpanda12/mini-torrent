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
    /*
    fetch hash ====> vector<string>
    share IP:PORT hash file_name
    remove hash
    */
    string message;
    vector<string> req = split_str(s);
    if(req[0].compare("fetch")==0){
            // message = "got a fetch request";
        if(req.size()<1)
            message="Usage : fetch hash";
        else{
            vector<string> msg = fetch_entry(seederlist_file, req[1]);
            for (int i = 0; i < msg.size(); ++i)
                {
                    message = message + msg[i] + ";";
                    /* code */
                }
            }
        }
    else if(req[0].compare("share")==0){
            if(req.size()<3)
                message="Usage : share IP:PORT hash file_name";
            else{
                share_file(seederlist_file, req[1], req[2],req[3]);
                message="DONE! share";
                }
        }
    else if(req[0].compare("remove")==0){
            if(req.size()<1)
                message="Usage : remove hash";
            else{
                remove_entry(seederlist_file,req[1]);
                message="DONE! remove";
            }
        }
    else
        message = "Invalid Request";


    // answer request
    n = send(newsockfd, message.data(), message.size(), 0);

    close(newsockfd);
    goto listen;


    close(sockfd);
    return 0;
}
