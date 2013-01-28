//
//  Response.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/26/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

#include "Response.h"

using namespace std;

const char* nl = "\r\n";

Response::Response(int socket) {
    this->socket = socket;
}

Response* Response::setHeader(string key, string value ) {
    headers[key] = value;
    return this;
}

Response* Response::send(string body) {
    setHeader("Content-Length", to_string(body.size()));
    sendHeaders();
    
    ::send(socket, body.c_str(), body.size(), 0);
    ::close(socket);
    
    return this;
}

Response* Response::sendFile(string path) {
    struct stat src_stats;
    int src = open(path.c_str(), O_RDONLY);
    fstat(src, &src_stats);
    
    setHeader("Content-Length", to_string(src_stats.st_size));
    off_t offset = 0;
    off_t len = src_stats.st_size;
    int s = ::sendfile(src, socket, offset, &src_stats.st_size, nullptr, 0);
    
    ::close(src);
    ::close(socket);
    
    return this;
}

void Response::sendHeaders() {
    string intro = "HTTP/1.1 " + to_string(responseCode) + nl;
    ::send(socket, intro.c_str(), intro.length(), 0);
    
    for_each(headers.begin(), headers.end(), [this](pair<string, string> header){
        string out = header.first + ": " + headers[header.first] + nl;
        ::send(socket, out.c_str(), out.size(), 0);
    });
    
    ::send(socket, nl, strlen(nl), 0);
}