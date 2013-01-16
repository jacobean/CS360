//
//  main.cpp
//  Download
//
//  Created by Trevor Dixon on 1/7/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <iostream>
#include <vector>             // stl vector
#include <sys/types.h>   // Types used in sys/socket.h and netinet/in.h
#include <netinet/in.h>  // Internet domain address structures and functions
#include <sys/socket.h>  // Structures and functions used for socket API
#include <netdb.h>       // Used for domain/DNS hostname lookup
#include <unistd.h>
#include <errno.h>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <stdarg.h>
#include <string.h>

#include "Headers.h"

using namespace std;

void dprintf(const char *format, ...) {
    va_list arg;
    int done;
    
    cout << "\033[1;31mDebug info: \033[0m";
    
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    
    cout << "\n";
}

void print_usage() {
    cerr << "Usage: download [-d] host port resource\n";
}

int main(int argc, char **argv) {
    char * host;
    int port;
    char * resource;
    
    bool debug = false;
    
    char o;
    
    while ((o = getopt(argc, argv, "d")) != -1) {
        switch (o) {
                case 'd':
                    debug = true;
                    break;
                case '?':
                    exit(EXIT_FAILURE);
                    break;
        }
    }
    
    if ((!debug && argc != 4) || (debug && argc != 5)) {
        print_usage();
        exit(EXIT_FAILURE);
    }
    
    int i = optind;

    if (i < argc) host = argv[i++];
    else {
        print_usage();
        exit(EXIT_FAILURE);
    }
    
    if (i < argc) stringstream(argv[i++]) >> port;
    else {
        print_usage();
        exit(EXIT_FAILURE);
    }
    
    if (i < argc) resource = argv[i++];
    else {
        print_usage();
        exit(EXIT_FAILURE);
    }
    
    if (port < 1) {
        cerr << "Invalid port.\n";
        exit(EXIT_FAILURE);
    }
    
    if (debug) {
        dprintf("Debug output turned on");
        dprintf("Host: %s, Port: %d, Resource: %s", host, port, resource);
    }
    
    int socketHandle;
    
    if (debug) dprintf("Opening socket");
    if ((socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
        close(socketHandle);
        exit(EXIT_FAILURE);
    }
    if (debug) dprintf("Socket opened successfully");
    
    struct sockaddr_in remoteSocketInfo;
    struct hostent *hPtr;
    
    bzero(&remoteSocketInfo, sizeof(sockaddr_in));  // Clear structure memory
    
    // Get system information
    
    if (debug) dprintf("Getting host");
    if ((hPtr = gethostbyname(host)) == NULL) {
        cerr << "System DNS name resolution not configured properly." << endl;
        cerr << "Error number: " << ECONNREFUSED << endl;
        exit(EXIT_FAILURE);
    }
    if (debug) dprintf("Got host");
    
    // Load system information for remote socket server into socket data structures
    
    memcpy((char *) &remoteSocketInfo.sin_addr, hPtr->h_addr, hPtr->h_length);
    remoteSocketInfo.sin_family = AF_INET;
    remoteSocketInfo.sin_port = htons((u_short) port);      // Set port number
    
    if (debug) dprintf("Connecting to socket");
    if ((connect(socketHandle, (struct sockaddr *)&remoteSocketInfo, sizeof(sockaddr_in)) < 0)) {
        close(socketHandle);
        cerr << "Couldn't connect to " << host << " on port " << port << ".\n";
        exit(EXIT_FAILURE);
    }
    if (debug) dprintf("Successfully connected");

    int bytes, totalBytes;
    char *serverRequest;
    asprintf(&serverRequest, "GET %s HTTP/1.0\nHost: %s\r\n\r\n", resource, host);
    
    if (debug) dprintf("Sending request:\n\n%s", serverRequest);
    bytes = -1, totalBytes = 0;
    while (totalBytes < strlen(serverRequest)) {
        bytes = send( socketHandle, serverRequest+totalBytes, strlen(serverRequest), 0 );
        if (bytes == -1) {
            close(socketHandle);
            cerr << "No response.\n";
            exit(EXIT_FAILURE);
        } else totalBytes += bytes;
    }
    if (debug) dprintf("Request sent");
    
    if (debug) dprintf("Waiting for response");
    char *startline = Headers::GetLine(socketHandle);
    if (debug) dprintf("Got response. First line: %s", startline);
    
    vector<char *> headerLines;
    Headers::GetHeaderLines(headerLines, socketHandle, false);
    
    if (debug) dprintf("Headers received:\n");
    for (int i = 0; i < headerLines.size(); i++) {
        if (debug) cout << headerLines[i] << "\n";
    }
    
    if (debug) dprintf("End of headers");
    
    if (debug) dprintf("Reponse body: \n");
    
    #define MAX_MSG_SZ      1024

    int rval;
    char responseBody[MAX_MSG_SZ];
    while((rval = read(socketHandle, responseBody, MAX_MSG_SZ)) > 0) {
        write(1, responseBody, rval);
    }

    close(socketHandle);
    
    exit(EXIT_SUCCESS);
}

