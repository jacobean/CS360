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
#include <sys/stat.h>
#include <string.h>

#ifdef __linux__
#include <linux/limits.h>
#endif

#include "WebServer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // Get current working directory
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    
    // Find php-cgi
    char *path = getenv("PATH");
    char *phpcgi;
    bool phpfound = false;

    char *dir;
    struct stat path_stats;
    for (dir = strtok(path, ":"); dir; dir = strtok(NULL, ":")) {
        asprintf(&phpcgi, "%s/php-cgi", dir);
        if (stat(phpcgi, &path_stats) == 0) {
            phpfound = true;
            break;
        }
    }

    WebServer ws([=](Request req, Response res) {
        string extension = req.getExtension();
        string path = cwd + req.getResource();

        if (extension == "cgi" || extension == "pl") {
            res.execute(path);
        } else if (extension == "php") {
            if (phpfound) res.execute(phpcgi, path);
            else {
                res.responseCode = 500;
                res.send("PHP not found.");
            }
        } else {
            res.sendFile(path);
        }
    });
    
    ws.listen(8888);
    
    return 0;
}
