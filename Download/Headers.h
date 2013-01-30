//
//  Headers.h
//  Download
//
//  Created by Trevor Dixon on 1/8/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Download__Headers__
#define __Download__Headers__

#include <vector>

namespace Headers {
    char* GetLine(int fds);
    void GetHeaderLines(std::vector<char *> &headerLines, int skt, bool envformat);
}

#endif /* defined(__Download__Headers__) */
