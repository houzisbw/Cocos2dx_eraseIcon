#ifndef _ACHIEVELAYER_H_
#define _ACHIEVELAYER_H_
#include "cocos2d.h"
class AchieveLayer:public cocos2d::Layer
{
public:
	AchieveLayer();
	~AchieveLayer();
	virtual bool init();
	CREATE_FUNC(AchieveLayer);
private:
	cocos2d::Sprite * dialog;
	cocos2d::Sprite * pointerLeft;
	cocos2d::Sprite * pointerRight;
	cocos2d::Label  * record;
	cocos2d::Sprite * ok;
	cocos2d::Label  * okLabel;
	cocos2d::Sprite * icon1;
	cocos2d::Label  * historyBestScoreLabel;
	cocos2d::Label  * historyBestScoreLabelNum;

	cocos2d::Sprite * icon2;
	cocos2d::Label  * singleBest;
	cocos2d::Label  * singleBestNum;

	//游戏信息的label
	cocos2d::Sprite * pointerLeft1;
	cocos2d::Sprite * pointerRight1;
	cocos2d::Label  * info1;
	cocos2d::Sprite * icon3;
	cocos2d::Label  * infoLabel1;
	cocos2d::Sprite * icon4;
	cocos2d::Label  * infoLabel2;
	cocos2d::Sprite * icon5;
	cocos2d::Label  * infoLabel3;
	cocos2d::Sprite * icon6;
	cocos2d::Label  * infoLabel4;
	//道具兑换信息
	cocos2d::Sprite * pointerLeft2;
	cocos2d::Sprite * pointerRight2;
	cocos2d::Label  * info2;
	cocos2d::Label  * des2;
	cocos2d::Sprite * legendaryDialog;
	cocos2d::Label  * legendaryLabelDes;
	cocos2d::Label  * legendaryLabelNum;
	cocos2d::Sprite * itemChange;
	cocos2d::Sprite * itemBomb;
	cocos2d::Label  * changeNum;
	cocos2d::Label  * bombNum;
	//换取道具时的对话框
	cocos2d::Sprite * buyDialog;
	cocos2d::Label  * isBuy;
	cocos2d::Sprite * buyYes;
	cocos2d::Sprite * buyNo;
	//cocos2d::Sprite * warning;

	int changeOrBomb;
	//防止重复点击换取道具的变量
	bool isClickedChange;
	bool isClickedBomb;
};

#endif

