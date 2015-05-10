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

	void  enableEffectOrNot(bool is);//�Ƿ�����Ϸ��Ч
	bool getEffect();

	void  setAchieveLayer(int is);//
	int  getAdded();

	void  setInitCurrentScore(int is);//����ÿһ�ؿ�ʼ�ĳ�ʼ����
	int  getInitCurrentScore();

	cocos2d::Sprite* getCityAccoringToLevel(int level);

private:
	int levelTargetScore;
	int gameLevel;
	int currentScore;
	int scoreIncrement;//����ÿһ�����ӵ���
	std::vector<int> iconVec;
	//��־������changeIconLayer�Ƿ񱻼���
	bool isChangeLayerAdded;
	bool isBombLayerAdded;
	int selectedIconValue;//ѡȡ��iconֵ��������helloworld�жԱ�

	int changeIconNum;//��һ�ֵ��ߵ�����
	int bombIconNum;//�ڶ��ֵ��ߵ�����

	bool effect;//��Ч����
	int achieve;//0 ��ʾδ��Ӳ㣬1,2,3�ֱ�Ϊ3����

	int initCurrentScore;
	
};

DataManager& getDataManager();


#endif
