//
//  CQueue.cpp
//  Basic Server
//
//  Created by Trevor Dixon on 1/18/13.
//  Copyright (c) 2013 Trevor Dixon. All rights reserved.
//

#include "CQueue.h"

using namespace std;

void CQueue::push(int s) {
    unique_lock<mutex> lock(m);
    q.push(s);
    lock.unlock();
    cv.notify_one();
}
    
int CQueue::pop() {
    unique_lock<mutex> lock(m);
    while (q.empty()) cv.wait(lock);
        
    int tmp = q.front();
    q.pop();
    return tmp;
}
