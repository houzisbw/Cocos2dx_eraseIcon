#ifndef _GAMESELECT_H_
#define _GAMESELECT_H_
#include "cocos2d.h"
class gameSelect:public cocos2d::Layer
{
public:
	gameSelect();
	~gameSelect();
	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(gameSelect);

	void gameStart1(cocos2d::Ref * r);
	void gameStart2(cocos2d::Ref * r);

	//�����ƶ�����
	void move(float dt);
	//�ص����˵�
	void mainMenu(cocos2d::Ref * r);
	//��Ч����
	void particleEffect(cocos2d::Point pos);
private:
	cocos2d::Sprite * bg;
	cocos2d::Label * menuTitle1;
	cocos2d::Label * menuTitle2;
	cocos2d::Sprite * girl;
	float vy_girl1; //����y���ٶ�
	cocos2d::Sprite * transBg;
	cocos2d::Label * des;
	cocos2d::Sprite * pointer1;
	cocos2d::Sprite * pointer2;
	cocos2d::Label * label1;
	cocos2d::Label * label2;
	cocos2d::MenuItemImage * back;
};

#endif

