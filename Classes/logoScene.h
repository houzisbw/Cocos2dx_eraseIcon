#ifndef _LOGOSCENE_H_
#define _LOGOSCENE_H_
#include "cocos2d.h"
class logoScene :public cocos2d::Layer
{
public:
	logoScene();
	~logoScene();
	virtual bool init();
	CREATE_FUNC(logoScene);
	static cocos2d::Scene* createScene();
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * girl1;
	cocos2d::Sprite * girl2;
	cocos2d::Label  * logo;
};
#endif

