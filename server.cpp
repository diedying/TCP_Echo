//system header
#include <iostream>
#include <string>
#include <stdio.h>
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

//define不用加分号
#define str_size 10

//child process
void doprocess(int socket){
    int len;
    char str[str_size];
    memset(str, 0, str_size);
    
    //cout << "One client has connected to server." << endl;
    
    //Receive from and send message back to client 
    len = recv(socket, str, str_size, 0);
    if(len < 0){
        cout << "Unable to receive message from client..." << endl;
        _exit(1);
    }else if(len == 0){
        cout << "A connection has been shut off." << endl;
        _exit(1);
    }else{
        cout << "Message from client: " << str << endl;
    }
    send(socket, str, len, 0);
    if(len < 0){
        cout << "Unable to send message to client..." << endl;
        _exit(1);
    }
}

int main(){
    
    //==============initialize socket==================//
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        
    if(server_socket < 0 ){ 
        cout << "Could not establish connection" << endl;
        exit(0);
    }
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0){
        cout << "setsockopt(SO_REUSEADDR) failed" << endl;
    }
    
    cout << "Server socket established." << endl;
    
    //============bind socket to ip and port===============//
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(12345);
    socklen_t server_addr_size = sizeof(server_addr);
    
    if( bind(server_socket, (struct sockaddr* ) &server_addr, server_addr_size) < 0){
        cout << "Unable to bind socket." << endl;
        cout << "Error number " << (int) errno << endl;
        exit(0);
    }
    cout << "Socket binded." << endl;

    //=============listen to the client============//

    if(listen(server_socket, 10)< 0){
        cout << "Unable to find client." << endl;
        cout << "Error number " << (int) errno << endl;
        exit(0);
    }
    cout << "Listening to the cilent..." << endl;

    char received_str[str_size]; 
    //===========accept the client================//
    while(true){    
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        
        if(client_socket < 0){
            cout << "Unable to accept client" << endl;
            cout << "Error number " << (int) errno << endl;
            exit(0);
        }
        
        int pid = fork();
        
        if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         //Multiclient process
         doprocess(client_socket);
         exit(0);
      }
      else {
         close(client_socket);
      }
        
    }
    return 0;
    
}




// int length = recv(client_socket, received_str, str_size, 0);
// if(length > 0){
//     cout << "Message from client: " << received_str << endl;
// }
// //send the message back to client
// if(length == 0){
//     cout << "The connection has been shut off." << endl;
//     break;
// }
        
// send(client_socket, received_str, length, 0);
// memset(received_str, 0, str_size);





//Close the sockets.
//close(server_socket);
//close(client_socket);
