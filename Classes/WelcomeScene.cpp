#include "WelcomeScene.h"
#include "HelloWorldScene.h"
#include "gameSelect.h"
#include "SimpleAudioEngine.h"
#include "Achievement.h"
#include "WorldMapScene.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* WelcomeScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = WelcomeScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool WelcomeScene::isSaveFile()
{
	//只有dubug模式下才看得见xml文档
	if (!UserDefault::getInstance()->getBoolForKey("exist"))
	{
		UserDefault::getInstance()->setBoolForKey("exist", true);

		//初始化环球旅行的关卡数0 和 初始化最高分0 和 目标分数
		UserDefault::getInstance()->setIntegerForKey("level_Global", 0);
		UserDefault::getInstance()->setIntegerForKey("level_Global_highestScore", 0);
		UserDefault::getInstance()->setIntegerForKey("level_Global_targetScore", 1000);
		UserDefault::getInstance()->setIntegerForKey("level_Global_currentScore", 0);
		//初始化历史最高分
		UserDefault::getInstance()->setIntegerForKey("historyBestScore", 0);
		//初始化单次消去记录
		UserDefault::getInstance()->setIntegerForKey("singleBest", 0);
		//初始化超神次数
		UserDefault::getInstance()->setIntegerForKey("legendaryNum", 0);
		//初始化道具个数
		UserDefault::getInstance()->setIntegerForKey("BombNum", 10);
		UserDefault::getInstance()->setIntegerForKey("ChangeNum", 10);
		//初始化init分数
		UserDefault::getInstance()->setIntegerForKey("levelInitScore", 0);

		//写入
		UserDefault::getInstance()->flush();
		return false;
	}
	else
		return true;
}


bool WelcomeScene::init()
{
	Layer::init();
	
	//预加载音乐和bgm
	//添加音乐 预加载
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("mswlm.OGG");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("air.OGG");
	
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("mswlm.OGG", true);
	}

    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.6);

	//添加音效预加载
	SimpleAudioEngine::getInstance()->preloadEffect("firstBlood.OGG");
	
	SimpleAudioEngine::getInstance()->preloadEffect("4kill.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("5kill.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("6kill.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("7kill.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("8kill.OGG");

	SimpleAudioEngine::getInstance()->preloadEffect("explosion1.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("explosion2.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("explosion3.OGG");

	SimpleAudioEngine::getInstance()->preloadEffect("praise3.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("win.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("lose.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("change.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("bomb.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("button.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("menu.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("game_level_win.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("achieve.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("buySuccess.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("buyFail.OGG");
	SimpleAudioEngine::getInstance()->preloadEffect("ready.OGG");

	//罗秋雨 语音
	SimpleAudioEngine::getInstance()->preloadEffect("lqy_drag.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("1.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("2.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("3.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("4.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("5.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("6.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("7.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("8.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("9.wav");


	SimpleAudioEngine::getInstance()->setEffectsVolume(0.7);

	//是否存在存档
	isSaveFile();

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	bg1 = Sprite::create("welcome_bg1.png");
	bg1->setAnchorPoint(Point(0, 0));
	bg1->setPosition(0, 0);
	addChild(bg1,1);

	bg2 = Sprite::create("welcome_bg1.png");
	bg2->setAnchorPoint(Point(0, 0));
	bg2->setPosition(0, bg1->getPositionY() + bg1->getContentSize().height - 2);
	addChild(bg2,1);

	MenuItemImage* menu1 = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(WelcomeScene::gameStart, this));
	menu1->setScaleX(1.0);
	menu1->setScaleY(0.8);
	auto menu = Menu::create(menu1, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height - 420);
	addChild(menu,2);

	MenuItemImage* menu2 = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(WelcomeScene::gameOption, this));
	menu2->setScaleX(1.0);
	menu2->setScaleY(0.8);
	auto menu5 = Menu::create(menu2, NULL);
	menu5->setPosition(visibleSize.width / 2, visibleSize.height - 520);
	addChild(menu5,2);
	
	MenuItemImage* menu3 = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(WelcomeScene::gameEnd, this));
	menu3->setScaleX(1.0);
	menu3->setScaleY(0.8);
	auto menuEnd = Menu::create(menu3, NULL);
	menuEnd->setPosition(visibleSize.width / 2, visibleSize.height - 620);
	addChild(menuEnd,2);



	cloud = Sprite::create("cloud.png");
	cloud->setAnchorPoint(Point(0, 0));
	cloud->setPosition(0, -80);
	addChild(cloud,2);

	//22娘
	gt = GirlTwo::create();
	gt->setPosition(visibleSize.width / 2 - 150, visibleSize.height / 2 + 240);
	gt->setScale(1.6);
	gt->setScaleY(1.8);
	addChild(gt,2);


	//girl1 = Sprite::create("girl1.png");
	//girl1->setScale(0.25);
	//girl1->setPosition(visibleSize.width / 2 - 250, visibleSize.height / 2+80);
	//girl1->setAnchorPoint(Point(0, 0));
	//addChild(girl1);

	
    const char *str_start = ((String*)dic->objectForKey("new"))->_string.c_str();
	menuTitle1 = Label::createWithTTF(str_start, "fonts/b.ttf", 50);
	menuTitle1->setColor(Color3B::WHITE);
	menuTitle1->setScale(0.60);
	menuTitle1->setAnchorPoint(Point(0, 0));//防止数字抖动
	menuTitle1->setPosition(visibleSize.width / 2-65, visibleSize.height - 438);
	addChild(menuTitle1,3);

	const char *str_op = ((String*)dic->objectForKey("option"))->_string.c_str();
	menuTitle2 = Label::createWithTTF(str_op, "fonts/b.ttf", 50);
	menuTitle2->setColor(Color3B::WHITE);
	menuTitle2->setScale(0.60);
	menuTitle2->setAnchorPoint(Point(0, 0));//防止数字抖动
	menuTitle2->setPosition(visibleSize.width / 2 - 65, visibleSize.height - 538);
	addChild(menuTitle2,3);

	const char *str_quit = ((String*)dic->objectForKey("quit"))->_string.c_str();
	menuTitle3 = Label::createWithTTF(str_quit, "fonts/b.ttf", 50);
	menuTitle3->setColor(Color3B::WHITE);
	menuTitle3->setScale(0.60);
	menuTitle3->setAnchorPoint(Point(0, 0));//防止数字抖动
	menuTitle3->setPosition(visibleSize.width / 2 -65, visibleSize.height - 638);
	addChild(menuTitle3,3);

	//标题
	const char *str_title1 = ((String*)dic->objectForKey("welcomeTitle1"))->_string.c_str();
	title1 = Label::createWithTTF(str_title1, "fonts/hbb.ttf", 130);
	
	title1->setPosition(visibleSize.width / 2+90, visibleSize.height / 2 + 320);
	title1->setColor(Color3B(255,255,255));
	addChild(title1,2);

	auto st = ScaleBy::create(1.0, 1.1);
	auto sq = Sequence::create(st, st->reverse(), NULL);
	auto rp = RepeatForever::create(sq);
	title1->runAction(rp);

	const char *str_title2 = ((String*)dic->objectForKey("welcomeTitle2"))->_string.c_str();
	title2 = Label::createWithTTF(str_title2, "fonts/hbb.ttf", 55);
	title2->setAnchorPoint(Point(0, 0));
	title2->setPosition(visibleSize.width / 2 - 20, visibleSize.height / 2 + 100);
	title2->setColor(Color3B(255, 255, 255));
	addChild(title2,2);

	auto lb1 = Sprite::create("leftBracket.png");
	lb1->setScale(0.06);
	lb1->setPosition(visibleSize.width / 2+20 , visibleSize.height / 2 + 170);
	addChild(lb1,2);

	auto lb2 = Sprite::create("rightBracket.png");
	lb2->setScale(0.06);
	lb2->setPosition(visibleSize.width / 2+160, visibleSize.height / 2 + 170);
	addChild(lb2,2);

	auto lb3 = Sprite::create("leftBracket.png");
	lb3->setScale(0.06);
	lb3->setPosition(visibleSize.width / 2 + 20, visibleSize.height / 2 + 100);
	lb3->setFlippedY(true);
	addChild(lb3,2);

	auto lb4 = Sprite::create("rightBracket.png");
	lb4->setScale(0.06);
	lb4->setPosition(visibleSize.width / 2 + 160, visibleSize.height / 2 + 100);
	lb4->setFlippedY(true);
	addChild(lb4,2);

	//6个图标
	auto icon1 = Sprite::create("level1_3.png");
	icon1->setPosition(visibleSize.width / 2-20 , visibleSize.height / 2 + 205);
	addChild(icon1,2);
	icon1->setScale(0.26);

	auto icon2 = Sprite::create("level1_7.png");
	icon2->setPosition(visibleSize.width / 2 +25, visibleSize.height / 2 + 205);
	addChild(icon2,2);
	icon2->setScale(0.26);

	auto icon3 = Sprite::create("level1_1.png");
	icon3->setPosition(visibleSize.width / 2 + 70, visibleSize.height / 2 + 205);
	addChild(icon3,2);
	icon3->setScale(0.26);

	auto icon4 = Sprite::create("level1_6.png");
	icon4->setPosition(visibleSize.width / 2 + 115, visibleSize.height / 2 + 205);
	addChild(icon4,2);
	icon4->setScale(0.26);

	auto icon5 = Sprite::create("level1_5.png");
	icon5->setPosition(visibleSize.width / 2 + 160, visibleSize.height / 2 + 205);
	addChild(icon5,2);
	icon5->setScale(0.26);

	auto icon6 = Sprite::create("level1_4.png");
	icon6->setPosition(visibleSize.width / 2 + 205, visibleSize.height / 2 + 205);
	addChild(icon6,2);
	icon6->setScale(0.26);


	//移动背景
	this->schedule(SEL_SCHEDULE(&WelcomeScene::move), 0.02f);


	return true;
}

void WelcomeScene::gameStart(Ref* r)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
	particleEffect(Point(visibleSize.width / 2, visibleSize.height - 420));
	Director::getInstance()->replaceScene(TransitionFade::create(2, gameSelect::createScene()));

	
}
void WelcomeScene::gameEnd(Ref * r)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
	Director::getInstance()->replaceScene(TransitionFade::create(2, WorldMapScene::createScene()));
	particleEffect(Point(visibleSize.width / 2, visibleSize.height - 620));
}

void WelcomeScene::gameOption(Ref *r)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
	Director::getInstance()->replaceScene(TransitionFade::create(2, Achievement::createScene()));
	particleEffect(Point(visibleSize.width / 2, visibleSize.height - 520));

	//改变音乐
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("air.mp3", true);
}
void WelcomeScene::move(float dt)
{
	Size visible = Director::getInstance()->getVisibleSize();
	
	float nextPos1 = bg1->getPositionY() - vy;
	float nextPos2 = bg2->getPositionY() - vy;
	bg1->setPositionY(nextPos1);
	bg2->setPositionY(nextPos2);

	if (nextPos1 < -bg1->getContentSize().height)
	{
		nextPos1 = bg2->getPositionY() + bg2->getContentSize().height - 1;
		bg1->setPositionY(nextPos1);
	}
	else if (nextPos2 < -bg2->getContentSize().height)
	{
		nextPos2 = bg1->getPositionY() + bg1->getContentSize().height - 1;
		bg2->setPositionY(nextPos2);
	}

	//云移动,横向移动
	float nextCloudX = cloud->getPositionX() - vx;
	cloud->setPositionX(nextCloudX);
	if (nextCloudX < -(cloud->getContentSize().width - visible.width))
	{
		vx = -vx;
		//log("%f", nextCloudX);
	}
	else if (nextCloudX > 0)
	{
		vx = -vx;
		//log("111");
	}

	//萌妹沿着y轴上下移动
	/*float nextGirlY = girl1->getPositionY() - vy_girl1;
	girl1->setPositionY(nextGirlY);
	if (nextGirlY <= visible.height / 2+50)
	{
		vy_girl1 = -vy_girl1;
	}
	if (nextGirlY >= visible.height / 2 + 80)
	{
		vy_girl1 = -vy_girl1;
	}*/



}
void WelcomeScene::particleEffect(Point pos)
{
	ParticleExplosion* effect = ParticleExplosion::create();
	effect->setTexture(Director::getInstance()->getTextureCache()->addImage("menu.png"));
	effect->setTotalParticles(10);

	//让其为图片本身的颜色
	effect->setStartColor(Color4F(255, 255, 255, 255));
	effect->setEndColor(Color4F(255, 255, 255, 0));

	effect->setStartSize(50.0f);

	effect->setLife(2.6f);
	effect->setSpeed(200);
	effect->setSpeedVar(10);
	effect->setPosition(pos);
	this->addChild(effect,5);
}

WelcomeScene::WelcomeScene() :
vy(5.0),
vx(2.0),
vy_girl1(0.5)
{
}


WelcomeScene::~WelcomeScene()
{
}
