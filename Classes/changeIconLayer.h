#ifndef _CHANGEICONLAYER_H_
#define _CHANGEICONLAYER_H_
#include "cocos2d.h"
#include "HelloWorldScene.h"
class changeIconLayer:public cocos2d::Layer
{
public:
	changeIconLayer();
	~changeIconLayer();
	virtual bool init();
	CREATE_FUNC(changeIconLayer);

	void closeMenuCallback(cocos2d::Ref * r);
	//std::vector<int> getIconVector();
private:
	cocos2d::Sprite * bg;
	cocos2d::Label * descriptionLabel;
	cocos2d::Label * titleLabel;
	cocos2d::MenuItemImage * close;
	cocos2d::Menu * closeMenu;
	cocos2d::Label * closeLabel;

	cocos2d::Sprite *icon1;
	cocos2d::Sprite *icon2;
	cocos2d::Sprite *icon3;
	cocos2d::Sprite *icon4;
	cocos2d::Sprite *icon5;

	std::vector<int> vec;
	cocos2d::Vector<cocos2d::Sprite*> iconVec;//存放5个icon 的vector，便于点击查找是哪个icon被点击了
	cocos2d::Sprite * last;

	

	

};
#endif

