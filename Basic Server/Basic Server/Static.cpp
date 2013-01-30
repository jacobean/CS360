//
//  Static.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/29/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include "Static.h"

using namespace std;

namespace Static {
    function<void(Request, Response)> load(string path) {
        return [path](Request req, Response res) {
            res.sendFile(path + req.getUrl());
        };
    }
}