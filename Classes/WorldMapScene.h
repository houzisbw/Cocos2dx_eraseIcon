#ifndef _WORLDMAPSCENE_H_
#define _WORLDMAPSCENE_H_
#include "cocos2d.h"
#include "vector"
#include "GirlTwo.h"
using namespace std;
class WorldMapScene : public cocos2d::Layer
{
public:
	WorldMapScene();
	~WorldMapScene();
	virtual bool init();
	CREATE_FUNC(WorldMapScene);
	static cocos2d::Scene * createScene();

	void backToMenu(cocos2d::Ref * r);

	void traceMove();

	void previousLocation(cocos2d::Ref  * r);
	void nextLocation(cocos2d::Ref  * r);
private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * menuBg;
	cocos2d::MenuItemImage * back;
	cocos2d::MenuItemImage * restart;
	
	int curLevelNum;

	vector<cocos2d::Point> pos_vec; //存储20个城市的地理位置

	GirlTwo * gt;

	cocos2d::Sprite * dialog;

	cocos2d::MenuItemImage * previous;
	cocos2d::MenuItemImage * next;

	int loc;
	vector<cocos2d::Sprite*> loc_vec;

	cocos2d::Label * locLabel;


};

#endif
