//
//  Response.h
//  Basic Server
//
//  Created by Trevor Dixon on 1/26/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Basic_Server__Response__
#define __Basic_Server__Response__

#include <iostream>
#include <map>

#include "Request.h"

class Response {
    
public:
    
    Response(int, Request*);
    int responseCode = 200;
    Response* setHeader(std::string, std::string);
    Response* removeHeader(std::string);
    Response* send(std::string);
    Response* sendFile(std::string);
    Response* sendDirListing(std::string);
    Response* execute(std::string);
    Response* redirect(std::string);
    
private:
    
    int socket;
    Request* req;
    std::map<std::string, std::string> headers;
    std::vector<char *> CGIEnvironment(std::string);
    void sendHeaders();
    void sendHeaders(bool);
    
    static std::map<std::string, std::string> mime;
};


#endif /* defined(__Basic_Server__Response__) */

