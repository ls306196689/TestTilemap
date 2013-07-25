//
//  HelpFunctions.h
//  TestTilemap
//
//  Created by apple on 13-7-24.
//
//

#ifndef __TestTilemap__HelpFunctions__
#define __TestTilemap__HelpFunctions__

#include <iostream>
template <class T>
//if value is less than lowlimit, 
//void checkLimit(T *value,T lowlimit, T uplimit);

void checkLimit(T *value,T lowlimit, T uplimit)
{
    if (*value < lowlimit) {
        *value = lowlimit;
    }
    if (*value > uplimit) {
        *value = uplimit;
    }
}

#define CCLOG_POINT(p) CCLOG("x:%f,y:%f",(p).x,(p).y)

#define CCLOG_D(d) CCLOG("%d",(d));

#define CCLOG_X(x) CCLOG("%x",(x));



#define CCLOG_F(f) CCLOG("%f",(f));

#define CCLOG_SIZE(s) CCLOG("width:%f,height:%f",(s).width,(s).height)

#endif /* defined(__TestTilemap__HelpFunctions__) */
