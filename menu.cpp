#include <iostream>
#include <string>
#include <vector>
#include <openssl/sha.h>
#include <fstream>

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;


#include "tracker_socket.h"
#include "mtorrent_handle.h"

vector<string> get_command();

int main(int argc, char const *argv[])
{
    // int i;
    if(argc<=4){
        cout<<"Usage ​./client <CLIENT_IP>:<UPLOAD_PORT> <TRACKER_IP_1>:<TRACKER_PORT_1> <TRACKER_IP_2>:<TRACKER_PORT_2> <log_file>"<<endl;
        exit(1);
    }


    // GET the IP and PORT NO
    string client_ip_port = argv[1];
    string ip;
    int port;
    size_t pos = client_ip_port.find(":");
    if(pos!=std::string::npos){
        string temp = client_ip_port.substr(pos+1);
        port = stoi(temp);
        ip = client_ip_port.substr(0,pos);
    }else
        cout<<"Invalid IP and PORT NO"<<endl;
    // cout<<ip<<endl;
    // cout<<port;
    // cout<<client_ip_port;

    /* MENU
        1. Sharing a Local File.
        2. Download a remote file.
        3. Show Downloads.
        4. Removing a Shared File.
        5. Close application.
    */
    vector<string> command;
    while(1){
        command = get_command();
        if (command[0].compare("share")==0){
            cout<<"Do a share"<<endl;
            if (command.size()<3)
                cout<<"Usage : share <local file path> <filename>"<<endl;
            else{
                if (create_m_torrent(command[1],command[2],ip,port,client_ip_port)!=0)
                    cout<<"Error in sharing the file";
            }
        }
        else if (command[0].compare("get")==0){
            cout<<"Do a download"<<endl;
            if (command.size()<3)
                cout<<"Usage : get <path to .mtorrent file> <destination path>"<<endl;
            else{
                string hash = get_sha1(command[1]);
                cout<<"HASH : "<<hash<<endl;
                string recv_ip = get_ip_of_file(hash,ip,port);
                vector<string> v = split_str_semi(recv_ip);
                for (int i = 0; i < v.size(); ++i)
                {
                    cout<<"File Source IP : "<<v[i]<<endl;
                }
                // cout<<recv_ip<<endl;
            }
        }
        else if (command[0].compare("test")==0){
            cout<<"testing"<<endl;
            test_socket(ip, port);
        }
        else if (command[0].compare("show")==0 && command[1].compare("downloads")==0){
            cout<<"showing downloads"<<endl;
        }
        else if (command[0].compare("remove")==0){
            cout<<"Do a remove"<<endl;
        }
        else if (command[0].compare("quit")==0){
            cout<<"Quitting!"<<endl;
            return 0;
        }
        else
            cout<<"Invalid Command"<<endl;
    }

    for (int i = 0; i < command.size(); ++i)
    {
        /* code */
        cout<<command[i]<<endl;
    }
    return 0;
}


vector<string> get_command(){
    cout<<"Enter your command"<<endl;
    string s;
    getline(cin, s);
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
