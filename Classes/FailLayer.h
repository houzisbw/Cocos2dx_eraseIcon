#ifndef _FAILLAYER_H_
#define _FAILLAYER_H_
#include "cocos2d.h"
class FailLayer:public cocos2d::Layer
{
public:
	FailLayer();
	~FailLayer();
	virtual bool init();

	void yesButton(cocos2d::Ref * r);
	void noButton(cocos2d::Ref * r);

	CREATE_FUNC(FailLayer);
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * fail;
	cocos2d::Sprite * transBg;
	cocos2d::Label * title;
	cocos2d::MenuItemImage * left;
	cocos2d::MenuItemImage * right;
	cocos2d::Label * leftLabel;
	cocos2d::Label * rightLabel;
};
#endif

