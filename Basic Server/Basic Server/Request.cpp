//
//  Request.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/24/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <sstream>
#include <unistd.h>

#include "Request.h"
#include "Headers.h"

using namespace std;

#ifdef __llvm__
#include <regex>
#else
#include <boost/regex.hpp>
using namespace boost;
#endif

Request::Request(int socketConnection, int port) {
    // Get method and url from the first line
    this->port = port;
    string firstLine = Headers::GetLine(socketConnection);
    
    cmatch fl_matches;
    regex flrx("([a-zA-Z]+) ([^ ]+) (HTTP/1.[01])");
    regex_search(firstLine.c_str(), fl_matches, flrx);
    
    method = fl_matches[1];
    url = fl_matches[2];
    serverProtocol = fl_matches[3];
    
    // Get query string
    size_t i = url.find("?");
    bool hasQS = i != string::npos;
    resource = url.substr(0, i);
    queryString = hasQS ? url.substr(i+1) : "";
    
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
        char body[length+1];
        read(socketConnection, body, length);
        body[length] = '\0';
        this->body = body;
    }
    
    if (headers.count("Host")) {
        host = headers["Host"];
    }
}

int Request::getPort() { return port; }
string Request::getMethod() { return method; }
string Request::getUrl() { return url; }
string Request::getServerProtocol() { return serverProtocol; }
string Request::getResource() { return resource;  }
string Request::getQueryString() { return queryString; }
string Request::getBody() { return body; }
string Request::getHost() { return host; }

string Request::getExtension() {
    string extension;
    string::size_type idx = resource.rfind('.');
    
    if (idx != string::npos) {
        extension = resource.substr(idx + 1);
    }

    return extension;
}
