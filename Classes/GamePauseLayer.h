#ifndef _GAMEPAUSELAYER_H_
#define _GAMEPAUSELAYER_H_
#include "cocos2d.h"
class GamePauseLayer : public cocos2d::Layer
{
public:
	GamePauseLayer();
	~GamePauseLayer();
	virtual bool init();
	//场景进入退出的方法
	void onEnter();
	void onExit();

	void resumeGame(cocos2d::Ref *r);
	void goBackToMenu(cocos2d::Ref *r);
	void bgmOnOff(cocos2d::Ref *r);
	void effectOnOff(cocos2d::Ref *r);
	CREATE_FUNC(GamePauseLayer);
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * dialog;
	cocos2d::Label * pauseLabel;
	cocos2d::Sprite * pointer1;
	cocos2d::Sprite * pointer2;
	cocos2d::MenuItemImage * resume;
	cocos2d::MenuItemImage * mainMenu;
	cocos2d::Label * label1;
	cocos2d::Label * label2;

	cocos2d::MenuItemSprite * bgm_on;
	cocos2d::Sprite * bgmSprite;

	cocos2d::MenuItemSprite * effect_on;
	cocos2d::Sprite * effectSprite;
	
};
#endif

