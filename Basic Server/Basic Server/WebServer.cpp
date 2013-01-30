//
//  WebServer.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/17/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <iostream>
#include <thread>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "WebServer.h"

using namespace std;

void WebServer::handleConnections() {
    int socketConnection;
    while ((socketConnection = accept(socketHandle, NULL, NULL)) > 0) {
        q.push(socketConnection);
    }
    
    close(socketHandle);
}

void WebServer::handleRequest(int socketConnection, int t) {
    Request req(socketConnection);
    Response res(socketConnection, req);
    
    handler(req, res);
}

WebServer::WebServer(std::function<void(Request, Response)> handler) {
    this->handler = handler;
    initSocket();
}

void WebServer::listen(int port) {
    this->port = port;
    bindSocket();
    acceptConnections();
}

void WebServer::acceptConnections() {
    thread mainThread(&WebServer::handleConnections, this);
    thread tarr [5];
    
    for (int i = 0; i < threads; i++) {
        tarr[i] = thread([=] {
            while (true) {
                int socketConnection = q.pop();
                handleRequest(socketConnection, i);
            }
        });
    }
    
    mainThread.join();
}

void WebServer::bindSocket() {
    socketInfo.sin_port = htons(port);        // Set port number

    // Bind the socket to a local socket address
    if( ::bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(struct sockaddr_in)) < 0) {
        close(socketHandle);
        perror("bind");
        return;
    }
    
    ::listen(socketHandle, 1);
}

void WebServer::initSocket() {
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
        close(socketHandle);
        return;
    }
    
    int yes = 1;
    setsockopt (socketHandle, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    
    #define MAXHOSTNAME 256
    
    char sysHost[MAXHOSTNAME+1];  // Hostname of this computer we are running on
    struct hostent *hPtr;
    
    bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory
    
    // Get system information
    gethostname(sysHost, MAXHOSTNAME);  // Get the name of this computer we are running on
    if((hPtr = gethostbyname(sysHost)) == NULL) {
        cerr << "System hostname misconfigured." << endl;
        //exit(EXIT_FAILURE);
    }
    
    // Load system information into socket data structures
    socketInfo.sin_family = AF_INET;
    
    // Use any address available to the system. This is a typical configuration for a server.
    // Note that this is where the socket client and socket server differ.
    // A socket client will specify the server address to connect to.
    socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Translate long integer to network byte order.
}
