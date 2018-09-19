void request_handler(int newsockfd, string seederlist_file){
    // recieve request
    int n;
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
                sleep(10);
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

    // close socket
    close(newsockfd);
}
