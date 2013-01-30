//
//  Request.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/24/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <boost/regex.hpp>
#include <sstream>

#include <unistd.h>

#include "Request.h"
#include "Headers.h"

using namespace std;
using namespace boost;

Request::Request(int socketConnection) {
    // Get method and url from the first line
    string firstLine = Headers::GetLine(socketConnection);
    
    cmatch fl_matches;
    regex flrx("([a-zA-Z]+) ([^ ]+) (HTTP/1.[01])");
    regex_search(firstLine.c_str(), fl_matches, flrx);
    
    method = fl_matches[1];
    url = fl_matches[2];
    
    // Get headers
    cmatch h_matches;
    regex hrx("([a-zA-Z0-9-]+):\\s*(.+)");
    string line;
    while (!(line = Headers::GetLine(socketConnection)).empty()) {
        regex_search(line.c_str(), h_matches, hrx);
        headers[h_matches[1]] = h_matches[2];
    }
    
    // Deal with special headers
    if (headers.count("Content-Length")) {
        int length;
        stringstream(headers["Content-Length"]) >> length;
        char body[length];
        read(socketConnection, body, length);
        this->body = body;
    }
    
    if (headers.count("Host")) {
        host = headers["Host"];
    }
}

string Request::getMethod() {
    return method;
}

string Request::getUrl() {
    return url;
}

string Request::getBody() {
    return body;
}

string Request::getHost() {
    return host;
}
