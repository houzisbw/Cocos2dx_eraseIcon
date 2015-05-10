#ifndef _ISCONTINUELASTPLAY_H_
#define _ISCONTINUELASTPLAY_H_
#include "cocos2d.h"
class isContinueLastPlay :public cocos2d::Layer
{
public:
	isContinueLastPlay();
	~isContinueLastPlay();
	virtual bool init();
	CREATE_FUNC(isContinueLastPlay);

	//游戏开始
	void gameStart(cocos2d::Ref * r);
	//重新开始游戏
	void gameStartNew(cocos2d::Ref * r);
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * transBg;
	cocos2d::Label  * des;
	cocos2d::Label  * des2;
	cocos2d::MenuItemImage * left;
	cocos2d::MenuItemImage * right;
	cocos2d::Label * yes;
	cocos2d::Label * no;
	cocos2d::Label * levelNum;
};
#endif

