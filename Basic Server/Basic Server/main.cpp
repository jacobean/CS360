//
//  main.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/16/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <iostream>
#include <thread>
#include <unistd.h>

#ifdef __linux__
#include <linux/limits.h>
#endif

#include "WebServer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    
    WebServer ws([=](Request req, Response res) {
        string extension = req.getExtension();

        if (extension == "cgi" || extension == "pl") {
            res.execute(cwd + req.getResource());
        } else {
            res.sendFile(cwd + req.getUrl());            
        }
    });
    
    ws.listen(8888);
    
    return 0;
}
