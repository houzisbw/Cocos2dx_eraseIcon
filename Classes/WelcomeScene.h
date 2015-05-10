#ifndef _WELCOMESCENE_H_
#define _WELCOMESCENE_H_
#include "cocos2d.h"
#include "GirlTwo.h"
#pragma once
class WelcomeScene:public cocos2d::Layer
{
public:
	WelcomeScene();
	~WelcomeScene();
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(WelcomeScene);
public:
	//背景下移函数，实现滚动效果
	void move(float dt);
	void gameStart(cocos2d::Ref *r);
	void gameOption(cocos2d::Ref *r);
	void gameEnd(cocos2d::Ref * r);
	//判断是否存在存档数据
	bool isSaveFile();
	void particleEffect(cocos2d::Point pos);
private:
	float vy;  //y方向上移动速度
	float vx;  //x方向上移动速度
	float vy_girl1; //萌妹y轴速度
	cocos2d::Sprite* bg1;
	cocos2d::Sprite* bg2;
	cocos2d::Sprite* cloud;
	cocos2d::Sprite* girl1;
	cocos2d::Label * menuTitle1;
	cocos2d::Label * menuTitle2;
	cocos2d::Label * menuTitle3;
	cocos2d::Label * title1;
	cocos2d::Label * title2;

	//22娘
	GirlTwo * gt;
};
#endif

