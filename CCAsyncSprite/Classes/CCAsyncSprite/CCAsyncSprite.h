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
    
    CCAsyncSprite(){};
    ~CCAsyncSprite();
    
    static CCAsyncSprite *createWithUrl(const char* pszUrl);
    
    virtual void operationDidFinish(NetworkOperation *operation);
    virtual void operationDidFail(NetworkOperation *operation);
    
    bool initWithUrl(const char* pszUrl);
    void setSpriteWithCCImage(CCImage *img);
    void setImage(const char *data, size_t size, const string &url);
    
    std::string m_strFileName;
};

#endif /* defined(__CCAsyncSprite__CCAsyncSprite__) */
