#ifndef _FINALSCENE_H_
#define _FINALSCENE_H_
#include "cocos2d.h"
class finalScene:public cocos2d::Layer
{
public:
	finalScene();
	~finalScene();
	virtual bool init();
	CREATE_FUNC(finalScene);
	static cocos2d::Scene * createScene();
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * dialog;
	cocos2d::Sprite * ok;
	cocos2d::Label  * okLabel;
	cocos2d::Sprite * clear;
	cocos2d::Sprite * title;
	cocos2d::Sprite * girl1;
	cocos2d::Sprite * girl2;
	cocos2d::Label  * web;
	cocos2d::Label  * web2;
};
#endif

