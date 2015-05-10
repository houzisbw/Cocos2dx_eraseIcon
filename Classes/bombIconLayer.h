#ifndef _BOMBICONLAYER_H_
#define _BOMBICONLAYER_H_
#include "cocos2d.h"
class bombIconLayer:public cocos2d::Layer
{
public:
	bombIconLayer();
	~bombIconLayer();
	virtual bool init();
	CREATE_FUNC(bombIconLayer);

	void closeMenuCallback(cocos2d::Ref * r);
private:
	cocos2d::Sprite * bg;
	cocos2d::Label * descriptionLabel;
	cocos2d::MenuItemImage * close;
	cocos2d::Menu * closeMenu;
	cocos2d::Label * closeLabel;

	cocos2d::Sprite * missile;
	cocos2d::Sprite * turret;
};
#endif

