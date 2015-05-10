#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "cocos2d.h"
class DataManager
{
public:
	DataManager();
	~DataManager();
	bool init();

	int getLevelTargetScore();
	void setLevelTargetScore(int score);

	int getGameLevel();
	void setGameLevel(int level);

	int getCurrentScore();
	void setCurrentScore(int score);

	int getScoreIncrement();
	void setScoreIncrement(int score);

	std::vector<int> getIconVec();
	void setIconVec(int value);
	void clearIconVec();

	bool getChangeLayerAdded();
	void setChangeLayerAdded(bool is);

	bool getBombLayerAdded();
	void setBombLayerAdded(bool is);

	int getSelectedIconValue();
	void setSelectedIconValue(int value);

	int getChangeIconNum();
	void setChangeIconNum(int value);

	int getBombIconNum();
	void setBombIconNum(int value);

	void  enableEffectOrNot(bool is);//是否开启游戏音效
	bool getEffect();

	void  setAchieveLayer(int is);//
	int  getAdded();

	void  setInitCurrentScore(int is);//设置每一关开始的初始分数
	int  getInitCurrentScore();

	cocos2d::Sprite* getCityAccoringToLevel(int level);

private:
	int levelTargetScore;
	int gameLevel;
	int currentScore;
	int scoreIncrement;//分数每一关增加的量
	std::vector<int> iconVec;
	//标志变量，changeIconLayer是否被加上
	bool isChangeLayerAdded;
	bool isBombLayerAdded;
	int selectedIconValue;//选取的icon值，用于在helloworld中对比

	int changeIconNum;//第一种道具的数量
	int bombIconNum;//第二种道具的数量

	bool effect;//音效变量
	int achieve;//0 表示未添加层，1,2,3分别为3个层

	int initCurrentScore;
	
};

DataManager& getDataManager();


#endif
