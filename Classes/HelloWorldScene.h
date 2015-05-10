#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "changeIconLayer.h"
#include "GirlTwo.h"
#include <cstring>



class HelloWorld : public cocos2d::Layer
{
public:
    
	friend class changeIconLayer;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//添加装备(10*10)
	void addEquipment();

	//生产所有点阵
	void generatePoint();

	//得到相应的装备
	cocos2d::Sprite* getEquipWithCoordinate(cocos2d::Sprite * e[][10], int row, int column);

	//找出相邻的装备的函数
	void findAdjacentEquip(int b[][10], int row, int column,int cur,bool single);

	//确定是否单独一个装备
	bool isSingleIcon(int b[][10], int row, int column, int cur);

	//下移函数
	void moveDown(int b[][10]);

	//打印模拟数组的函数
	void printBoard(int b[][10]);

	//向左移动的函数
	void moveLeft(int b[][10]);

	//检测游戏是否结束：即没有2个相连的装备
	bool isGameOver();

	//装备消去特效
	void equipmentEffect(int boardValue,cocos2d::Point pos);
	void equipmentEffectWithNoGravity(int boardValue, cocos2d::Point pos);

	//胜利特效
	void winEffect(cocos2d::Point pos);

	//回到主菜单
	void returnToMenu(cocos2d::Ref* r);

	//重新开始
	void restartGame(cocos2d::Ref* r);

	//定时调用的函数，用于动态更新
	void gameStep(float dt);

	//游戏结束时摧毁剩下的方块
	void destoryRemaining();

	//开始计时函数
	void startTimer();
	
	//结束计时

	void endTimer();
	
	//计时方法
	virtual void update(float dt);

	//去下一关函数
	void goToNextLevel(Ref * r);

	//道具函数： 改变种类
	void changeIconType(Ref * r);

	//道具函数： 爆炸icon
	void bombIconType(Ref * r);

	//飞弹击碎icon的函数，0.2秒击碎一个,type表示是row还是column消除
	void hitIconByMissile(int column,int row,int type);

	//飞弹击碎icon得到的分数,一个icon只加5分，这样就保证不会频繁使用飞弹
	void scoreGetFromMissile(int posX,int posY);
    
	//替换icon时的动画函数
	void changeIconFlyAnimation(int temp,cocos2d::Point target);
	
	//warning回调函数
	void warningCallback(cocos2d::Ref *r);

	//道具不足提示函数
	void notEnoughItem();


    std::vector<int> icon;

	//gamePauseLayer 层获得当前的level数
	static int getGlobalLevel();



    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	
	
	
	cocos2d::PointArray *pointArray;//声明存放各个装备坐标的数组
	int board[10][10];//模拟摆放装备的数组
	cocos2d::Vector<cocos2d::Sprite*> equip;//存放所有装备的数组 是指针
	cocos2d::Sprite* equipment[10][10];//存放装备的数组
	int count;//消除计数器：至少2个才能消除
	cocos2d::MenuItemImage * btm;//返回主菜单
	cocos2d::MenuItemImage * res;//重新开始
	cocos2d::Label * label_score;//显示分数的控件
	int currentScore; // 游戏当前分数
	cocos2d::Label * lb;
	cocos2d::Vector<cocos2d::Label*> vec_label;
	bool isDouble;
	int tempScore;
	int totalScore;//一次消除的总分数
	//cocos2d::Label* totalScoreLabel;//不能在这里定义，必须临时定义才行
	cocos2d::Sprite * gameEnd;
	
	//数据统计
	int statistic_LeftIcon;
	int statistic_LeftBonus;
	int statistic_SingleMost;
	int statistic_MostIcon;

	int four_kill;
	int five_kill;
	int six_kill;
	int seven_kill;
	int legendary_kill;

	double statistic_Time;
	cocos2d::Label * leftIcon_label;
	cocos2d::Label * leftBonus_label;
	cocos2d::Label * singleMost_label;
	cocos2d::Label * mostIcon_label;
	cocos2d::Label * timer_label;//这个label是最后数据统计显示时间的
	cocos2d::Label * timerLabel; //这个label是实时显示时间的

	cocos2d::Label * fourKill_label;
	cocos2d::Label * fiveKill_label;
	cocos2d::Label * sixKill_label;
	cocos2d::Label * sevenKill_label;
	cocos2d::Label * legendaryKill_label;
	//剩余奖励数组
	std::vector<int> bonus_vec;
	//记录5个icon分别消去的个数的map
	std::map<int, int> icon_map;
	//计时器
	bool timerRunning;
	float startTime;//游戏开始的时间
	bool gameIsOver;//标志游戏结束的变量
	//游戏icon知识的存储
	//std::string info[19][4];
	//std::vector<std::string*> info_vec; //vector储存string数组
	cocos2d::Label * knowledge_label;
	cocos2d::Label * name_label;
	cocos2d::Sprite * small_pic;

	//每一关当前目标分数
	int target_score;
	int curLevel;
	cocos2d::Label * targetScore_label;
	cocos2d::Label * level_label;
	cocos2d::Label * levelNum_label;
	
	//下一关menu按钮
	cocos2d::MenuItemImage * nextLevel;
	cocos2d::Label * nextLevelLabel;

	//ready go 飘字label
	cocos2d::Label * readyLabel;
	cocos2d::Label * goLabel;
	cocos2d::Sprite * transBg;

	bool isGameReady;//开始动画完了才能点击消除方块
	bool isTargetReached;//是否到达目标分数

	cocos2d::Sprite * girl;//每一关开始前介绍的女孩
	cocos2d::Label * welcome;//欢迎来到
	cocos2d::Label * spots;//地点

	//2个功能性道具
	cocos2d::MenuItemImage * changeIcon;
	cocos2d::MenuItemImage * bombIcon;

	//2个道具的数量以及label
	
	cocos2d::Label * changeIconLabel;
	cocos2d::Label * bombIconLabel;

	//2个激光炮塔
	cocos2d::Sprite * tower1;
	cocos2d::Sprite * tower2;
	//2个飞弹
	cocos2d::Sprite * missile1;
	cocos2d::Sprite * missile2;

	//道具不足的提示图片
	cocos2d::Sprite * notEnough;
	cocos2d::MenuItemImage * notEnoughMII;
	cocos2d::Menu * notEnoughMenu;
	cocos2d::Label * ok;
	cocos2d::Sprite * yellow;
	cocos2d::Label * notEnoughDes;

	//剩余奖励数组
	std::vector<int> leftBonus;
	//剩余奖励图标box
	cocos2d::Sprite * leftBonusBox;

	bool firstHit;//判断是否第一次点击

	cocos2d::Sprite * clear;

	bool isGameOverHit;//防止游戏结束时多次点击

	//人工智能 萌系女孩 罗秋雨
	cocos2d::Sprite * lqy;
	float vy_lqy; //萌妹y轴速度
	void moveLqy(float dt);
	float posY_lqy;//lqy在y轴的位置
	cocos2d::Sprite * lqy_dialog;
	cocos2d::Point lastPos;//位置如果变化，则拖动了，放拖动音效
	cocos2d::Point thisPos;

	//22娘
	GirlTwo * tg;
	bool isPlayed;
	cocos2d::Label * girlTalk;

	//数据统计的label们
	cocos2d::Label * statisticTitle;
	cocos2d::Label * title1;
	cocos2d::Sprite * pointer1;
	cocos2d::Label * title2;
	cocos2d::Sprite * pointer2;
	cocos2d::Label * title3;
	cocos2d::Sprite * pointer3;
	cocos2d::Label * title4;
	cocos2d::Sprite * pointer4;
	cocos2d::Label * title5;
	cocos2d::Sprite * pointer5;
	cocos2d::Label * title6;
	cocos2d::Sprite * pointer6;
	cocos2d::Label * title7;
	cocos2d::Sprite * pointer7;
	cocos2d::Label * title8;
	cocos2d::Sprite * pointer8;
	cocos2d::Label * title9;
	cocos2d::Sprite * pointer9;
	cocos2d::Label * title10;
	cocos2d::Sprite * pointer10;

	cocos2d::Menu * menuNextLevel;
	cocos2d::Sprite * clear2;

	


};



#endif // __HELLOWORLD_SCENE_H__
