//
//  Static.h
//  Basic Server
//
//  Created by Trevor Dixon on 1/29/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Basic_Server__Static__
#define __Basic_Server__Static__

#include <iostream>
#include <functional>

#include "Request.h"
#include "Response.h"

namespace Static {
    std::function<void(Request, Response)> load(std::string path);
}

#endif /* defined(__Basic_Server__Static__) */
