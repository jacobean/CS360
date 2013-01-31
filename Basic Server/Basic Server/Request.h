//
//  Request.h
//  Basic Server
//
//  Created by Trevor Dixon on 1/24/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Basic_Server__Request__
#define __Basic_Server__Request__

#include <iostream>
#include <map>

class Request {
    
public:
    
    Request(int socketConnection);
    std::string getMethod();
    std::string getUrl();
    std::string getBody();
    std::string getHost();
    std::string getExtension();
    
private:
    
    std::string method;
    std::string url;
    std::string host;
    std::map<std::string, std::string> headers;
    std::string body;
};

#endif /* defined(__Basic_Server__Request__) */


