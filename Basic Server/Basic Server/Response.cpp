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

#ifdef __APPLE__
#include <sys/uio.h>
#else
#include <sys/sendfile.h>
#endif

#include <dirent.h>
#include <string.h>
#include <algorithm>

#include "Response.h"

using namespace std;

const char* nl = "\r\n";

Response::Response(int socket, Request req) {
    this->socket = socket;
    this->req = &req;
}

Response* Response::setHeader(string key, string value ) {
    headers[key] = value;
    return this;
}

Response* Response::removeHeader(string key) {
    headers.erase(key);
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
    struct stat path_stats;
    
    if (stat(path.c_str(), &path_stats) == 0) {
        // succes
        if (S_ISDIR(path_stats.st_mode)) {
            // is a directory, check for index.html
            struct stat idx_stats;
            if (stat((path + "index.html").c_str(), &idx_stats) == 0) {
                // index.html file exists
                return sendFile(path + "index.html");
            } else {
                // redirect to add trailing slash if necessary
                if (*req->getUrl().rbegin() != '/')
                    return redirect(req->getUrl() + "/");
                
                // list directory
                return sendDirListing(path);
            }
        } else {
            // is a file
            int fd = open(path.c_str(), O_RDONLY);
            
            string extension = req->getExtension();

            string fileType;
            if (mime.count(extension)) {
                fileType = mime[extension];
            } else {
                fileType = "text/plain";
            }
            
            setHeader("Content-Type", fileType);
            setHeader("Content-Length", to_string(path_stats.st_size));
            sendHeaders();
            off_t offset = 0;
#ifdef __APPLE__
            int s = ::sendfile(fd, socket, offset, &path_stats.st_size, nullptr, 0);
#else
            int s = ::sendfile(socket, fd, &offset, path_stats.st_size);
#endif
            
            ::close(fd);
            ::close(socket);
            
            return this;
        }
    } else {
        // stat call failed
        responseCode = 404;
        return send("Not found");
    }
    
    return this;
}

Response* Response::sendDirListing(string path) {
    string response;
    response += "<html><head><title>Directory listing for " + req->getUrl() + "</title></head>";
    response += "<body><h1>Directory listing for " + req->getUrl() + "</h1>";
    
    response += "<ul>";
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {

        while ((ent = readdir (dir)) != NULL) {
            response += string("<li><a href=\"") + ent->d_name + "\">" + ent->d_name + "</a></li>";
        }
        closedir (dir);
    } else {
        responseCode = 404;
        return send("Not found");
    }
    
    response += "</ul></body></html>";
    
    return send(response);
}

Response* Response::redirect(std::string url) {
    responseCode = 301;
    setHeader("Location", url);
    return send("<a href=\"" + url + "\">" + url + "</a>");
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

map<string, string> Response::mime = {
    {"aif", "audio/aiff"},
    {"aifc", "audio/aiff"},
    {"aiff", "audio/aiff"},
    {"avi", "video/avi"},
    {"bz", "application/x-bzip"},
    {"bz2", "application/x-bzip2"},
    {"c", "text/plain"},
    {"cc", "text/plain"},
    {"class", "application/java"},
    {"com", "application/octet-stream"},
    {"conf", "text/plain"},
    {"cpp", "text/x-c"},
    {"css", "text/css"},
    {"doc", "application/msword"},
    {"dot", "application/msword"},
    {"dv", "video/x-dv"},
    {"dvi", "application/x-dvi"},
    {"exe", "application/octet-stream"},
    {"gtar", "application/x-gtar"},
    {"gz", "application/x-gzip"},
    {"gzip", "application/x-gzip"},
    {"h", "text/plain"},
    {"hh", "text/plain"},
    {"hta", "application/hta"},
    {"htc", "text/x-component"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"htmls", "text/html"},
    {"htt", "text/webviewhtml"},
    {"htx", "text/html"},
    {"java", "text/plain"},
    {"jfif", "image/jpeg"},
    {"jpe", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"jpg", "image/jpeg"},
    {"js", "application/javascript"},
    {"latex", "application/x-latex"},
    {"log", "text/plain"},
    {"lsp", "application/x-lisp"},
    {"m", "text/plain"},
    {"m1v", "video/mpeg"},
    {"m2a", "audio/mpeg"},
    {"m2v", "video/mpeg"},
    {"m3u", "audio/x-mpequrl"},
    {"mid", "audio/midi"},
    {"midi", "audio/midi"},
    {"mp2", "audio/mpeg"},
    {"mp3", "audio/mpeg3"},
    {"mpeg", "video/mpeg"},
    {"mpg", "video/mpeg"},
    {"png", "image/png"},
    {"ppt", "application/powerpoint"},
    {"rtf", "application/rtf"},
    {"sh", "application/x-sh"},
    {"swf", "application/x-shockwave-flash"},
    {"tex", "application/x-tex"},
    {"text", "text/plain"},
    {"tgz", "application/x-compressed"},
    {"tif", "image/tiff"},
    {"tiff", "image/tiff"},
    {"txt", "text/plain"},
    {"word", "application/msword"},
    {"xls", "application/excel"},
    {"xml", "application/xml"},
    {"zip", "application/zip"},
    {"zsh", "text/x-script.zsh"}
};
