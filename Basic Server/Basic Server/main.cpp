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
#include "Static.h"

using namespace std;

int main(int argc, const char * argv[]) {
    WebServer ws(Static::load("hmmmm"));
    ws.listen(8888);
    
    return 0;
}

/*
 [](Request req, Response res) {
 cout << "REQUEST" << endl;
 res.setHeader("Content-Type", "application/json");
 res.send("Web server");
 //res.sendFile("/Users/tdixon/tmp/test.txt");
 }
*/