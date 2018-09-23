/*
Name : Abhijeet Panda
Roll No : 2018201044
*/
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
#include <thread>
#include <set>
using namespace std;


#include "tracker_socket.h"
#include "mtorrent_handle.h"
#include "client_server.h"
#include "get_client.h"

vector<string> get_command();

int main(int argc, char const *argv[])
{
    // int i;
    if(argc<=4){
        cout<<"Usage ​./client <CLIENT_IP>:<UPLOAD_PORT> <TRACKER_IP_1>:<TRACKER_PORT_1> <TRACKER_IP_2>:<TRACKER_PORT_2> <log_file>"<<endl;
        exit(1);
    }
    // GET the tracker IP and PORT
    // client upload port
    string client_ip_port = argv[1];
    string client_ip;
    int client_port;
    size_t client_pos = client_ip_port.find(":");
    if(client_pos!=std::string::npos){
        string temp = client_ip_port.substr(client_pos+1);
        client_port = stoi(temp);
        client_ip = client_ip_port.substr(0,client_pos);
    }else{
            cout<<"Invalid IP and PORT NO"<<endl;
            return 0;
        }

    //tracker 1
    string tracker_ip_port = argv[2];
    string ip;
    int port;
    size_t pos = tracker_ip_port.find(":");
    if(pos!=std::string::npos){
        string temp = tracker_ip_port.substr(pos+1);
        port = stoi(temp);
        ip = tracker_ip_port.substr(0,pos);
    }else{
            cout<<"Invalid IP and PORT NO"<<endl;
            return 0;
        }

    // tracker 2

    //log file
    string log_file = argv[4];
    // cout<<ip<<endl;
    // cout<<port;
    // cout<<tracker_ip_port;

    /* MENU
        1. Sharing a Local File.
        2. Download a remote file.
        3. Show Downloads.
        4. Removing a Shared File.
        5. Close application.
    */
    // open the listen server for sending files
    thread t_c_server(file_request_handler, client_ip, client_port, log_file);
    t_c_server.detach();

    // menu driven interface
    vector<string> command;
    string db_file="file_list.txt";

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
                set<string> s( v.begin(), v.end() );
                v.assign( s.begin(), s.end() );
                v.erase(v.begin());
                cout<<v.size()<<endl;
                for (int i = 0; i < v.size(); ++i)
                {
                    cout<<"File Source IP : "<<v[i]<<endl;
                }
                bool flag=false;
                while(!flag){
                vector<string> ip_name = split_str_pipe(v[0]);// change v[0]
                cout<<ip_name[0]<<endl<<ip_name[1]<<endl;
                get_file(ip_name[0], ip_name[1], command[2]);
                flag=true;
                }
                ofstream file_db(db_file);
                file_db<<command[2]<<endl;
                file_db.close();
                // cout<<recv_ip<<endl;
            }
        }
        else if (command[0].compare("test")==0){
            cout<<"testing"<<endl;
            test_socket(ip, port);
        }
        else if (command[0].compare("show")==0 && command[1].compare("downloads")==0){
            cout<<"showing downloads"<<endl;
            string file;
            ifstream file_db(db_file);
            while(getline(file_db,file)){
                cout<<"[S]"<<file<<endl;
            }
            file_db.close();
        }
        else if (command[0].compare("remove")==0){
            cout<<"Do a remove"<<endl;
            if (command.size()<2)
                cout<<"Usage : remove <path to .mtorrent file>"<<endl;
            else{
                string hsh = get_sha1(command[1]);
                send_tracker("remove "+hsh, ip, port);
            }

        }
        else if (command[0].compare("quit")==0){
            cout<<"Quitting!"<<endl;
            if(t_c_server.joinable())
                t_c_server.join();
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
