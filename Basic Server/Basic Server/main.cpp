//
//  main.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/16/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include <iostream>
#include <thread>
#include "WebServer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    WebServer ws([](Request req, Response res) {
        res.send("Hello there from cpp\n");
        //res.sendFile("/Users/tdixon/tmp/test.txt");
    });
    ws.listen(8888);
    
    return 0;
}

