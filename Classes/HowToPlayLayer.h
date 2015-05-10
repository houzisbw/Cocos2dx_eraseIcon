#ifndef _HOWTOPLAYLAYER_H_
#define _HOWTOPLAYLAYER_H_
#include "cocos2d.h"
class HowToPlayLayer:public cocos2d::Layer
{
public:
	HowToPlayLayer();
	~HowToPlayLayer();
	virtual bool init();
	CREATE_FUNC(HowToPlayLayer);

	//萌妹移动函数
	void move(float dt);

	//NEXT介绍回调函数
	void nextIntro(cocos2d::Ref * r);

	//以下函数是girl移动函数
	void girlMove(std::string str, float girlPosY, cocos2d::Point destination);
	//开始游戏，删除layer
	void deleteLayer();

private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * girl;
	float vy_girl; //萌妹y轴速度
	float girlPosY;//萌妹的y位置，因为move函数不能传递进pos参数
	cocos2d::Sprite * dialog;//对话框，介绍每一项功能
	cocos2d::Label * function;//label说明介绍
	cocos2d::MenuItemSprite * next;//下一项介绍
	cocos2d::Menu * menuNext;
	cocos2d::Sprite * hand;
	//控制每一步的变量
	int progress;
};
#endif

