//
//  CCAsyncSprite.h
//  CCAsyncSprite
//
//  Created by mac on 13-7-23.
//
//

#ifndef __CCAsyncSprite__CCAsyncSprite__
#define __CCAsyncSprite__CCAsyncSprite__

#include <iostream>
#include "cocos2d.h"
#include "NetworkOperation.h"

USING_NS_CC;

class CCAsyncSprite :public CCSprite, public NetworkOperationDelegate  {
    
public:
    static CCAsyncSprite *createWithUrl(const char* pszUrl);
    
    virtual void operationDidFinish(NetworkOperation *operation);
    virtual void operationDidFail(NetworkOperation *operation);
    
    bool initWithUrl(const char* pszUrl);
    void setImageWithRawData(const char *data, size_t size, CCImage::EImageFormat format);
    void setImage(const char *data, size_t size, const string &url);
};

#endif /* defined(__CCAsyncSprite__CCAsyncSprite__) */
