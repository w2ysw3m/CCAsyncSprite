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
    CCSpriteFrame *spriteFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("EP0.png"); //CCSpriteFrame::create("CloseNormal.png", CCRectMake(0, 0, 30, 30));
    CCAsyncSprite *ret = new CCAsyncSprite();
    if (ret && ret->initWithSpriteFrame(spriteFrame)) {
        ret->autorelease();
        ret->initWithUrl(pszUrl);
    }
    return ret;
}

bool CCAsyncSprite::initWithUrl(const char *pszUrl){
    string url(pszUrl);
    size_t pos = url.find_last_of('/');
    if (pos != string::npos) {
        m_strFileName = url.substr(pos, url.size()+1);
    }
    else {
        return false;
    }
    
    CCImage *image = new CCImage();
    if (image->initWithImageFile((CCFileUtils::sharedFileUtils()->getWritablePath()+m_strFileName).c_str())){
        setSpriteWithCCImage(image);
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
            
            CCImage* img = new CCImage;
            img->initWithImageData((void*)data, (long)size, format);
            
            setSpriteWithCCImage(img);
            img->saveToFile((CCFileUtils::sharedFileUtils()->getWritablePath()+m_strFileName).c_str());
        }
    }
}

void CCAsyncSprite::setSpriteWithCCImage(CCImage *img){

    cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
    texture->initWithImage(img);
    CCSpriteFrame *frame = CCSpriteFrame::createWithTexture(texture, CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));
    this->setDisplayFrame(frame);
}

