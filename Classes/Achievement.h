#ifndef _ACHIEVEMENT_H_
#define _ACHIEVEMENT_H_
#include "cocos2d.h"
class Achievement: public cocos2d::Layer
{
public:
	Achievement();
	~Achievement();
	virtual bool init();
	CREATE_FUNC(Achievement);
	static cocos2d::Scene* createScene();
	void goBackToMainmenu(cocos2d::Ref * r);
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * menu;
	cocos2d::MenuItemImage * back;
	cocos2d::Sprite * transBg;
	cocos2d::Sprite * girl;
	cocos2d::Label * des;
	cocos2d::Sprite * trophy;
	cocos2d::Sprite * info;
	cocos2d::Sprite * item;
	bool isLayerAdded;



};
#endif

