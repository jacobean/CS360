//
//  CQueue.h
//  Basic Server
//
//  Created by Trevor Dixon on 1/18/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#ifndef __Basic_Server__CQueue__
#define __Basic_Server__CQueue__

#include <iostream>
#include <queue>

#endif /* defined(__Basic_Server__CQueue__) */

class CQueue {
private:
    std::queue <int> q;
    std::mutex m;
    std::condition_variable cv;
    
public:
    void push(int s);
    int pop();
};