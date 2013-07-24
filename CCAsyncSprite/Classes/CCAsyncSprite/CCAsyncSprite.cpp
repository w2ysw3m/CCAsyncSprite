//
//  CCAsyncSprite.cpp
//  CCAsyncSprite
//
//  Created by mac on 13-7-23.
//
//

#include "CCAsyncSprite.h"
#include "NetworkOperationQueue.h"
#include "LocalStorage.h"
#include "RC4Encrypt.h"

CCAsyncSprite::~CCAsyncSprite(){
    NetworkOperationQueue::sharedInstance()->cancelOperationsForDelegate(this);
}

CCAsyncSprite *CCAsyncSprite::createWithUrl(const char *pszUrl){
    CCSpriteFrame *spriteFrame = CCSpriteFrame::create("CloseNormal.png", CCRectMake(0, 0, 30, 30));// CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("EP0.png");
    CCAsyncSprite *ret = new CCAsyncSprite();
    if (ret && ret->initWithSpriteFrame(spriteFrame)) {
        ret->autorelease();
        ret->initWithUrl(pszUrl);
    }
    return ret;
}

bool CCAsyncSprite::initWithUrl(const char *pszUrl){
    const char* data = localStorageGetItem(pszUrl);
    const char* charSize = localStorageGetItem(string(string(pszUrl)+"size").c_str());
//    char *data = RC4Encrypt::Decrypt(rawdata);
//    char *charSize = RC4Encrypt::Decrypt(rawcharSize);
    size_t size = 0;
    if (charSize != NULL) {
        size = atoi(charSize);
    }
    if (data != NULL && size != 0) {
        setImage(data, size, string(pszUrl));
    }
    else {
        NetworkOperation *op = new NetworkOperation(pszUrl);
        op->setDelegate(this);
        this->retain();
        NetworkOperationQueue::sharedInstance()->addOperation(op);
    }
    return true;
}

void CCAsyncSprite::operationDidFail(NetworkOperation *operation){
    this->release();
}

void CCAsyncSprite::operationDidFinish(NetworkOperation *operation){
    this->release();
    size_t size;

    char *data = operation->getRawData(size);
    string url = operation->getUrl();
    
    setImage(data, size, url);
}

void CCAsyncSprite::setImage(const char *data, size_t size, const string &url){
    CCImage::EImageFormat format = CCImage::kFmtUnKnown;
    size_t pos = url.find_last_of('.');
    if (pos != string::npos) {
        string ext = url.substr(pos, url.size()+1);
        if (ext.compare(".png") == 0|| ext.compare(".PNG") == 0) {
            format = CCImage::kFmtPng;
        }
        else if (ext.compare(".jpg") == 0|| ext.compare(".JPG") == 0 || ext.compare(".jpeg") ||ext.compare(".JPEG") == 0){
            format = CCImage::kFmtJpg;
        }
        if (format != CCImage::kFmtUnKnown) {
            setImageWithRawData(data, size, format);
            //缓存
//            char *rawdata = RC4Encrypt::Encrypt(data);
//            localStorageSetItem(url.c_str(), rawdata);
//            char charSize[128] = {0};
//            sprintf(charSize, "%ld", size);
//            char *rawCharSize = RC4Encrypt::Encrypt(charSize);
//            localStorageSetItem(string(url+"size").c_str(), rawCharSize);
        }
    }
}

void CCAsyncSprite::setImageWithRawData(const char *data, size_t size, CCImage::EImageFormat format){
    CCImage* img = new CCImage;
    img->initWithImageData((void*)data, (long)size, format);
    cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
    texture->initWithImage(img);
    CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    this->setDisplayFrame(frame);
}

