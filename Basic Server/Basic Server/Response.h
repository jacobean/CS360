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

class Response {
    
public:
    
    Response(int socket);
    int responseCode = 200;
    Response* setHeader(std::string key, std::string value);
    Response* send(std::string);
    Response* sendFile(std::string);
    
private:
    
    int socket;
    std::map<std::string, std::string> headers;
    void sendHeaders();
    
};


#endif /* defined(__Basic_Server__Response__) */

