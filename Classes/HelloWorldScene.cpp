#include "HelloWorldScene.h"
#include "WelcomeScene.h"
#include "DataManager.h"
#include "GamePauseLayer.h"
#include "FailLayer.h"
#include "changeIconLayer.h"
#include "bombIconLayer.h"
#include "HowToPlayLayer.h"
#include "SimpleAudioEngine.h"
#include "finalScene.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <string>

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;




Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
	
}

void HelloWorld::moveLqy(float dt)
{
	int delta = 3;
	float nextGirlY = lqy->getPositionY() - vy_lqy;
	//log("nextGirlY: %f", nextGirlY);
	//log("girlPosY: %f", girlPosY);
	lqy->setPositionY(nextGirlY);
	if (nextGirlY >= posY_lqy + delta)
	{
		vy_lqy = -vy_lqy;
	}
	if (nextGirlY <= posY_lqy - delta)
	{
		//log("vy_girl: %f",vy_girl);
		vy_lqy = -vy_lqy;
	}
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	//这里更新每一次玩到的关卡数 
	int tempLevel = getDataManager().getGameLevel();
	UserDefault::getInstance()->setIntegerForKey("level_Global", tempLevel);

	//将每一关初始化分数存进文件，下次进游戏要用
	int tempCurrent = getDataManager().getInitCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("levelInitScore", tempCurrent);

	//更新目标分数
	int tempTargetScore = getDataManager().getLevelTargetScore();
	UserDefault::getInstance()->setIntegerForKey("level_Global_targetScore", tempTargetScore);

	UserDefault::getInstance()->flush();

	firstHit = true;//第一次点击，播放firstblood音效
	isGameOverHit = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	//剩余奖励数组,5个以下才奖励
	leftBonus.push_back(1000);
	leftBonus.push_back(500);
	leftBonus.push_back(250);
	leftBonus.push_back(125);
	leftBonus.push_back(100);

	

	tg = GirlTwo::create();
	tg->setPosition(390, 650);
	tg->setScale(0.8);
	addChild(tg, 20);

	lqy_dialog = Sprite::create("gameEnd.png");
	lqy_dialog->setScale(0.3);
	lqy_dialog->setPosition(tg->getPosition() - Point(130, 0));
	lqy_dialog->setVisible(false);
	addChild(lqy_dialog, 21);

	

	//处理22娘说的话
	//const char *str_girl = ((String*)dic->objectForKey(""))->_string.c_str();
	girlTalk = Label::createWithTTF("", "fonts/b.ttf", 18);
	girlTalk->setDimensions(130,300);
	girlTalk->setPosition(tg->getPosition() - Point(125, 100));
	girlTalk->setVisible(false);
	addChild(girlTalk, 22);
	

	//道具不足的提示图片,开始不显示
	notEnough = Sprite::create("warning.png");
	notEnough->setScale(0.8);
	notEnough->setVisible(false);
	notEnough->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(notEnough, 10);

	yellow = Sprite::create("yellowWarning.png");
	yellow->setVisible(false);
	yellow->setScale(0.6);
	yellow->setPosition(visibleSize.width / 2 - 75, visibleSize.height / 2 + 40);
	addChild(yellow, 11);

	const char *str_notDes = ((String*)dic->objectForKey("notenough"))->_string.c_str();
	notEnoughDes = Label::createWithTTF(str_notDes, "fonts/b.ttf", 35);
	notEnoughDes->setPosition(visibleSize.width / 2+32, visibleSize.height / 2 + 40);
	notEnoughDes->setVisible(false); 
	addChild(notEnoughDes, 11);

	notEnoughMII = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(HelloWorld::warningCallback, this));
	notEnoughMII->setScale(0.4);
	notEnoughMenu = Menu::create(notEnoughMII, NULL);
	notEnoughMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 50);
	notEnoughMenu->setVisible(false);
	addChild(notEnoughMenu, 12);

	const char *str_ok = ((String*)dic->objectForKey("ok"))->_string.c_str();
	ok = Label::createWithTTF(str_ok, "fonts/b.ttf", 20);
	ok->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 50);
	ok->setVisible(false);
	addChild(ok, 13);

	
	tower1 = nullptr;
	tower2 = nullptr;

	isGameReady = false;
	isTargetReached = false;
	curLevel = getDataManager().getGameLevel();

	
	

	//2个功能性道具
	changeIcon = MenuItemImage::create("changeIcon.png","changeIcon.png",CC_CALLBACK_1(HelloWorld::changeIconType,this));
	changeIcon->setScale(0.5);
	auto changeIconMenu = Menu::create(changeIcon, NULL); 
	changeIconMenu->setPosition(visibleSize.width / 2 + 100, visibleSize.height - 75);
	addChild(changeIconMenu, 3);
	changeIcon->setOpacity(0);
	ActionInterval * delay_ci = DelayTime::create(4.6);   //每个sprite出现间隔0.4秒
	auto fi_ci = FadeIn::create(0.5);
	auto seq_ci = Sequence::create(delay_ci, fi_ci, NULL);
	changeIcon->runAction(seq_ci);

	//道具1的数量label
	char cChangeIconNum[10];
	sprintf(cChangeIconNum, "%d", UserDefault::getInstance()->getIntegerForKey("ChangeNum"));
	string strChangeIconNum = cChangeIconNum;
	changeIconLabel = Label::createWithTTF(cChangeIconNum, "fonts/pirulenrg.ttf", 18);
	changeIconLabel->setPosition(visibleSize.width / 2 + 125, visibleSize.height - 92);
	addChild(changeIconLabel, 3);
	changeIconLabel->setOpacity(0);
	ActionInterval * delay_cil = DelayTime::create(4.6);   //每个sprite出现间隔0.4秒
	auto fi_cil = FadeIn::create(0.5);
	auto seq_cil = Sequence::create(delay_cil, fi_cil, NULL);
	changeIconLabel->runAction(seq_cil);


	//道具2的数量label
	char cBombIconNum[10];
	sprintf(cBombIconNum, "%d", UserDefault::getInstance()->getIntegerForKey("BombNum"));
	string strBombIconNum = cBombIconNum;
	bombIconLabel = Label::createWithTTF(cBombIconNum, "fonts/pirulenrg.ttf", 18);
	bombIconLabel->setPosition(visibleSize.width / 2 + 185, visibleSize.height - 92);
	addChild(bombIconLabel, 3);
	bombIconLabel->setOpacity(0);
	ActionInterval * delay_bil = DelayTime::create(5);   //每个sprite出现间隔0.4秒
	auto fi_bil = FadeIn::create(0.5);
	auto seq_bil = Sequence::create(delay_bil, fi_bil, NULL);
	bombIconLabel->runAction(seq_bil);


	bombIcon = MenuItemImage::create("bomb.png", "bomb.png", CC_CALLBACK_1(HelloWorld::bombIconType, this));
	bombIcon->setScale(0.5);
	auto bombIconMenu = Menu::create(bombIcon, NULL);
	bombIconMenu->setPosition(visibleSize.width / 2 + 160, visibleSize.height - 75);
	addChild(bombIconMenu, 3);
	bombIcon->setOpacity(0);
	ActionInterval * delay_bi = DelayTime::create(5);   //每个sprite出现间隔0.4秒
	auto fi_bi = FadeIn::create(0.5);
	auto seq_bi = Sequence::create(delay_bi, fi_bi, NULL);
	bombIcon->runAction(seq_bi);

	//让道具数字变红
	if (UserDefault::getInstance()->getIntegerForKey("BombNum")==0)
	{
		bombIconLabel->setColor(Color3B(208, 52, 13));
	}
	if (UserDefault::getInstance()->getIntegerForKey("ChangeNum")==0)
	{
		changeIconLabel->setColor(Color3B(208, 52, 13));
	}

	
	
	
	//半透明黑色背景
	transBg = Sprite::create("transparent.png");
	transBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	transBg->setScaleX(1.5);
	transBg->setScaleY(0.001);
	addChild(transBg,7);

	ActionInterval * delaytime_bg = CCDelayTime::create(1.5);
	auto st_bg = ScaleTo::create(0.5, 1.5, 0.8);  //这里的x y比例不是原来图片的大小了，是setscale之后的大小了
	auto sq_bg = Sequence::create(delaytime_bg, st_bg, NULL);
	transBg->runAction(sq_bg);

	//女孩飞入介绍城市
	girl = Sprite::create("girl1.png");
	girl->setScale(0.2);
	girl->setPosition(-100,visibleSize.height/2);
	addChild(girl,15);
	ActionInterval * delaytime_girl = CCDelayTime::create(2.0);
	auto mt_girl = MoveTo::create(0.5, Vec2(50, visibleSize.height / 2));
	auto sq_girl = Sequence::create(delaytime_girl, mt_girl, NULL);
	girl->runAction(sq_girl);

	//2条label相反方向飞入
	const char *str_welcome = ((String*)dic->objectForKey("welcome"))->_string.c_str();
	welcome = Label::createWithTTF(str_welcome, "fonts/b.ttf", 40);
	welcome->setRotationSkewX(15);//这个函数是倾斜字体用的
	welcome->setPosition(-100, visibleSize.width / 2 + 180);
	addChild(welcome, 8);
	ActionInterval * delaytime_welcome = CCDelayTime::create(2.0);
	auto mt_welcome = MoveTo::create(0.6, Vec2(200, visibleSize.width / 2 + 180));
	auto deleteWelcome = CallFunc::create([=](){

		welcome->removeAllChildrenWithCleanup(true);
	});
	ActionInterval * delaytime_welcome2 = CCDelayTime::create(2.0);
	auto sq_welcome = Sequence::create(delaytime_welcome, mt_welcome,delaytime_welcome2,deleteWelcome ,NULL);
	welcome->runAction(sq_welcome);


	
	

	//城市string，保存各个城市的名字
	string cityName[20] = {"beijing","tokyo","singnapore","hawaii","canada","newyork","sanfan","rio","london","germany","swiss","dutch","greece","moscow","paris","rome","africa","dubai","maldive","sydeny"};
	//介绍地名的label
	const char *str_city = ((String*)dic->objectForKey(cityName[curLevel-1]))->_string.c_str();
	spots = Label::createWithTTF(str_city, "fonts/b.ttf", 30);
	spots->setColor(Color3B(242,221,28));
	spots->setRotationSkewX(15);
	spots->setPosition(650, visibleSize.width / 2 + 125);
	addChild(spots, 8);
	ActionInterval * delaytime_spots = CCDelayTime::create(2.0);
	auto mt_spots = MoveTo::create(0.6, Vec2(300, visibleSize.width / 2 + 125));
	auto deleteSpots = CallFunc::create([=](){

		spots->removeAllChildrenWithCleanup(true);
	});
	ActionInterval * delaytime_spots2 = CCDelayTime::create(2.0);
	auto sq_spots = Sequence::create(delaytime_spots, mt_spots,delaytime_spots2,deleteSpots, NULL);
	spots->runAction(sq_spots);



	
	//ready go 动画
	readyLabel = Label::createWithTTF("READY", "fonts/HANGTHEDJ.ttf", 100);
	readyLabel->setPosition(visibleSize.width+200, visibleSize.height / 2);
	addChild(readyLabel,8);
	ActionInterval * delaytime_ready = CCDelayTime::create(4.7);
	auto mt_ready = MoveTo::create(0.3, Point(300, visibleSize.height / 2));
	auto mt_ready_1 = MoveTo::create(1.0, Point(200, visibleSize.height / 2));
	auto mt_ready_2 = MoveTo::create(0.3, Point(-200, visibleSize.height / 2));
	auto musicReady = CallFunc::create([=](){
		SimpleAudioEngine::getInstance()->playEffect("ready.OGG");
	});
	auto sq_ready = Sequence::create(delaytime_ready, musicReady,mt_ready, mt_ready_1,mt_ready_2,NULL);
	readyLabel->runAction(sq_ready);

	goLabel = Label::createWithTTF("GO", "fonts/HANGTHEDJ.ttf", 100);
	goLabel->setScale(2.5);
	goLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	goLabel->setOpacity(0);
	addChild(goLabel, 7);
	auto fi_go = FadeIn::create(0.5);
	auto st_go = ScaleTo::create(0.5, 1);
	auto sp_go = Spawn::create(fi_go, st_go, NULL);
	ActionInterval * delaytime_go = CCDelayTime::create(6.3);
	auto deleteBg = CallFunc::create([=](){

		transBg->removeFromParentAndCleanup(true);
		goLabel->removeFromParentAndCleanup(true);
		girl->removeFromParentAndCleanup(true);
		isGameReady = true;
	});
	ActionInterval * delaytime_wait = CCDelayTime::create(0.4);
	auto sq_go = Sequence::create(delaytime_go, sp_go,delaytime_wait,deleteBg,NULL);
	goLabel->runAction(sq_go);
	
	//添加怎么玩的层,只有第一关才有
	if (getDataManager().getGameLevel() == 1)
	{
		ActionInterval * delaytime_htp = CCDelayTime::create(7.5);
		auto addHowToPlay = CallFunc::create([=](){

			HowToPlayLayer * htp = HowToPlayLayer::create();
			htp->setPosition(0, 0);
			addChild(htp, 20);
		});
		auto seq_htp = Sequence::create(delaytime_htp, addHowToPlay, NULL);
		this->runAction(seq_htp);
	}
	
	
	//更新每一关的目标分数
	target_score = getDataManager().getLevelTargetScore();
	char target_temp[20];
	sprintf(target_temp, "%d", target_score);
	string target_str = target_temp;
	targetScore_label = Label::createWithTTF(target_str, "fonts/watch.ttf", 25);
	targetScore_label->setPosition(visibleSize.width / 2+10, 750);
	targetScore_label->setOpacity(0);
	ActionInterval * delay_ts= DelayTime::create(4.2);   //每个sprite出现间隔0.4秒
	auto fi_ts = FadeIn::create(0.5);
	auto seq_ts = Sequence::create(delay_ts, fi_ts, NULL);
	targetScore_label->runAction(seq_ts);
	addChild(targetScore_label,5);
	


	level_label = Label::createWithTTF("Lv:", "fonts/ethnocentricrg.ttf", 30);
	level_label->setPosition(visibleSize.width / 2 - 180, visibleSize.height - 160);
	addChild(level_label,2);
	level_label->setOpacity(0);
	ActionInterval * delay_ll = DelayTime::create(5.4);   //每个sprite出现间隔0.4秒
	auto fi_ll = FadeIn::create(0.5);
	auto seq_ll = Sequence::create(delay_ll, fi_ll, NULL);
	level_label->runAction(seq_ll);


	
	

	//目标字体
	const char *str_action = ((String*)dic->objectForKey("target_score"))->_string.c_str();
	auto label2 = Label::createWithTTF(str_action, "fonts/b.ttf", 18);
	label2->setPosition(visibleSize.width / 2 - 40, 750);
	label2->setOpacity(0);
	ActionInterval * delay_la = DelayTime::create(3.8);   //每个sprite出现间隔0.4秒
	auto fi_la = FadeIn::create(0.5);
	auto seq_la = Sequence::create(delay_la, fi_la, NULL);
	label2->runAction(seq_la);
	addChild(label2, 3);

	
	log("cur level: %d", curLevel);
	char target_level[20];
	sprintf(target_level, "%d", curLevel);
	string target_str1 = target_level;
	levelNum_label = Label::createWithTTF(target_str1, "fonts/ethnocentricrg.ttf", 40);
	levelNum_label->setPosition(visibleSize.width / 2 - 120, visibleSize.height - 157);
	addChild(levelNum_label, 2);
	levelNum_label->setOpacity(0);
	ActionInterval * delay_lnl = DelayTime::create(5.6);   //每个sprite出现间隔0.4秒
	auto fi_lnl = FadeIn::create(0.5);
	auto seq_lnl = Sequence::create(delay_lnl, fi_lnl, NULL);
	levelNum_label->runAction(seq_lnl);



	char c[10];
	sprintf(c, "%d", curLevel);
	string str = c;
	string city = "city" + str;
	Sprite * bg = Sprite::create(city+".png");
	bg->setAnchorPoint(Vec2(0,0));
	addChild(bg,1);

	timerRunning = false;
	gameIsOver = false;

	//知识label
	knowledge_label = Label::createWithTTF("", "fonts/b.ttf", 20);
	knowledge_label->setPosition(visibleSize.width / 2-85, 40);
	knowledge_label->setAnchorPoint(Point(0, 0));
	knowledge_label->setDimensions(300, 60);
	addChild(knowledge_label, 6);

	small_pic = Sprite::create("level1_1.png");
	small_pic->setPosition(visibleSize.width / 2 - 160, 80);
	small_pic->setScale(0.5);
	small_pic->setVisible(false);
	addChild(small_pic,5);

	name_label = Label::createWithTTF("evernote", "fonts/b.ttf", 20);
	name_label->setPosition(visibleSize.width / 2 - 160, 40);
	name_label->setVisible(false);
	addChild(name_label,5);
	
	

	//游戏对话框图片:显示每个icon的知识
	Sprite * dialog = Sprite::create("dialog.png");
	dialog->setPosition(visibleSize.width / 2+10, -100);
	dialog->setScaleY(1.4);
	auto mt_dialog = MoveTo::create(1, Vec2(visibleSize.width / 2 + 10, 90));
	ActionInterval * delay_dialog = DelayTime::create(2.7);
	EaseOut * dialog_slow = EaseOut::create(mt_dialog, 7.0);	  
	//由快到慢的动作，第二个参数是变化率
	auto seq_dialog = Sequence::create(delay_dialog,dialog_slow, NULL);
	dialog->runAction(seq_dialog);
	addChild(dialog,2);

	
	//背景图片,3个最上面的
	Sprite * m1 = Sprite::create("menu.png");
	m1->setScaleX(0.8);
	m1->setScaleY(0.8);
	m1->setPosition(visibleSize.width / 2, visibleSize.height +80);                                       //dellta Y = 130
	auto mt_m1 = MoveTo::create(1, Vec2(visibleSize.width / 2, visibleSize.height - 50));
	ActionInterval * delay_m1 = DelayTime::create(2.7);
	EaseOut * m1_slow = EaseOut::create(mt_m1, 7.0);
	//由快到慢的动作，第二个参数是变化率
	auto seq_m1 = Sequence::create(delay_m1, m1_slow, NULL);
	m1->runAction(seq_m1);
	addChild(m1,2);

	Sprite * m2 = Sprite::create("menu.png");
	m2->setScaleX(0.8);
	m2->setScaleY(0.8);
	m2->setPosition(visibleSize.width / 2-140, visibleSize.height +55);
	auto mt_m2 = MoveTo::create(1, Vec2(visibleSize.width / 2 - 140, visibleSize.height - 75));
	ActionInterval * delay_m2 = DelayTime::create(2.7);
	EaseOut * m2_slow = EaseOut::create(mt_m2, 7.0);
	//由快到慢的动作，第二个参数是变化率
	auto seq_m2 = Sequence::create(delay_m2, m2_slow, NULL);
	m2->runAction(seq_m2);
	addChild(m2,2);

	Sprite * m3 = Sprite::create("menu.png");
	m3->setScaleX(0.8);
	m3->setScaleY(0.8);
	m3->setPosition(visibleSize.width / 2+140, visibleSize.height +55);
	auto mt_m3 = MoveTo::create(1, Vec2(visibleSize.width / 2 + 140, visibleSize.height - 75));
	ActionInterval * delay_m3 = DelayTime::create(2.7);
	EaseOut * m3_slow = EaseOut::create(mt_m3, 7.0);
	//由快到慢的动作，第二个参数是变化率
	auto seq_m3 = Sequence::create(delay_m3, m3_slow, NULL);	
	m3->runAction(seq_m3);
	addChild(m3,2);

	

	

	//返回主菜单和重新开始按钮
	btm = MenuItemImage::create("backToMenu.png", "backToMenu.png", CC_CALLBACK_1(HelloWorld::returnToMenu, this));
	btm->setAnchorPoint(Point(0,0));
	btm->setScale(0.45);
	auto btm_menu = Menu::create(btm, NULL);
	btm_menu->setPosition(visibleSize.width / 2 - 200, visibleSize.height - 105);
	btm_menu->setOpacity(0);
	ActionInterval * delay_btm = DelayTime::create(3.0);
	auto fi_btm = FadeIn::create(0.5);
	auto seq_btm = Sequence::create(delay_btm, fi_btm,NULL);
	btm_menu->runAction(seq_btm);
	addChild(btm_menu,3);

	res = MenuItemImage::create("restart.png", "restart.png", CC_CALLBACK_1(HelloWorld::restartGame, this));
	res->setAnchorPoint(Point(0, 0));
	res->setScale(0.45);
	auto res_menu = Menu::create(res, NULL);
	res_menu->setPosition(visibleSize.width / 2 - 140, visibleSize.height - 100);
	res_menu->setOpacity(0);
	ActionInterval * delay_res = DelayTime::create(3.4);   //每个sprite出现间隔0.4秒
	auto fi_res = FadeIn::create(0.5);
	auto seq_res = Sequence::create(delay_res, fi_res, NULL);
	res_menu->runAction(seq_res);
	addChild(res_menu, 3);

	//初始化分数
	int tempICS = getDataManager().getCurrentScore();
	getDataManager().setInitCurrentScore(tempICS);


	//当前分数
	currentScore = getDataManager().getCurrentScore();
	log("current score init: %d", currentScore);
	char s[20];
	sprintf(s, "%d", currentScore);
	label_score = Label::createWithTTF(s, "fonts/HANGTHEDJ.ttf", 50);
	label_score->setPosition(visibleSize.width / 2-15, visibleSize.height - 150);
	addChild(label_score,3);
	auto st = ScaleBy::create(1.0, 1.1);
	auto sq = Sequence::create(st, st->reverse(), NULL);
	auto rp = RepeatForever::create(sq);
	label_score->runAction(rp);
	label_score->setOpacity(0);
	ActionInterval * delay_ls = DelayTime::create(6);   //每个sprite出现间隔0.4秒
	auto fi_ls = FadeIn::create(0.5);
	auto seq_ls = Sequence::create(delay_ls, fi_ls, NULL);
	label_score->runAction(seq_ls);

	//游戏结束对话框
	gameEnd = Sprite::create("gameEnd.png");
	gameEnd->setPosition(visibleSize.width / 2, visibleSize.height - 450);
	gameEnd->setScale(0.85);
	addChild(gameEnd, 5);
	gameEnd->setVisible(false);

	
	


	//初始化数据统计
	statistic_LeftIcon = 0;
	statistic_LeftBonus = 0;
	statistic_SingleMost = 0;
	statistic_MostIcon = 0;
	statistic_Time = 0;

    four_kill = 0;
	five_kill = 0;
	six_kill = 0;
	seven_kill = 0;
	legendary_kill = 0;

	//////////////////////////////////////////数据统计
	const char *str_1 = ((String*)dic->objectForKey("statistic"))->_string.c_str();
	 statisticTitle = Label::createWithTTF(str_1,"fonts/b.ttf", 30);
	statisticTitle->setPosition(visibleSize.width / 2, visibleSize.height - 320);
	statisticTitle->setVisible(false);
	addChild(statisticTitle, 6);
	
	//剩余图标
	 pointer1 = Sprite::create("pointer.png");
	pointer1->setPosition(visibleSize.width / 2 - 190, visibleSize.height - 380);
	pointer1->setRotation(-90);
	pointer1->setVisible(false);
	addChild(pointer1, 6);

	const char *str_2 = ((String*)dic->objectForKey("lefticon"))->_string.c_str();
	title1 = Label::createWithTTF(str_2, "fonts/b.ttf", 20);
	title1->setPosition(visibleSize.width / 2 - 130, visibleSize.height - 380);
	title1->setVisible(false);
	addChild(title1, 6);

	char sLabel1[20];
	sprintf(sLabel1, "%d", statistic_LeftIcon);
	leftIcon_label = Label::createWithTTF(sLabel1, "fonts/watch.ttf", 25);
	leftIcon_label->setPosition(visibleSize.width / 2 - 30, visibleSize.height - 380);
	leftIcon_label->setVisible(false);
	addChild(leftIcon_label, 6);

	//剩余奖励
	pointer2 = Sprite::create("pointer.png");
	pointer2->setPosition(visibleSize.width / 2 - 190, visibleSize.height - 420);
	pointer2->setRotation(-90);
	pointer2->setVisible(false);
	addChild(pointer2, 6);

	const char *str_3 = ((String*)dic->objectForKey("leftbonus"))->_string.c_str();
	 title2 = Label::createWithTTF(str_3, "fonts/b.ttf", 20);
	title2->setPosition(visibleSize.width / 2 - 130, visibleSize.height - 420);
	title2->setVisible(false);
	addChild(title2, 6);

	char sLabel2[20];
	sprintf(sLabel2, "%d", statistic_LeftBonus);
	leftBonus_label = Label::createWithTTF(sLabel2, "fonts/watch.ttf", 25);
	leftBonus_label->setPosition(visibleSize.width / 2 - 30, visibleSize.height - 420);
	leftBonus_label->setVisible(false);
	addChild(leftBonus_label, 6);

	//单个最多
	pointer3 = Sprite::create("pointer.png");
	pointer3->setPosition(visibleSize.width / 2 - 190, visibleSize.height - 460);
	pointer3->setRotation(-90);
	pointer3->setVisible(false);
	addChild(pointer3, 6);

	const char *str_4 = ((String*)dic->objectForKey("singlerecord"))->_string.c_str();
	title3 = Label::createWithTTF(str_4, "fonts/b.ttf", 20);
	title3->setPosition(visibleSize.width / 2 - 130, visibleSize.height - 460);
	title3->setVisible(false);
	addChild(title3, 6);

	char sLabel4[20];
	sprintf(sLabel4, "%d", statistic_SingleMost);
	singleMost_label = Label::createWithTTF(sLabel4, "fonts/watch.ttf", 25);
	singleMost_label->setPosition(visibleSize.width / 2 - 30, visibleSize.height - 460);
	singleMost_label->setVisible(false);
	addChild(singleMost_label, 6);

	//消去最多的icon
	pointer4 = Sprite::create("pointer.png");
	pointer4->setPosition(visibleSize.width / 2 - 190, visibleSize.height - 500);
	pointer4->setRotation(-90);
	pointer4->setVisible(false);
	addChild(pointer4, 6);

	const char *str_5 = ((String*)dic->objectForKey("mosterase"))->_string.c_str();
	title4 = Label::createWithTTF(str_5, "fonts/b.ttf", 20);
	title4->setPosition(visibleSize.width / 2 - 130, visibleSize.height - 500);
	title4->setVisible(false);
	addChild(title4, 6);

	char sLabel5[20];
	sprintf(sLabel5, "%d", statistic_MostIcon);
	mostIcon_label = Label::createWithTTF(sLabel5, "fonts/watch.ttf", 25);
	mostIcon_label->setPosition(visibleSize.width / 2 - 30, visibleSize.height - 500);
	mostIcon_label->setVisible(false);
	addChild(mostIcon_label, 6);

	//时间
	pointer5 = Sprite::create("pointer.png");
	pointer5->setPosition(visibleSize.width / 2 - 190, visibleSize.height - 540);
	pointer5->setRotation(-90);
	pointer5->setVisible(false);
	addChild(pointer5, 6);

	const char *str_6 = ((String*)dic->objectForKey("time"))->_string.c_str();
	title5 = Label::createWithTTF(str_6, "fonts/b.ttf", 20);
	title5->setPosition(visibleSize.width / 2 - 130, visibleSize.height - 540);
	title5->setVisible(false);
	addChild(title5, 6);

	char sLabel6[20];
	sprintf(sLabel6, "%f", statistic_Time);
	timer_label = Label::createWithTTF(sLabel6, "fonts/watch.ttf", 25);
	timer_label->setPosition(visibleSize.width / 2 - 30, visibleSize.height - 540);
	timer_label->setVisible(false);
	addChild(timer_label, 6);

	
	

	//////显示4~8杀的label
	//4杀
	pointer6 = Sprite::create("pointer.png");
	pointer6->setPosition(visibleSize.width / 2 +30, visibleSize.height - 380);
	pointer6->setRotation(-90);
	pointer6->setVisible(false);
	addChild(pointer6, 6);

	const char *str_7 = ((String*)dic->objectForKey("fourkill"))->_string.c_str();
	title6 = Label::createWithTTF(str_7, "fonts/b.ttf", 20);
	title6->setPosition(visibleSize.width / 2 +70, visibleSize.height - 380);
	title6->setVisible(false);
	addChild(title6, 6);

	
	
	char sLabel7[20];
	sprintf(sLabel7, "%d", four_kill);
	fourKill_label = Label::createWithTTF(sLabel7, "fonts/watch.ttf", 25);
	fourKill_label->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 380);
	fourKill_label->setVisible(false);
	addChild(fourKill_label, 6);

	//5杀
	pointer7 = Sprite::create("pointer.png");
	pointer7->setPosition(visibleSize.width / 2 + 30, visibleSize.height - 420);
	pointer7->setRotation(-90);
	pointer7->setVisible(false);
	addChild(pointer7,6);

	const char *str_8 = ((String*)dic->objectForKey("fivekill"))->_string.c_str();
	title7 = Label::createWithTTF(str_8, "fonts/b.ttf", 20);
	title7->setPosition(visibleSize.width / 2 + 70, visibleSize.height - 420);
	title7->setVisible(false);
	addChild(title7, 6);

	char sLabel8[20];
	sprintf(sLabel8, "%d", five_kill);
	fiveKill_label = Label::createWithTTF(sLabel8, "fonts/watch.ttf", 25);
	fiveKill_label->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 420);
	fiveKill_label->setVisible(false);
	addChild(fiveKill_label, 6);


	//6杀
	pointer8 = Sprite::create("pointer.png");
	pointer8->setPosition(visibleSize.width / 2 + 30, visibleSize.height - 460);
	pointer8->setRotation(-90);
	pointer8->setVisible(false);
	addChild(pointer8, 6);

	const char *str_9 = ((String*)dic->objectForKey("sixkill"))->_string.c_str();
	title8 = Label::createWithTTF(str_9, "fonts/b.ttf", 20);
	title8->setPosition(visibleSize.width / 2 + 70, visibleSize.height - 460);
	title8->setVisible(false);
	addChild(title8, 6);

	char sLabel9[20];
	sprintf(sLabel9, "%d", six_kill);
	sixKill_label = Label::createWithTTF(sLabel8, "fonts/watch.ttf", 25);
	sixKill_label->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 460);
	sixKill_label->setVisible(false);
	addChild(sixKill_label, 6);

	//7杀
	pointer9 = Sprite::create("pointer.png");
	pointer9->setPosition(visibleSize.width / 2 + 30, visibleSize.height - 500);
	pointer9->setRotation(-90);
	pointer9->setVisible(false);
	addChild(pointer9, 6);

	const char *str_10 = ((String*)dic->objectForKey("sevenkill"))->_string.c_str();
	title9 = Label::createWithTTF(str_10, "fonts/b.ttf", 20);
	title9->setPosition(visibleSize.width / 2 + 70, visibleSize.height - 500);
	title9->setVisible(false);
	addChild(title9, 6);

	char sLabel10[20];
	sprintf(sLabel10, "%d", seven_kill);
	sevenKill_label = Label::createWithTTF(sLabel10, "fonts/watch.ttf", 25);
	sevenKill_label->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 500);
	sevenKill_label->setVisible(false);
	addChild(sevenKill_label, 6);

	//超神
	pointer10 = Sprite::create("pointer.png");
	pointer10->setPosition(visibleSize.width / 2 + 30, visibleSize.height - 540);
	pointer10->setRotation(-90);
	pointer10->setVisible(false);
	addChild(pointer10, 6);

	const char *str_11 = ((String*)dic->objectForKey("legendary"))->_string.c_str();
	title10 = Label::createWithTTF(str_11, "fonts/b.ttf", 20);
	title10->setPosition(visibleSize.width / 2 + 70, visibleSize.height - 540);
	title10->setVisible(false);
	addChild(title10, 6);

	char sLabel11[20];
	sprintf(sLabel11, "%d", legendary_kill);
	legendaryKill_label = Label::createWithTTF(sLabel11, "fonts/watch.ttf", 25);
	legendaryKill_label->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 540);
	legendaryKill_label->setVisible(false);
	addChild(legendaryKill_label, 6);

	/////////////////////////////////////////////////////没问题
	

	//剩余奖励icon
	leftBonusBox = Sprite::create("leftBonus.png");                                    //大小写问题，vs忽略大小写，adnroid不忽略，卧槽
	leftBonusBox->setPosition(visibleSize.width / 2 , visibleSize.height/2);
	leftBonusBox->setScale(1);
	leftBonusBox->setOpacity(0);
	addChild(leftBonusBox, 6);

	

	nextLevel = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(HelloWorld::goToNextLevel, this));
	nextLevel->setScale(0.5);
	menuNextLevel = Menu::create(nextLevel, NULL);
	menuNextLevel->setPosition(visibleSize.width / 2, visibleSize.height - 600);
	menuNextLevel->setVisible(false);
	//menuNextLevel->setVisible(true);
	addChild(menuNextLevel, 6);

	

	const char *str_nextLevel = ((String*)dic->objectForKey("nextLevel"))->_string.c_str();
	nextLevelLabel = Label::createWithTTF(str_nextLevel, "fonts/b.ttf", 20);
	nextLevelLabel->setPosition(visibleSize.width / 2 - 5, visibleSize.height - 600);
	addChild(nextLevelLabel, 7);
	nextLevelLabel->setVisible(false);
	
	

	//初始化数组
	pointArray = new PointArray();
	pointArray->initWithCapacity(100);
	generatePoint();

	

	

	

	//////////////////中文测试(要用edit plus打开cpp文件并且保存为utf-8才行,有点麻烦)

	//添加装备
	addEquipment();

	//初始化记录5个icon的数组
	icon_map.insert(make_pair(icon[0], 0));
	icon_map.insert(make_pair(icon[1], 0));
	icon_map.insert(make_pair(icon[2], 0));
	icon_map.insert(make_pair(icon[3], 0));
	icon_map.insert(make_pair(icon[4], 0));

	//触摸事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *t, Event *e){ //info要按引用访问&
		
		string info[20] = { "evernote", "forrst", "google", "instagram", "skype", "vimeo", "rss", "bloggr", "deviantart", "digg", "dribbble", "facebook", "flickr", "linkedin", "pinterest", "share", "stumble", "tumblr", "twitter", "youtube" };
		srand(time(NULL));

		
		//Rect game = Rect(25, 540, 450, 450);
		//如果在游戏区域内,才能继续判断
		if ((t->getLocation().x >= 25 && t->getLocation().x <= 475 && t->getLocation().y >= 145 && t->getLocation().y <= 590)&&isGameReady == true)
		{
			int row = (int)(590 - t->getLocation().y) / 45;
			int column = (int)(t->getLocation().x - 25) / 45;

			if (getDataManager().getChangeLayerAdded() == false && getDataManager().getBombLayerAdded()==false)
			{
				//log("x: %f y: %f\n", t->getLocation().x, t->getLocation().y);
				//log("矩阵坐标: x: %d y: %d", (int)(540 - t->getLocation().y) / 45, (int)(t->getLocation().x - 25) / 45);
				

				if (!timerRunning)
				{
					if (!isGameOver())
					{
						this->startTimer();//开始计时
					}

				}

				//计算与之相邻的相同装备数量
				int currentBoardValue = board[row][column];
				//0表示是空白的，只有不为0时，才能继续计算,游戏未结束才能点击
				if (currentBoardValue != 0 && !gameIsOver)
				{
					count = 0;//count为2时才能消除
					totalScore = 0;
					Sprite* tempEquip = getEquipWithCoordinate(equipment, row, column); //保存一个副本
					Point tempPoint = tempEquip->getPosition();

					//更新知识介绍label
					Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
					string tempStr = info[currentBoardValue - 1];//current-1
					
					log("currentbv-1: %d", currentBoardValue - 1);

					int randNumber = rand() % 4 + 1;
					log("num: %d", randNumber);
					char sNum[10] = "";
					sprintf(sNum, "%d", randNumber);
					string str = sNum;
					string out = tempStr + "_" + str;
					const char *str_action = ((String*)dic->objectForKey(out))->_string.c_str();
					knowledge_label->setString(str_action);

					//更新小图标显示
					char tempPic[10];
					sprintf(tempPic, "%d", currentBoardValue);
					string tempPicStr = tempPic;
					small_pic->setTexture("level1_" + tempPicStr + ".png");
					small_pic->setVisible(true);
					//更新label显示				
					name_label->setString(info[currentBoardValue - 1]);
					name_label->setVisible(true);

					//是否单独一个,这个函数用来避免点击单独的一个icon也会产生label的问题
					bool is = isSingleIcon(board, row, column, currentBoardValue);
					
					//找周围相同的,这里面就开始消除了
					findAdjacentEquip(board, row, column, currentBoardValue, is);
					
					//更新至多单次消去的个数,写入文件
					int tempSingleBest = UserDefault::getInstance()->getIntegerForKey("singleBest");
					if (count > tempSingleBest)
					{
						UserDefault::getInstance()->setIntegerForKey("singleBest",count);
						UserDefault::getInstance()->flush();
					}

					
					//4~8杀计数
					if (count >= 2)
					{
						switch (count)
						{
						case 2:	
							if (getDataManager().getEffect()==true)
							SimpleAudioEngine::getInstance()->playEffect("explosion1.OGG");							
							break;
						case 3:			
							if (getDataManager().getEffect() == true)
							SimpleAudioEngine::getInstance()->playEffect("explosion2.OGG");						
							break;
						case 4:
						{        if (!firstHit)
						         {
							if (getDataManager().getEffect() == true)
							{
								SimpleAudioEngine::getInstance()->playEffect("4kill.OGG");
								SimpleAudioEngine::getInstance()->playEffect("explosion3.OGG");
							}
						         }
								 four_kill++;
								 break;
						}
						case 5:
						{
								  five_kill++;
								  auto praise = Sprite::create("praise0.png");
								  praise->setPosition(tempPoint - Point(0, 70));
								  praise->setScale(0.001);
								  addChild(praise, 20);
								  auto st = ScaleTo::create(0.5, 1.5);
								  auto mt = MoveTo::create(1.5, Vec2(tempPoint + Point(0, 70)));
								  auto st2 = ScaleTo::create(0.5, 0.001);
								  auto deleteP = CallFunc::create([=](){
									  praise->removeFromParent();
								  });
								  auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
								  praise->runAction(seq);
								  if (!firstHit)
								  {
									  if (getDataManager().getEffect() == true)
									  {
										  SimpleAudioEngine::getInstance()->playEffect("5kill.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("explosion4.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("praise3.OGG");
									  }
								  }
								  break;
						}
						case 6:
						{
								  auto praise = Sprite::create("praise1.png");
								  praise->setPosition(tempPoint - Point(0, 70));
								  praise->setScale(0.001);
								  addChild(praise, 20);
								  auto st = ScaleTo::create(0.5, 1.5);
								  auto mt = MoveTo::create(1.5, Vec2(tempPoint + Point(0, 70)));
								  auto st2 = ScaleTo::create(0.5, 0.001);
								  auto deleteP = CallFunc::create([=](){
									  praise->removeFromParent();
								  });
								  auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
								  praise->runAction(seq);
								  six_kill++;
								  if (!firstHit)
								  {
									  if (getDataManager().getEffect() == true)
									  {
										  SimpleAudioEngine::getInstance()->playEffect("6kill.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("praise3.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("explosion4.OGG");
									  }
								  }
								  break;
						}
						case 7:
						{         auto praise = Sprite::create("praise2.png");
						          praise->setPosition(tempPoint - Point(0, 70));
						          praise->setScale(0.001);
						          addChild(praise, 20);
						          auto st = ScaleTo::create(0.5, 1.2);
						          auto mt = MoveTo::create(1.5, Vec2(tempPoint + Point(0, 70)));
						          auto st2 = ScaleTo::create(0.5, 0.001);
						          auto deleteP = CallFunc::create([=](){
							       praise->removeFromParent();
						          });
						          auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
						          praise->runAction(seq);
						          seven_kill++;
								  if (!firstHit)
								  {
									  if (getDataManager().getEffect() == true)
									  {
										  SimpleAudioEngine::getInstance()->playEffect("7kill.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("praise3.OGG");
										  SimpleAudioEngine::getInstance()->playEffect("explosion4.OGG");
									  }
								  }
						          break;
						}
						default: //超神
						{
								   //超神数加一
								   int tempLegend = UserDefault::getInstance()->getIntegerForKey("legendaryNum");
								   UserDefault::getInstance()->setIntegerForKey("legendaryNum", tempLegend + 1);
								   UserDefault::getInstance()->flush();

								   auto praise = Sprite::create("praise3.png");
								   praise->setPosition(tempPoint - Point(0, 70));
								   praise->setScale(0.001);
								   addChild(praise, 20);
								   auto st = ScaleTo::create(0.5, 1.0);
								   auto mt = MoveTo::create(1.5, Vec2(tempPoint + Point(0, 70)));
								   auto st2 = ScaleTo::create(0.5, 0.001);
								   auto deleteP = CallFunc::create([=](){
									   praise->removeFromParent();
								   });
								   auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
								   praise->runAction(seq);
								   legendary_kill++;
								   if (!firstHit)
								   { 
									   if (getDataManager().getEffect() == true)
									   {
										   SimpleAudioEngine::getInstance()->playEffect("8kill.OGG");
										   SimpleAudioEngine::getInstance()->playEffect("praise3.OGG");
										   SimpleAudioEngine::getInstance()->playEffect("explosion4.OGG");
									   }
								   }
						}
					}
						
					}

					//播放第一次击杀音效
					if (firstHit && count != 1)
					{
						firstHit = false;
						if (getDataManager().getEffect() == true)
						{
							SimpleAudioEngine::getInstance()->playEffect("firstBlood.OGG");
							SimpleAudioEngine::getInstance()->playEffect("explosion4.OGG");
						}
					}

					//更新单个消除的最高值
					if (count >= statistic_SingleMost)
					{
						statistic_SingleMost = count;
					}
					//记录5种icon的消去个数
					if (count > 1)
					{
						map<int, int> ::iterator map_it;
						map_it = icon_map.find(currentBoardValue);
						(*map_it).second += count;

					}

					if (count == 1)//只有一个
					{
						//加上该装备,不能复制指针
						char c[10];
						sprintf(c, "%d", currentBoardValue);
						string str = c;
						Sprite* sTemp = Sprite::create("level1_" + str + ".png");
						sTemp->setScale(0.36);
						sTemp->setAnchorPoint(Point(0, 0));
						equip.pushBack(sTemp);

						sTemp->setPosition(tempPoint);
						addChild(sTemp,2);
						equipment[row][column] = sTemp;
						//装备数组里得加上
						board[row][column] = currentBoardValue;


					}
					//log("before moveDown:\n");
					//printBoard(board);

					moveDown(board);
					moveLeft(board);
					//log("size of equip: %d", equip.size());

					//log("after moveDown:\n");
					//printBoard(board);

					//每次点击生成的点击总分
					if (count > 1)
					{
						//log("total score: %d", totalScore);
						char sl[20];
						sprintf(sl, "%d", totalScore);
						Label* totalScoreLabel;//上面的label
						totalScoreLabel = Label::createWithTTF(sl, "fonts/thg.ttf", 50);
						totalScoreLabel->setColor(Color3B(242, 221, 28));
						if (tempPoint.x < 100)
						{
							totalScoreLabel->setPosition(tempPoint + Point(60, 0));
						}
						else
						{
							totalScoreLabel->setPosition(tempPoint - Point(60, 0));
						}

						addChild(totalScoreLabel,4);

						Point tp = totalScoreLabel->getPosition();

						auto mt = MoveTo::create(1.0f, tp + Point(0, 80));

						auto fo = FadeOut::create(0.4);

						//问题：为啥不能把spawn用在sequence中
						//auto st = ScaleTo::create(0.4, 0.01);
						//auto sp = Spawn::create(fo, st);


						auto actionDelete = CallFunc::create([=](){

							totalScoreLabel->removeFromParent();
						});
						auto sq = Sequence::create(mt, fo, actionDelete, NULL);
						totalScoreLabel->runAction(sq);
					}

					
				}

			}//end  if changeLayerAdd
			else if(getDataManager().getChangeLayerAdded()==true) //改变board的值以及替换icon
			{
				//如果点击的icon与选取要变的不一样才能修改,-1是表示已经选取了5个要改变的icon之一
				if (getDataManager().getSelectedIconValue() != -1)
				{
					if (UserDefault::getInstance()->getIntegerForKey("ChangeNum")==0)
					{
						notEnoughItem();
					}


					//如果2者不一样才能修改
					int temp = getDataManager().getSelectedIconValue();
					//此道具数量大于0且要修改的是不同的icon
					if (temp != board[row][column] && UserDefault::getInstance()->getIntegerForKey("ChangeNum") > 0 && board[row][column] != 0)
					{
						Sprite* tempEquipIcon = getEquipWithCoordinate(equipment, row, column);
						Point targetPos = tempEquipIcon->getPosition();
						
						changeIconFlyAnimation(temp,targetPos);

						ActionInterval * delay_fly = DelayTime::create(1.0);
						auto change = CallFunc::create([=](){

							equipmentEffectWithNoGravity(board[row][column], targetPos);
							board[row][column] = temp;
							char cIcon[10];
							sprintf(cIcon, "%d", temp);
							string strIcon = cIcon;
							tempEquipIcon->setTexture("level1_" + strIcon + ".png");
							if (getDataManager().getEffect() == true)
							{
								SimpleAudioEngine::getInstance()->playEffect("change.OGG");
							}

						});
						auto sq = Sequence::create(delay_fly, change, NULL);
						this->runAction(sq);

													
						//数量减少1
						int tempNum = UserDefault::getInstance()->getIntegerForKey("ChangeNum");
						changeIconLabel->setString(StringUtils::format("%d", tempNum - 1));

						//写入文件
						UserDefault::getInstance()->setIntegerForKey("ChangeNum", tempNum - 1);
						UserDefault::getInstance()->flush();

						if (tempNum == 1)
						{
							changeIconLabel->setColor(Color3B(208, 52, 13));//数字变红色
						}

					}
					//一次点击的话只能修改一次，防止无限修改
					getDataManager().setSelectedIconValue(-1);
				}

			}
			else if (getDataManager().getBombLayerAdded() == true)
			{
				int i, j;
				if (UserDefault::getInstance()->getIntegerForKey("BombNum")<=0)
				{
					notEnoughItem();
				}

				if (missile1 == nullptr && missile2 == nullptr && UserDefault::getInstance()->getIntegerForKey("BombNum")>0)  //只有在2个导弹都飞出屏幕后才能继续使用bomb
				{
					
				   //创建光圈，然后变小圈住目标icon	
					if (board[row][column] != 0)
					{
					
						auto lock = Sprite::create("lock.png");
						lock->setScale(1.3);

						Sprite* tempEquipIcon = getEquipWithCoordinate(equipment, row, column);
						Point targetPos = tempEquipIcon->getPosition();
						lock->setPosition(targetPos + Point(20, 20));
						addChild(lock, 20);
						auto rt = RotateBy::create(1, 90);
						auto rp = RepeatForever::create(rt);
						auto st = ScaleTo::create(1.0, 0.3);
						lock->runAction(st);
						lock->runAction(rp);				

						//消除光圈
						//找到lock位置行列值较小的一个，计算时间
						float tempTime;
						if (10 - column > row + 1)
						{
							tempTime = (row + 1)*0.12;
						}
						else
						{
							tempTime = (10 - column)*0.12;
						}

						ActionInterval * delay_lock = DelayTime::create(tempTime + 2.4);
						auto deleteLock = CallFunc::create([=](){

							lock->removeFromParent();

						});

						auto sq_lock = Sequence::create(delay_lock, deleteLock, NULL);
						lock->runAction(sq_lock);
					}

					missile1 = Sprite::create("missile.png");
					missile1->setScale(0.8);
					missile1->setPosition(45 + column * 45, 550);
				
					//tower1 是x轴上的tower				
					if (tower1 == nullptr)
					{
						log("tower1 null");
						tower1 = Sprite::create("tower.png");
						tower1->setScale(0.4);
						tower1->setOpacity(0);
						tower1->setPosition(45 + column * 45, 600);
						addChild(tower1, 10);
						auto fi_t1 = FadeIn::create(0.5);
						tower1->runAction(fi_t1);

						addChild(missile1, 10);
						ActionInterval *delay_m1 = DelayTime::create(1.7);
						auto mt_m1 = MoveTo::create(2.0, Vec2(45 + column * 45, -100));

						auto deleteM1 = CallFunc::create([=](){

							moveDown(board);
							moveLeft(board);
							missile1->removeFromParentAndCleanup(true);
							missile1 = nullptr;

						});
						auto sq_m1 = Sequence::create(delay_m1, mt_m1, deleteM1, NULL);
						missile1->runAction(sq_m1);
						
						//消除icon的处理部分
						ActionInterval *delay_icon = DelayTime::create(1.7);
						auto hit = CallFunc::create([=](){

							hitIconByMissile(column,row,1); //type == 1  表示是按列消除，从上到下 ，0表示按行消除，从右到左

						});
						auto sq_deleteIcon = Sequence::create(delay_icon,hit, NULL);
						this->runAction(sq_deleteIcon);

					}
					else  //towerr存在的话就移动tower
					{
						auto mt_t1 = MoveTo::create(0.5, Vec2(45 + column * 45, 600));
						tower1->runAction(mt_t1);
						ActionInterval *delay_missile1 = DelayTime::create(1.5);
						auto addMissile1 = CallFunc::create([=](){

							addChild(missile1, 10);

						});
						auto sq_missile1 = Sequence::create(delay_missile1, addMissile1, NULL);
						this->runAction(sq_missile1);
						ActionInterval *delay_m1 = DelayTime::create(0.7);
						auto mt_m1 = MoveTo::create(2.0, Vec2(45 + column * 45, -100));
						auto deleteM1 = CallFunc::create([=](){

							moveDown(board);
							moveLeft(board);
							missile1->removeFromParentAndCleanup(true);
							missile1 = nullptr;

						});
						auto sq_m1 = Sequence::create(delay_m1, mt_m1, deleteM1, NULL);
						missile1->runAction(sq_m1);

						//消除icon的处理部分
						ActionInterval *delay_icon = DelayTime::create(2.2);
						auto hit = CallFunc::create([=](){

							hitIconByMissile(column, row, 1); //type == 1  表示是按列消除，从上到下 ，0表示按行消除，从右到左

						});
						auto sq_deleteIcon = Sequence::create(delay_icon, hit, NULL);
						this->runAction(sq_deleteIcon);

					}
					/////////////////////第二个导弹
					missile2 = Sprite::create("missile.png");
					missile2->setRotation(90);
					missile2->setScale(0.8);
					missile2->setPosition(430, 570 - row * 45);

					if (tower2 == nullptr)
					{
						//tower2 是y轴上的tower
						tower2 = Sprite::create("tower.png");
						tower2->setRotation(90);
						tower2->setScale(0.4);
						tower2->setOpacity(0);
						tower2->setPosition(480, 570 - row * 45);
						addChild(tower2, 10);
						auto fi_t2 = FadeIn::create(0.5);
						tower2->runAction(fi_t2);

						addChild(missile2, 10);
						auto mt_m2 = MoveTo::create(2.0, Vec2(-100, 570 - row * 45));
						
						ActionInterval *delay_m2 = DelayTime::create(1.7);
						auto deleteM2 = CallFunc::create([=](){

							moveDown(board);
							moveLeft(board);
							missile2->removeFromParentAndCleanup(true);
							missile2 = nullptr;

						});
						auto sq_m2 = Sequence::create(delay_m2, mt_m2, deleteM2, NULL);
						missile2->runAction(sq_m2);

						//消除icon的处理部分
						ActionInterval *delay_icon = DelayTime::create(1.7);
						auto hit = CallFunc::create([=](){

							hitIconByMissile(column, row, 0); //type == 1  表示是按列消除，从上到下 ，0表示按行消除，从右到左

						});
						auto sq_deleteIcon = Sequence::create(delay_icon, hit, NULL);
						this->runAction(sq_deleteIcon);

					}
					else
					{
						auto mt_t2 = MoveTo::create(0.5, Vec2(480, 570 - row * 45));
						tower2->runAction(mt_t2);
						ActionInterval *delay_missile2 = DelayTime::create(1.5);
						auto addMissile2 = CallFunc::create([=](){

							addChild(missile2, 10);

						});
						auto sq_missile2 = Sequence::create(delay_missile2, addMissile2, NULL);
						this->runAction(sq_missile2);

						ActionInterval *delay_m2 = DelayTime::create(0.7);
						auto deleteM2 = CallFunc::create([=](){

							moveDown(board);
							moveLeft(board);
							missile2->removeFromParentAndCleanup(true);
							missile2 = nullptr;

						});
						auto mt_m2 = MoveTo::create(2.0, Vec2(-100, 570 - row * 45));
						auto sq_m2 = Sequence::create(delay_m2, mt_m2, deleteM2, NULL);
						missile2->runAction(sq_m2);


						//消除icon的处理部分
						ActionInterval *delay_icon = DelayTime::create(2.2);
						auto hit = CallFunc::create([=](){

							hitIconByMissile(column, row, 0); //type == 1  表示是按列消除，从上到下 ，0表示按行消除，从右到左

						});
						auto sq_deleteIcon = Sequence::create(delay_icon, hit, NULL);
						this->runAction(sq_deleteIcon);

					}
					//数量减少1
					int tempNum = UserDefault::getInstance()->getIntegerForKey("BombNum");
					bombIconLabel->setString(StringUtils::format("%d", tempNum - 1));

					//写入文件
					UserDefault::getInstance()->setIntegerForKey("BombNum", tempNum - 1);
					UserDefault::getInstance()->flush();

					if (tempNum == 1)
					{
						bombIconLabel->setColor(Color3B(208, 52, 13));//数字变红色
					}
				}//end if nullptr								

			}
		}

		return true;
	};

	
	

	//listener1 专门用于lqy的处理
	auto listenerLQY = EventListenerTouchOneByOne::create();
	listenerLQY->setSwallowTouches(true);
	listenerLQY->onTouchBegan = [=](Touch *t, Event *e){ //info要按引用访问&
		
		if (tg->getBoundingBox().containsPoint(t->getLocation()))
		{
			lastPos = t->getLocation();
			isPlayed = false;
		
		}

		
		return true;
	
	};

	//end 更新lqy在y轴的位置
	listenerLQY->onTouchEnded = [=](Touch *t, Event *e){ //info要按引用访问&

		posY_lqy = tg->getPositionY();
		tg->stopAllActions();
		tg->girlAction();

		//点击lqy的处理
		if (tg->getBoundingBox().containsPoint(t->getLocation()))
		{
			lqy_dialog->setVisible(true);
			girlTalk->setVisible(true);


			if (tg->getPositionX() >= 250)
			{
				lqy_dialog->setPosition(tg->getPosition() - Point(130, 0));
				girlTalk->setPosition(tg->getPosition()  - Point(130, 0)+Point(0,-100));
			}
			else
			{
				lqy_dialog->setPosition(tg->getPosition() + Point(140, 0));
				girlTalk->setPosition(tg->getPosition() + Point(140, 0) + Point(0, -100));
			}
			ActionInterval * delay = DelayTime::create(3.0);
			auto invisible = CallFunc::create([=](){
				lqy_dialog->setVisible(false);
				girlTalk->setVisible(false);
			});
			auto seq = Sequence::create(delay, invisible, NULL);
			lqy_dialog->runAction(seq);

			//音效
			srand(time(NULL));
			int randNum = rand() % 9 + 1;
			char cT[10];
			sprintf(cT, "%d.wav", randNum);
			SimpleAudioEngine::getInstance()->playEffect(cT);

			Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml"); //必须包含进来
			char cTalk[10];
			sprintf(cTalk, "%d", randNum);
			string strT = cTalk;
			string strHead = "22_";
			const char *str_talk = ((String*)dic->objectForKey(strHead+cTalk))->_string.c_str();
			string strG = str_talk;
			girlTalk->setString(strG);



			//girlTalk->setString()

			//音效
			thisPos = t->getLocation();
			if (lastPos != thisPos)
			{
				//SimpleAudioEngine::getInstance()->playEffect("lqy_drag.mp3");
			}
		}

	};

	//move 拖动lqy
	listenerLQY->onTouchMoved = [=](Touch *t, Event *e){ //info要按引用访问&

		if (tg->getBoundingBox().containsPoint(t->getLocation()))
		{
			if (abs(t->getLocation().x - lastPos.x )> 30 || abs(t->getLocation().y - lastPos.y )> 30)
			{
				if (!isPlayed)
				{
					SimpleAudioEngine::getInstance()->playEffect("lqy_drag.wav");
					isPlayed = true;
				}
			}
			//更换图片
			tg->stopAllActions();
			tg->setTexture("22-h.png");

			if (t->getLocation().x >= 50 && t->getLocation().x <= 480 && t->getLocation().y >= 20 && t->getLocation().y <= 760)
			{
				tg->setPosition(t->getLocation());
			}
		}


	};

	

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerLQY, this);
	//定时检测游戏逻辑
	this->schedule(SEL_SCHEDULE(&HelloWorld ::gameStep), 0.02f); 




    
    return true;
}

void HelloWorld::changeIconFlyAnimation(int temp,Point target)
{
	//动画效果，一个icon飞过去替代原来的ICON
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point flyPos;
	char cFly[10];
	sprintf(cFly, "%d", temp);
	string strFly = cFly;
	Sprite * fly = Sprite::create("level1_" + strFly + ".png");
	fly->setAnchorPoint(Point(0, 0));
	vector<int> temp_vec = getDataManager().getIconVec();

	if (temp == temp_vec[0])
	{
		flyPos = Point(90, visibleSize.height - 110);
	}
	else if (temp == temp_vec[1])
	{
		flyPos = Point(90+80, visibleSize.height - 110);
	}
	else if (temp == temp_vec[2])
	{
		flyPos = Point(90 + 160, visibleSize.height - 110);
	}
	else if (temp == temp_vec[3])
	{
		flyPos = Point(90 + 240, visibleSize.height - 110);
	}
	else if (temp == temp_vec[4])
	{
		flyPos = Point(90 + 320, visibleSize.height - 110);
	}

	fly->setPosition(flyPos);
	fly->setScale(0.36);
	addChild(fly,15);

	auto mt = MoveTo::create(1, target);
	auto deleteFly = CallFunc::create([=](){

		fly->removeFromParent();

	});
	auto sq = Sequence::create(mt, deleteFly, NULL);
	fly->runAction(sq);
		
}

void HelloWorld::hitIconByMissile(int column, int row, int type)
{
	int i;
	
	if (type == 1)
	{
		for (i = 0; i < 10; i++)
		{
			ActionInterval * delay = DelayTime::create(0.12*i);
			if (board[i][column] != 0)
			{
				Sprite * temp = getEquipWithCoordinate(equipment, i, column);
				Point tempPoint = temp->getPosition();

				auto deleteSelf = CallFunc::create([=](){

					scoreGetFromMissile(i, column);
					equipmentEffect(board[i][column], tempPoint);
					temp->removeFromParent();
					board[i][column] = 0;
					if (getDataManager().getEffect() == true)
					{
						SimpleAudioEngine::getInstance()->playEffect("bomb.OGG");
					}
				});
				auto sq = Sequence::create(delay, deleteSelf, NULL);
				temp->runAction(sq);
			}

		}
	}
	else //type ==0 按行消除
	{
		for (i = 9; i >= 0; i--)
		{
			ActionInterval * delay = DelayTime::create(0.12*(9-i));
			if (board[row][i] != 0)
				{
					Sprite * temp = getEquipWithCoordinate(equipment,row, i);
					Point tempPoint = temp->getPosition();
					auto deleteSelf = CallFunc::create([=](){
						
						scoreGetFromMissile(row, i);
						equipmentEffect(board[row][i], tempPoint);
						temp->removeFromParent();
						board[row][i] = 0;
						if (getDataManager().getEffect() == true)
						{
							SimpleAudioEngine::getInstance()->playEffect("bomb.OGG");
						}
					});
					auto sq = Sequence::create(delay, deleteSelf, NULL);
					temp->runAction(sq);
				}

			
		}
	}
}

void HelloWorld::bombIconType(Ref * r)
{
	if (getDataManager().getEffect() == true && UserDefault::getInstance()->getIntegerForKey("BombNum") > 0)
	{
		SimpleAudioEngine::getInstance()->playEffect("button.OGG");
	}
	if (UserDefault::getInstance()->getIntegerForKey("BombNum")<=0)
	{
		notEnoughItem();
		SimpleAudioEngine::getInstance()->playEffect("buyFail.OGG");
	}
	else
	{
		if (getDataManager().getChangeLayerAdded() == false && getDataManager().getBombLayerAdded() == false) //如果已经添加了layer就不能再添加了
		{
			tower1 = nullptr;
			tower2 = nullptr;
			missile1 = nullptr;
			missile2 = nullptr;
			bombIconLayer * bil = bombIconLayer::create();
			bil->setPosition(0, 0);
			addChild(bil, 20);
			getDataManager().setBombLayerAdded(true);

		}
	}
}
//warning回调函数
void  HelloWorld::warningCallback(Ref *r)
{
	notEnoughMenu->setVisible(false);
	notEnough->setVisible(false);
	ok->setVisible(false);
	notEnoughDes->setVisible(false);
	yellow->setVisible(false);
}
void HelloWorld::notEnoughItem()
{

	//显示warning图片
	notEnoughMenu->setVisible(true);
	notEnough->setVisible(true);
	ok->setVisible(true);
	
	notEnoughDes->setVisible(true);
	auto fo = FadeOut::create(1.0);
	auto fi = FadeIn::create(1.0);
	auto sq = Sequence::create(fo, fi, NULL);
	auto rp = RepeatForever::create(sq);
	notEnoughDes->runAction(rp);
	yellow->setVisible(true);
}
void  HelloWorld::changeIconType(Ref * r)
{
	if (getDataManager().getEffect() == true && UserDefault::getInstance()->getIntegerForKey("ChangeNum") > 0)
	{
		SimpleAudioEngine::getInstance()->playEffect("button.OGG");
	}
	if (UserDefault::getInstance()->getIntegerForKey("ChangeNum") <= 0)
	{		
		notEnoughItem();
		SimpleAudioEngine::getInstance()->playEffect("buyFail.OGG");
	}
	else
	{

		if (getDataManager().getChangeLayerAdded() == false && getDataManager().getBombLayerAdded() == false) //如果已经添加了layer就不能再添加了
		{
			changeIconLayer * cil = changeIconLayer::create();
			cil->setPosition(0, 0);
			addChild(cil, 20);
			getDataManager().setChangeLayerAdded(true);
		}
	}
}
void HelloWorld::goToNextLevel(Ref * r)
{
	int tempIncrement = getDataManager().getScoreIncrement();
	getDataManager().setScoreIncrement(tempIncrement + 100);
	getDataManager().setCurrentScore(currentScore);

	//更新下一关目标分数
	getDataManager().setLevelTargetScore(target_score + tempIncrement);
	getDataManager().setGameLevel(curLevel + 1);
	//更新道具个数
	/*int tempChange = getDataManager().getChangeIconNum();
	getDataManager().setChangeIconNum(tempChange + 1);
	int tempBomb = getDataManager().getBombIconNum();
	getDataManager().setBombIconNum(tempBomb + 1);*/

	if (curLevel + 1 <= 20)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));
	}
	else
	{
		Director::getInstance()->replaceScene(TransitionFade::create(1, finalScene::createScene()));
	}
}
void HelloWorld::update(float dt)
{
	statistic_Time = clock() - startTime;
	//timerLabel->setString(StringUtils::format("%0.1f", ((double)statistic_Time) / 1000));   //真机上再除以1000才行 
}

void HelloWorld::startTimer()
{
	if (!timerRunning)
	{
		scheduleUpdate();
		startTime = clock();
		timerRunning = true;
	}
}

void HelloWorld::endTimer()
{
	if (timerRunning)
	{
		unscheduleUpdate();
		timerRunning = false;
	}
}

void HelloWorld::destoryRemaining()
{
	int row, column;
	//先遍历每一列，从左下角开始
	for (column = 0; column < 10; column++)
	{
		
		for (row = 9; row >= 0; row--)
		{
			float delay = column*0.4;
			ActionInterval * delaytime = CCDelayTime::create(delay);//延时动作,目的是为了让爆炸动画执行完毕
			if (board[row][column] != 0)
			{
				
				Sprite* tempEquip = getEquipWithCoordinate(equipment, row, column); //保存一个副本
				auto action = CallFunc::create([=](){
										
					Point tempPoint = tempEquip->getPosition();
					equipmentEffect(board[row][column], tempPoint);
					tempEquip->removeFromParent();
					//不能忘了让数组归0
					board[row][column] = 0;
				});
				auto sq = Sequence::create(delaytime, action, NULL);
				tempEquip->runAction(sq);
				
				

			}
		}
	}
}

bool HelloWorld::isSingleIcon(int b[][10], int row, int column, int cur)
{
	bool isSingle = true;
	if (b[row - 1][column] == cur || b[row + 1][column] == cur || b[row][column - 1] == cur || b[row][column + 1] == cur)
	{
		isSingle = false;
	}
	return isSingle;
}

void HelloWorld::gameStep(float dt)
{
	 
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//检查飞行的分数是否飞到位，如果到位了就去除之
	if (vec_label.size() != 0)
	{
		//log("size: %d",vec_label.size());
		//不能用迭代器
		int i;
		for (i = 0; i < vec_label.size();i++)
		{
			Label* temp  = vec_label.at(i);
			if (temp->getPosition() == label_score->getPosition())
			{
				
				int tempScore = temp->getTag();				
				currentScore += tempScore;
				label_score->setString(StringUtils::format("%d", currentScore));
				vec_label.eraseObject(temp);
				temp->removeFromParent();


			}
		}
	} 

	//检测是否到达目标分数
	if (currentScore >= getDataManager().getLevelTargetScore() && isTargetReached == false)
	{
		//这是为了让所有的分数都飘到位了，防止成功又失败的情况出现，也有问题，点快了的话size一直不为0 ，则gameover要判断为失败
		if (vec_label.size() == 0)
		{
			if (getDataManager().getEffect() == true)
			{
				SimpleAudioEngine::getInstance()->playEffect("game_level_win.OGG");
			}
			Size vs = Director::getInstance()->getVisibleSize();

			clear = Sprite::create("clear.png");
			clear->setPosition(vs.width / 2 + 130, vs.height / 2);
			clear->setScale(1.3);
			addChild(clear,10);

			auto fi_clear = FadeIn::create(0.5);
			auto st_clear = ScaleTo::create(0.5, 0.7);
			auto sp_clear = Spawn::create(fi_clear, st_clear, NULL);

			auto mt_clear = MoveTo::create(0.5, Point(vs.width / 2 + 180, vs.height - 170));
			auto st_clear2 = ScaleTo::create(0.5, 0.2);
			auto sp_clear2 = Spawn::create(mt_clear, st_clear2, NULL);

			ActionInterval * delay_clear = DelayTime::create(0.5);

			auto sq_clear = Sequence::create(sp_clear, delay_clear, sp_clear2, NULL);

			clear->runAction(sq_clear);

			//目标分数数字变颜色
			targetScore_label->setColor(Color3B(0, 255, 0));
			
			//达到目标分数
			isTargetReached = true;

			//显示特效
			winEffect(Point(vs.width / 2, vs.height - 200));
		}
		
	}

	//消去2个导弹
	if (getDataManager().getBombLayerAdded() == false)
	{
		if (tower1 != nullptr)
		{
			log("tower remove");
			tower1->removeFromParent();
			tower1 = nullptr; ////////////////不能少，remove之后不是将其变为nullptr
			
		}
		if (tower2 != nullptr)
		{
			tower2->removeFromParent();
			tower2 = nullptr;
		}
	}

	//检测是否游戏结束
	if (isGameOver() && isGameOverHit == false && getDataManager().getChangeLayerAdded() == false && getDataManager().getBombLayerAdded() == false)
	{
		log("game over");

		//计算剩余icon数量
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (board[i][j] != 0)
				{
					statistic_LeftIcon++;
				}
			}
		}
		//判断剩余奖励
		if (statistic_LeftIcon < 5)
		{
			statistic_LeftBonus = leftBonus[statistic_LeftIcon];
			leftBonusBox->setTag(statistic_LeftBonus);
			auto fi = FadeIn::create(0.5);
			auto mt = MoveTo::create(1.5, Vec2(visibleSize.width / 2 - 15, visibleSize.height - 150));
			auto deleteLbb = CallFunc::create([=](){

				currentScore += leftBonusBox->getTag();
				leftBonusBox->removeFromParent();
				label_score->setString(StringUtils::format("%d", currentScore));

			});
			auto seq = Sequence::create(fi, mt, deleteLbb, NULL);
			leftBonusBox->runAction(seq);




		}
		else
		{
			statistic_LeftBonus = 0;
		}

		//判断是否达成目标

		ActionInterval * delayJudge = DelayTime::create(3.0);
		auto judge = CallFunc::create([=](){

			if (isTargetReached) //如果目标达成
			{
				gameIsOver = true;
				//停止计时
				endTimer();

				//计算剩余icon数量
				statistic_LeftIcon = 0;
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						if (board[i][j] != 0)
						{
							statistic_LeftIcon++;
						}
					}
				}

				//clear label飞到中间来放大缩小
				auto mt_clear = MoveTo::create(0.5, Vec2(visibleSize.width / 2 + 100, visibleSize.height / 2 + 150));
				auto st_clear = ScaleTo::create(0.5, 0.5);
				auto sp_clear = Spawn::create(mt_clear, st_clear, NULL);

				clear2 = Sprite::create("clear.png");
				clear2->setPosition(visibleSize.width / 2 + 180, visibleSize.height - 170);
				clear2->setScale(0.2);
				addChild(clear2,10);
			    clear2->runAction(sp_clear);

				gameEnd->setVisible(true);
				//延时1秒执行摧毁动画
				ActionInterval * delaytime = CCDelayTime::create(2.0);
				//ActionInterval * delaytime2 = CCDelayTime::create(6.0);
				auto action = CallFunc::create([=](){

					destoryRemaining();
					//更新当前分数存进data manager

					getDataManager().setCurrentScore(currentScore);
					log("current score save: %d", currentScore);
				});
				/*auto action2 = CallFunc::create([=](){

				Director::getInstance()->replaceScene(TransitionFade::create(1, HelloWorld::createScene()));

				});*/

				auto sq = Sequence::create(delaytime, action, NULL);
				this->runAction(sq);

				//播放音效
				SimpleAudioEngine::getInstance()->playEffect("win.OGG");

				////////////////////////////////显示数据统计
				gameEnd->setVisible(true);
				//剩余图标
				statisticTitle->setVisible(true);
				pointer1->setVisible(true);
				title1->setVisible(true);
				leftIcon_label->setString(StringUtils::format("%d", statistic_LeftIcon));
				leftIcon_label->setVisible(true);


				leftBonus_label->setVisible(true);
				leftBonus_label->setString(StringUtils::format("%d", statistic_LeftBonus));
				pointer2->setVisible(true);
				title2->setVisible(true);

				//单个最多
				pointer3->setVisible(true);
				title3->setVisible(true);
				singleMost_label->setString(StringUtils::format("%d", statistic_SingleMost));
				singleMost_label->setVisible(true);

				//消去最多的icon	
				map<int, int> ::iterator map_it1;
				for (map_it1 = icon_map.begin(); map_it1 != icon_map.end(); map_it1++)
				{
					if (statistic_MostIcon < (*map_it1).second)
					{
						statistic_MostIcon = (*map_it1).second;
					}
				}
				pointer4->setVisible(true);
				title4->setVisible(true);
				mostIcon_label->setString(StringUtils::format("%d", statistic_MostIcon));
				mostIcon_label->setVisible(true);

				//时间
				pointer5->setVisible(true);
				title5->setVisible(true);
				timer_label->setVisible(true);
				timer_label->setString(StringUtils::format("%0.1f", statistic_Time / 1000000)); //真机得除1000000

				//4杀
				pointer6->setVisible(true);
				title6->setVisible(true);
				fourKill_label->setVisible(true);
				fourKill_label->setString(StringUtils::format("%d", four_kill));

				//5杀
				pointer7->setVisible(true);
				title7->setVisible(true);
				fiveKill_label->setVisible(true);
				fiveKill_label->setString(StringUtils::format("%d", five_kill));

				//6杀
				pointer8->setVisible(true);
				title8->setVisible(true);
				sixKill_label->setVisible(true);
				sixKill_label->setString(StringUtils::format("%d", six_kill));

				//7杀
				pointer9->setVisible(true);
				title9->setVisible(true);
				sevenKill_label->setVisible(true);
				sevenKill_label->setString(StringUtils::format("%d", seven_kill));

				//超神
				pointer10->setVisible(true);
				title10->setVisible(true);
				legendaryKill_label->setVisible(true);
				legendaryKill_label->setString(StringUtils::format("%d", legendary_kill));


				menuNextLevel->setVisible(true);
				nextLevelLabel->setVisible(true);


			}
			else//添加失败层
			{
				FailLayer *fl = FailLayer::create();
				fl->setPosition(0, 0);
				addChild(fl, 10);

				//播放音效
				SimpleAudioEngine::getInstance()->playEffect("lose.OGG");
				//更新当前分数
				getDataManager().setCurrentScore(currentScore);
			}

		});//end call func

		auto seq = Sequence::create(delayJudge, judge, NULL);
		if (isGameOverHit == false)
		{
			this->runAction(seq);
			isGameOverHit = true;
		}

	}
}

void HelloWorld::restartGame(cocos2d::Ref* r)
{
	if (getDataManager().getChangeLayerAdded() == false && getDataManager().getBombLayerAdded()==false) //如果已经添加了layer就不能再添加了
	{
		Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, HelloWorld::createScene()));
	}
}

void HelloWorld::returnToMenu(cocos2d::Ref* r)
{
	if (getDataManager().getChangeLayerAdded() == false&&getDataManager().getBombLayerAdded() == false) //如果已经添加了layer就不能再添加了
	{
		//在这里得更新当前分数,方便暂停层使用
		getDataManager().setCurrentScore(currentScore);


		Size size = Director::getInstance()->getVisibleSize();
		GamePauseLayer * gpl = GamePauseLayer::create();
		gpl->setPosition(0, 0);
		this->addChild(gpl, 9);
	}
}


bool HelloWorld::isGameOver()
{
	int i, j, count = 0;
	bool over = true;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (board[i][j] != 0)//别忘了这句话
			{
				if (board[i - 1][j] == board[i][j] || board[i + 1][j] == board[i][j] || board[i][j - 1] == board[i][j] || board[i][j + 1] == board[i][j])
				{

					over = false;
				}
			}
		}
	}

	return over;
}

Sprite* HelloWorld::getEquipWithCoordinate(Sprite * e[][10], int row, int column)
{
	if (e[row][column] != nullptr)
	{
		return e[row][column];
	}
	else
		return nullptr;
}

void HelloWorld::printBoard(int b[][10])
{
	for (int i = 0; i < 10; i++)
	{
		log("%d %d %d %d %d %d %d %d %d %d", b[i][0], b[i][1], b[i][2], b[i][3], b[i][4], b[i][5], b[i][6], b[i][7], b[i][8], b[i][9]);
		
	}
	log("\n");
}

void HelloWorld::moveLeft(int b[][10])
{
	
	
	//log("after moveleft:");
	//printBoard(board);
	
	//改正后的方法，从左到右一次检测每一列
	int lowerBound, i, j, spaceNum, tempLowerBound;
	for (i = 0; i <=8; i++)
	{
		lowerBound = -1;
		if (b[9][i] == 0 && b[9][i + 1] != 0)
		{
			lowerBound = i+1;//找到最下面一行从右至左的bound
			tempLowerBound = lowerBound;//下面要用lowerBound，所以预先保存
			
		}
		//如果lowerBound存在的话才能进一步判断
		if (lowerBound >= 0)
		{
			//log("lower_bound: %d", lowerBound);
			spaceNum = 0;
			while (b[9][lowerBound - 1] == 0)
			{
				spaceNum++;
				lowerBound--;
				if (lowerBound < 1)
					break;
			}
			//log("spaceNum: %d", spaceNum);
			//左移：从上到下一次遍历每一行
			for (int row = 0; row <= 9; row++)
			{
				for (int column = tempLowerBound; column <= 9; column++)
				{
					b[row][column - spaceNum] = b[row][column];
					//log("c-s: %d", column - spaceNum);
					if (b[row][column] != 0)
					{
						Sprite *s = getEquipWithCoordinate(equipment, row, column);
						Point temp = s->getPosition();
						float duration = (45 * spaceNum) / 200.0;
						//log("duration: %f", duration);
						auto mt = MoveTo::create(duration, temp - Point(45 * spaceNum, 0));
						s->runAction(mt);
						//更新equipment数组
						equipment[row][column - spaceNum] = s;
					}
				}
				//数组还原0
				for (int k = 9; k >= 9 - spaceNum + 1; k--)
				{
					b[row][k] = 0;
				}


			}


		}


	}
	




}

void HelloWorld::moveDown(int b[][10])
{
	//必须从下往上遍历才行

	int bound, i, j, k;
	//依次遍历每一列
	for (j = 0; j <= 9; j++)
	{
		
		for (i = 9; i>=0; i--)
		{
			bound = -1;//点击第一行时，bound为-1
			//找到分界点
			if (b[i][j] == 0 && b[i - 1][j] != 0)
			{
				bound = i-1;
				if (bound != -1)//必须，否则就不会落下
				{
					int spaceNum = 0;
					k = bound + 1;
					while (b[k][j] == 0)
					{
						spaceNum++;
						k++;
						if (k > 9)
							break;
					}
					//cout << "空格数为" << spaceNum << endl;
					//log("space :%d", spaceNum);
									
					//下移操作					
					b[bound + spaceNum][j] = b[bound][j];//这一句必须放在if外面,找了好久bug
											
				    Sprite *s = getEquipWithCoordinate(equipment, bound, j);
				    Point temp = s->getPosition();
					float duration = (45 * spaceNum) / 200.0;
					//log("duration: %f", duration);
					auto mt = MoveTo::create(duration, temp - Point(0, 45 * spaceNum));
					s->runAction(mt);
					//更新equipment数组
					equipment[bound + spaceNum][j] = s;			
					b[bound][j] = 0;//万万不能少
				}//end if

			}

		}
		//log("bound: %d", bound);
		//计算出空格数
		
	}//end j
}
void HelloWorld::equipmentEffectWithNoGravity(int boardValue, Point pos)
{
	char c[10];
	sprintf(c, "%d", boardValue);
	string str = c;
	//log("board value: %d", boardValue);

	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("level1_" + str + ".png"));
	effect->setTotalParticles(10);

	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255, 255, 255, 255));
	effect->setEndColor(Color4F(255, 255, 255, 0));

	effect->setStartSize(25.0f);

	effect->setLife(0.1f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);
	effect->setPosition(pos);
	this->addChild(effect,4);
}

void HelloWorld::winEffect(Point pos)
{
	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("smallstar.png"));
	effect->setTotalParticles(30);

	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255, 255, 255, 255));
	effect->setEndColor(Color4F(255, 255, 255, 0));

	effect->setStartSize(50.0f);
	effect->setGravity(Point(0, -300));
	effect->setLife(1.5f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);
	effect->setPosition(pos);
	this->addChild(effect,5);
}
void HelloWorld::equipmentEffect(int boardValue,Point pos)
{
	char c[10];
	sprintf(c, "%d", boardValue);
	string str = c;
	//log("board value: %d", boardValue);

	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("level1_"+str+".png"));
	effect->setTotalParticles(10);
	
	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255,255,255,255));
	effect->setEndColor(Color4F(255, 255, 255, 0));
	
	effect->setStartSize(25.0f);
	effect->setGravity(Point(0, -300));
	effect->setLife(0.1f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);
	effect->setPosition(pos);  
	this->addChild(effect,4);
}

void HelloWorld::findAdjacentEquip(int b[][10], int posX, int posY,int cur,bool single)
{
	
	//log("count: %d", count);
	
	if (b[posX][posY] != cur || posX<0 || posX>9 || posY<0 || posY>9)
	{
		
		return ;
	}
	
	//删除操作
	Sprite* sTemp = getEquipWithCoordinate(equipment, posX, posY);
	
	//(单个方块没有)粒子特效：装备向四周四散开来	
	if (count >= 1)
	{
		Point pos = sTemp->getPosition();
		equipmentEffect(b[posX][posY], pos);

	}
	
	
	b[posX][posY] = 0;
	count++;
	//游戏得分
	if (!single)
	{
		tempScore = count * 10-5;
		totalScore += tempScore;
		
		//创建label飞过来
		char s[20];
		sprintf(s, "%d", tempScore);
		lb = Label::createWithTTF(s, "fonts/ethnocentricrg.ttf", 30);
		lb->setPosition(sTemp->getPosition());
		//利用tag作为lb的分数，当然getDescription函数没搞懂
		lb->setTag(tempScore);
		
		//计算到达时间，根据距离来算
		float distance = sqrt((sTemp->getPositionX() - label_score->getPositionX())*(sTemp->getPositionX() - label_score->getPositionX()) + (sTemp->getPositionY() - label_score->getPositionY())*(sTemp->getPositionY() - label_score->getPositionY()));
		float time = distance/400;
		auto mt = MoveTo::create(time, label_score->getPosition());
		addChild(lb,4);
		vec_label.pushBack(lb);
		//延时
		float dt = count*0.05;
		ActionInterval * delaytime = CCDelayTime::create(dt);
		EaseOut * lb_easeIn = EaseOut::create(mt, 2.0);
		Sequence *sq = Sequence::create(delaytime, lb_easeIn, NULL);
		lb->runAction(sq);
	}
	

	
	if (sTemp != nullptr)
	{
		   
		    sTemp->removeFromParent();				
			equipment[posX][posY] = nullptr;
			equip.eraseObject(sTemp);
			
	}

	
	//递归
	findAdjacentEquip(b, posX - 1, posY, cur, single);
	findAdjacentEquip(b, posX + 1, posY, cur, single);
	findAdjacentEquip(b, posX, posY - 1, cur, single);
	findAdjacentEquip(b, posX, posY + 1, cur, single);

}
void HelloWorld::scoreGetFromMissile(int posX, int posY)
{
	int tempScore = 5;
	Sprite* sTemp = getEquipWithCoordinate(equipment, posX, posY);
	//创建label飞过来
	char s[20];
	sprintf(s, "%d", tempScore);
	lb = Label::createWithTTF(s, "fonts/ethnocentricrg.ttf", 30);
	lb->setPosition(sTemp->getPosition());
	//利用tag作为lb的分数，当然getDescription函数没搞懂
	lb->setTag(tempScore);

	//计算到达时间，根据距离来算
	float distance = sqrt((sTemp->getPositionX() - label_score->getPositionX())*(sTemp->getPositionX() - label_score->getPositionX()) + (sTemp->getPositionY() - label_score->getPositionY())*(sTemp->getPositionY() - label_score->getPositionY()));
	float time = distance / 400;
	auto mt = MoveTo::create(time, label_score->getPosition());
	addChild(lb,4);
	vec_label.pushBack(lb);
	
	lb->runAction(mt);

}

void HelloWorld::generatePoint()
{
	int i, j;
	srand(time(NULL));
	//20个图标中随机选取5个	
	
	PointArray *p = new PointArray();
	p->initWithCapacity(20);
	for (i = 1; i <= 20; i++)
	{
		p->addControlPoint(Point(i, 0));			
	}
	
	getDataManager().clearIconVec();
	for (i = 1; i <= 5; i++)
	{
		int randNum = rand() % p->count();
		int temp = (p->getControlPointAtIndex(randNum)).x;
		icon.push_back(temp);
		getDataManager().setIconVec(temp);
		p->removeControlPointAtIndex(randNum);
	}

	//log("%d %d %d %d %d", icon[0], icon[1], icon[2], icon[3], icon[4]);	
	
	for (i = 0; i < 10;i++)
	for (j = 0; j < 10;j++)
	{
		int min = 0, max = 5;
		int range = max - min;
		int number = rand() % range + 1;
		board[i][j] = icon[number-1];
		//board[i][j] = icon[0];
	
	}
	//board[0][0] = board[0][1] = board[1][0] = board[2][0] = board[3][0] =  board[3][1]=icon[1];

	
	

	
	
}

/*vector<int> HelloWorld::getIconVector()
{
	return icon;
}*/


void HelloWorld::addEquipment()
{
	

	int row, column;
	for (row = 0; row < 10; row++)
	{
		
		for (column = 0; column < 10; column++)
		{
			int temp = board[row][column];
			if (temp != 0)
			{
				char num[10] = "";
				sprintf(num, "%d", temp);
				string n = num;
				string name = "level1_" + n + ".png";
				Sprite * s = Sprite::create(name);
				s->setAnchorPoint(Point(0, 0));
				s->setScale(0.001);
				s->setPosition(Point(25 + column * 45, 550 - row * 45));
				addChild(s,2);
				//动画效果
				auto st = ScaleTo::create(0.8, 0.36);
				ActionInterval * delay = DelayTime::create(4);
				auto sq = Sequence::create(delay, st, NULL);
				s->runAction(sq);
							
				equip.pushBack(s);
				equipment[row][column] = s;//将装备放入对应的数组中
			}
		}
	}
	
	

}

int HelloWorld::getGlobalLevel()
{
	return 0;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
