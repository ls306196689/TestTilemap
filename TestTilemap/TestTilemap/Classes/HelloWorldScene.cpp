#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
enum TAG_node {
    TAG_map = 123,
    
    };
CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    
    testTMX();
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::testTMX()
{
    CCTMXTiledMap * map = CCTMXTiledMap::create("orthogonal-test2.tmx");
    addChild(map,1,TAG_map);
    
    CCArray* children = map->getChildren();
    
    CCSpriteBatchNode* child = NULL;
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(children, pObject)
    {
        child = (CCSpriteBatchNode*)pObject;
        if (!child) {
            break;
        }
        child->getTexture()->setAntiAliasTexParameters();
    }
    setTouchEnabled(true);
}
void HelloWorld::updateMap()
{
    CCNode *map = getChildByTag(TAG_map);
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint pmov = touch->getDelta();
    CCNode *map = getChildByTag(TAG_map);
    map->stopAllActions();
    map->setPosition(ccpAdd(map->getPosition(), pmov));
}
void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint pmov = touch->getDelta();
    CCNode *map = getChildByTag(TAG_map);
   
    if (fabsf(pmov.x) + fabsf(pmov.y) > 10) {
        float dx = 10 * pmov.x;
        float dy = 10 * pmov.y;
        
        map->runAction(CCEaseSineOut::create(CCMoveBy::create(0.3, ccp(dx, dy))));
    }
}
