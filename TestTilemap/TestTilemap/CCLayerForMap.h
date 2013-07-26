//
//  CCLayerForMap.h
//  TestTilemap
//
//  Created by apple on 13-7-26.
//
//

#ifndef __TestTilemap__CCLayerForMap__
#define __TestTilemap__CCLayerForMap__

#include "cocos2d.h"

class CCLayerForMap:public cocos2d::CCLayer {
    enum TAG_node {
        TAG_map = 123,
        
    };
    
    cocos2d::CCPoint m_delta_last;
    cocos2d::CCSize m_map_size;
    cocos2d::CCSprite*    m_tamara;
    cocos2d::CCSize m_screen_size;
    
    std::vector<cocos2d::CCTouch*> *m_vec_touches;
    float m_scale_touch;
    float m_distance_init;
    const float m_scale_low_limit = 0.5;
    const float m_scale_up_limit = 2;
    const float m_distance_limit = 100;
    float m_distance_limit_x;
    float m_distance_limit_y;
    cocos2d::CCPoint m_last_move;
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    void testTMX();
    
    void updateMap();
    void repositionSprite(float dt);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    

    bool checkShouldScale();
    bool checkShouldMove();
    void updateLayerAnchorPoint();
    void addCCTouch(cocos2d::CCSet *pTouches);
    void removeCCTouch(cocos2d::CCSet *pTouches);
    
    //    void moveMapByPosition(cocos2d::CCPoint pos, bool withaction = false);
    
    //    cocos2d::CCPoint getMapNewPosition(cocos2d::CCPoint dpos);
    
    cocos2d::CCPoint getMapNewPosition(cocos2d::CCPoint dpos, int edgemode = 0);//0:只能靠边；1：无需靠边，2：无需靠边，但离边越远反应越迟钝
    void moveMapByPosition(cocos2d::CCPoint pos, int edgemode = 0, bool withaction = false);//0:只能靠边；1：无需靠边，2：无需靠边，但离边越远反应越迟钝
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CCLayerForMap);
    
};

#endif /* defined(__TestTilemap__CCLayerForMap__) */
