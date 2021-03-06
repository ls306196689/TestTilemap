#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HelpFunctions.h"
#define WIDTH_MAP 1024.0
#define HEIGHT_MAP 1024.0
using namespace cocos2d;
using namespace CocosDenshion;
enum TAG_node {
    TAG_map = 123,
    
    };

void outputstring (std::string &str)
{
    CCLOG("%s",str.c_str());
}
//const float HelloWorld::m_scale_low_limit = 0.5;
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
    m_scale_touch = 1;
    m_distance_init = 0;

    m_vec_touches = new std::vector<CCTouch*>(0);
    m_screen_size = CCDirector::sharedDirector()->getWinSize();
    testTMX();
    testFun(outputstring, "ssdsdsd");
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
    CCTMXTiledMap * map = CCTMXTiledMap::create("maptest1.tmx");
    float scale = 1;
    m_map_size = CCSizeMake(map->getMapSize().width * map->getTileSize().width * scale, map->getMapSize().height * map->getTileSize().height * scale);

    CCLOG_SIZE(m_map_size);
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
    
    m_tamara = CCSprite::create("grossinis_sister1.png");
    map->addChild(m_tamara, map->getChildren()->count() );
    m_tamara->retain();
    int mapWidth = map->getMapSize().width * map->getTileSize().width;
    m_tamara->setPosition(CC_POINT_PIXELS_TO_POINTS(ccp( mapWidth/2,0)));
    m_tamara->setAnchorPoint(ccp(0.5f,-0.2f));
    
    
    CCActionInterval* move = CCMoveBy::create(10, ccp(300,250));
    CCActionInterval* back = move->reverse();
    CCSequence* seq = CCSequence::create(move, back,NULL);
    m_tamara->runAction( CCRepeatForever::create(seq) );
    schedule( schedule_selector(HelloWorld::repositionSprite) );
}
void HelloWorld::repositionSprite(float dt)
{
    CCPoint p = m_tamara->getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    CCNode *map = getChildByTag(TAG_map);
    
    // there are only 4 layers. (grass and 3 trees layers)
    // if tamara < 48, z=4
    // if tamara < 96, z=3
    // if tamara < 144,z=2
    
    int newZ = 4 - (p.y / 48);
    newZ = MAX((int) newZ,(int)0);
    
    map->reorderChild(m_tamara, newZ);
}
void HelloWorld::updateMap()
{
//    CCNode *map = getChildByTag(TAG_map);
}
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{

//    addCCTouch(pTouches);
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
//    CCLOG_D(pTouches->count());
    addCCTouch(pTouches);
    updateLayerAnchorPoint();
    checkShouldMove();
    updateLayerAnchorPoint();
    checkShouldScale();
    updateLayerAnchorPoint();
}
void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{

    removeCCTouch(pTouches);

}
void HelloWorld::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    removeCCTouch(pTouches);
//    m_delta_last = CCPointZero;
}
void HelloWorld::testFun(FUNCALL_TEST fun, std::string str)
{
    if (fun != NULL) {
        fun(str);
    }
}
bool HelloWorld::checkShouldScale()
{
    if (m_vec_touches->size() >= 2 && m_distance_init > 0) {
        float distance = ccpDistance(m_vec_touches->at(0)->getLocation(), m_vec_touches->at(1)->getLocation());
        m_scale_touch += (distance - m_distance_init)/m_distance_init;
        
        if (m_scale_touch > m_scale_low_limit && m_scale_touch < m_scale_up_limit) {
            m_distance_init = distance;
            CCNode *map  = getChildByTag(TAG_map);
            map->setScale(m_scale_touch);
        }
        else
        {
            if (m_scale_touch < m_scale_low_limit) {
                
                m_scale_touch = m_scale_low_limit;
            }
            if (m_scale_touch > m_scale_up_limit) {
                
                m_scale_touch = m_scale_up_limit;
            }
        }
        return true;
    }
    return false;
}
bool HelloWorld::checkShouldMove()
{
    CCPoint dp = CCPointZero;
    int i = 0;
    for (i = 0; i < 2 && i < m_vec_touches->size(); i ++) {
        dp = ccpAdd(dp, m_vec_touches->at(i)->getDelta());
    }
    if (i > 0) {
        dp = ccp(dp.x / (float)i,dp.y / (float)i);
    }
    m_last_move = dp;
    moveMapByPosition(dp);
    return m_vec_touches->size() > 0;
}
void HelloWorld::updateLayerAnchorPoint()
{
    CCPoint p0 = CCPointZero;
    CCPoint p1 = CCPointZero;
    if (m_vec_touches->size() >= 2) {
        p0 = m_vec_touches->at(0)->getLocation();
        p1 = m_vec_touches->at(1)->getLocation();
        
        CCPoint pmid = ccpMidpoint(p0, p1);
        CCNode *map  = getChildByTag(TAG_map);
        
        
        CCPoint dpmid = ccpSub(pmid ,map->getPosition());
        CCPoint danchor = ccp(dpmid.x / (map->getContentSize().width * map->getScaleX()), dpmid.y / (map->getContentSize().height * map->getScaleY()));

        CCPoint anchor = map->getAnchorPoint();
        CCPoint newanchor = ccpAdd(anchor, danchor);
        
        map->setPosition(pmid);
        map->setAnchorPoint(newanchor);

    }
  
}
void HelloWorld::addCCTouch(cocos2d::CCSet *pTouches)
{
    while (pTouches->count() > 0) {
        bool res = true;
        CCTouch * touch = (CCTouch *) pTouches->anyObject();
        for (int j = 0; j < m_vec_touches->size(); j ++) {
            if (touch == m_vec_touches->at(j)) {
                res = false;
                break;
            }
        }
        if (res) {
            m_vec_touches->push_back(touch);
            if (m_vec_touches->size() == 2) {
                m_distance_init = ccpDistance(m_vec_touches->at(0)->getLocation(), m_vec_touches->at(1)->getLocation());
            }
        }
        pTouches->removeObject(touch);
    }
    
}
void HelloWorld::removeCCTouch(cocos2d::CCSet *pTouches)
{
    while (pTouches->count() > 0) {

        CCTouch * touch = (CCTouch *) pTouches->anyObject();
        for (int j = 0; j < m_vec_touches->size(); j ++) {
            if (touch == m_vec_touches->at(j)) {
                m_vec_touches->erase(m_vec_touches->begin() + j);
                if (m_vec_touches->size() == 2) {
                    m_distance_init = ccpDistance(m_vec_touches->at(0)->getLocation(), m_vec_touches->at(1)->getLocation());
                }
                break;
            }
        }
        pTouches->removeObject(touch);
    }
    if (m_vec_touches->size() == 0) {
        
        CCPoint dp = ccp(m_last_move.x * 5 , m_last_move.y * 5 );
        moveMapByPosition(dp);
        m_last_move = CCPointZero;

    }
}
void HelloWorld::moveMapByPosition(cocos2d::CCPoint pos)
{
//    float dis = ccpLength(pos);
    CCNode *map  = getChildByTag(TAG_map);
    CCPoint pnew = getMapNewPosition(pos);
    float dis = ccpDistance(pnew, map->getPosition());
    if (dis < 20) {
        map->setPosition(pnew);
    }
    else
    {
        map->runAction(CCEaseSineOut::create(CCMoveTo::create(0.3, pnew)));
    }
}
cocos2d::CCPoint HelloWorld::getMapNewPosition(cocos2d::CCPoint dpos)
{
    CCNode *map  = getChildByTag(TAG_map);
    
    CCPoint p = map->getPosition();
    CCPoint pan = map->getAnchorPoint();
    float scalemap = map->getScale();
    CCPoint plb = ccp(p.x - m_map_size.width * pan.x * scalemap, p.y - m_map_size.height * pan.y* scalemap);
    CCPoint prt = ccp(plb.x + m_map_size.width * scalemap,plb.y + m_map_size.height * scalemap);
    if (plb.x + dpos.x > 0) {
        dpos.x = -plb.x;
    }
    if (plb.y + dpos.y > 0) {
        dpos.y = -plb.y;
    }
    
//    if (prt.x < m_screen_size.width) {
//        dpos.x += (m_screen_size.width - prt.x);
//    }
//    if (prt.y < m_screen_size.height) {
//        dpos.y += (m_screen_size.height - prt.y);
//    }
    CCLOG_POINT(dpos);
    return ccpAdd(dpos, map->getPosition());
}
