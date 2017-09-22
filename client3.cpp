//system header
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/un.h>
#include <sstream>
#include <stdlib.h>

//network header
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

using namespace std;
#define str_size 10

int main(){
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(12345);
    server_address.sin_addr.s_addr= inet_addr("127.0.0.1");
    
    char bufSend[str_size];
    char bufRecv[str_size];
    
        //create a socket
        int network_socket=socket(AF_INET,SOCK_STREAM,0);
        
        //call connect
        int connection_status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
        if(connection_status<0){
            cout<<"There is an error when connecting to server!"<<endl;
            cout << "Error number " << (int) errno << endl;
            exit(0);
        }
        cout << "Server connected!" << endl;
        
        //input
        cout<<"Input a String (or Control+D to stop the connection)! "<<endl;
    
    while(fgets(bufSend, sizeof(bufSend), stdin)){
        
        
        // call send() to send data to server;
        int send_success=send(network_socket, bufSend, sizeof(bufSend), 0);
        
        if(send_success<0){
            cout<<"No sending now!"<<endl;
        }
        //call reveive to receive data from server
    
        recv(network_socket, bufRecv, sizeof(bufRecv), 0);
        cout<<"Echoed message from server: " << bufRecv <<endl;
        
        //reset buffers
        memset(bufSend,0,str_size);
        memset(bufRecv,0,str_size);
        
        //close socket
        close(network_socket);
        
        //create a socket
        int network_socket=socket(AF_INET,SOCK_STREAM,0);
        
        //call connect
        int connection_status=connect(network_socket,(struct sockaddr*)&server_address,sizeof(server_address));
        if(connection_status<0){
            cout<<"There is an error when connecting to server!"<<endl;
            cout << "Error number " << (int) errno << endl;
            exit(0);
        }
        //cout << "Server connected!" << endl;
        
        //input
        cout<<"Input a String (or Control+D to shut off the connection)! "<<endl;
        
    }
    close(network_socket);
    cout<<"You shut off the connection!";
    
}