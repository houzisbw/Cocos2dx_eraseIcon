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
	//�������ƺ�����ʵ�ֹ���Ч��
	void move(float dt);
	void gameStart(cocos2d::Ref *r);
	void gameOption(cocos2d::Ref *r);
	void gameEnd(cocos2d::Ref * r);
	//�ж��Ƿ���ڴ浵����
	bool isSaveFile();
	void particleEffect(cocos2d::Point pos);
private:
	float vy;  //y�������ƶ��ٶ�
	float vx;  //x�������ƶ��ٶ�
	float vy_girl1; //����y���ٶ�
	cocos2d::Sprite* bg1;
	cocos2d::Sprite* bg2;
	cocos2d::Sprite* cloud;
	cocos2d::Sprite* girl1;
	cocos2d::Label * menuTitle1;
	cocos2d::Label * menuTitle2;
	cocos2d::Label * menuTitle3;
	cocos2d::Label * title1;
	cocos2d::Label * title2;

	//22��
	GirlTwo * gt;
};
#endif

