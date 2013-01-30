//
//  WebServer.h
//  Basic Server
//
//  Created by Trevor Dixon on 1/17/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Basic_Server__WebServer__
#define __Basic_Server__WebServer__

#include <iostream>
#include <netinet/in.h>

#include "CQueue.h"
#include "Request.h"
#include "Response.h"

class WebServer {
    
public:
    
    WebServer(std::function<void(Request, Response)> handler);
    void listen(int port);
    
private:
    
    int socketHandle;
    struct sockaddr_in socketInfo;
    int port;
    int threads = 5;
    CQueue q;
    
    std::function<void(Request, Response)> handler;
    
    void handleConnections();
    void handleRequest(int socketConnection, int t);
    
    void acceptConnections();
    void bindSocket();
    void initSocket();
};

#endif /* defined(__Basic_Server__WebServer__) */
