#include "DataManager.h"
USING_NS_CC;
using namespace std;

DataManager::DataManager()
{
	gameLevel = 1;
	levelTargetScore = 1000; 
	currentScore = 0;
	scoreIncrement = 1500;
	isChangeLayerAdded = false;
	isBombLayerAdded = false;

	selectedIconValue = -1;

	changeIconNum = 10; //只有10个
	bombIconNum = 10;

	effect = true;//默认开启游戏音效
	achieve = 0;
	
}


DataManager::~DataManager()
{
}

void  DataManager::setInitCurrentScore(int is)//设置每一关开始的初始分数
{
	initCurrentScore = is;
}
int  DataManager::getInitCurrentScore()
{
	return initCurrentScore;
}

void DataManager::setAchieveLayer(int is)
{
	achieve = is;
}
int  DataManager::getAdded()
{
	return achieve;
}


int DataManager::getBombIconNum()
{
	return bombIconNum;
}
void  DataManager::setBombIconNum(int value)
{
	bombIconNum = value;
}


void   DataManager::enableEffectOrNot(bool is)//是否开启游戏音效
{
	effect = is;
}
bool DataManager::getEffect()
{
	return effect;
}


int DataManager::getChangeIconNum()
{
	return changeIconNum;
}

void DataManager::setChangeIconNum(int value)
{
	changeIconNum = value;
}

int DataManager::getSelectedIconValue()
{
	return selectedIconValue;
}
void DataManager::setSelectedIconValue(int value)
{
	selectedIconValue = value;
}

void DataManager::clearIconVec()
{
	iconVec.clear();
}

std::vector<int> DataManager::getIconVec()
{
	return iconVec;
}
void DataManager::setIconVec(int value)
{
	iconVec.push_back(value);
}


int DataManager::getGameLevel()
{
	return gameLevel;
}
int DataManager::getLevelTargetScore()
{
	return levelTargetScore;
}

void DataManager::setGameLevel(int level)
{
	gameLevel = level;
}
void DataManager::setLevelTargetScore(int score)
{
	levelTargetScore = score;
}
int DataManager::getCurrentScore()
{	
	return currentScore;
}

void DataManager::setCurrentScore(int score)
{
	currentScore = score;
}
int DataManager::getScoreIncrement()
{
	return scoreIncrement;
}
void DataManager::setScoreIncrement(int score)
{
	scoreIncrement = score;
}

bool DataManager::getChangeLayerAdded()
{
	return isChangeLayerAdded;
}

void DataManager::setChangeLayerAdded(bool is)
{
	isChangeLayerAdded = is;
}

bool DataManager::getBombLayerAdded()
{
	return isBombLayerAdded;
}
void DataManager::setBombLayerAdded(bool is)
{
	isBombLayerAdded = is;
}



DataManager& getDataManager()
{
	static DataManager mgr;
	return mgr;
}
