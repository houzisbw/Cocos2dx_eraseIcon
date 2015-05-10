#include "isContinueLastPlay.h"
#include "HelloWorldScene.h"
#include "DataManager.h"
#include "SimpleAudioEngine.h"
#include "finalScene.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

isContinueLastPlay::isContinueLastPlay()
{
}


isContinueLastPlay::~isContinueLastPlay()
{
}
//继续上次的游戏
void isContinueLastPlay::gameStart(Ref * r)
{
	//改变音乐
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("air.OGG", true);

	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
	//设置当前关卡
	int tempLevel = UserDefault::getInstance()->getIntegerForKey("level_Global");
	getDataManager().setGameLevel(tempLevel);

	//设置当前分数
	int tempScore = UserDefault::getInstance()->getIntegerForKey("levelInitScore");
	getDataManager().setCurrentScore(tempScore);

	//设置目标分数
	int tempTargetScore = UserDefault::getInstance()->getIntegerForKey("level_Global_targetScore");
	getDataManager().setLevelTargetScore(tempTargetScore);


	Director::getInstance()->replaceScene(TransitionFade::create(2, HelloWorld::createScene()));
}
void isContinueLastPlay::gameStartNew(Ref * r)
{
	//改变音乐
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("air.OGG", true);

	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");

	getDataManager().setGameLevel(1);
	
	getDataManager().setCurrentScore(0);

	getDataManager().setLevelTargetScore(1000);


	Director::getInstance()->replaceScene(TransitionFade::create(2, HelloWorld::createScene()));
	//Director::getInstance()->replaceScene(TransitionFade::create(2, finalScene::createScene()));


}
bool isContinueLastPlay::init()
{
	Layer::init();
	Size size = Director::getInstance()->getVisibleSize();

	bg = Sprite::create("mainback.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg);

	auto listener = EventListenerTouchOneByOne::create();
	/*这里为了防止在该层点击能够点击到该层下面的精灵，所以要
	设置吞没函数，为true，而且ontouchbegin函数返要是true*/

	//选择文字
	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
	const char *str_des = ((String*)dic->objectForKey("isContinue1"))->_string.c_str();
	des = Label::createWithTTF(str_des, "fonts/b.ttf", 23);
	des->setPosition(size.width / 2, size.height / 2 + 210);
	addChild(des, 6);

	const char *str_des2 = ((String*)dic->objectForKey("isContinue2"))->_string.c_str();
	des2 = Label::createWithTTF(str_des2, "fonts/b.ttf", 23);
	des2->setPosition(size.width / 2+180, size.height / 2 + 210);
	addChild(des2, 6);

	//2个menu框
	left = MenuItemImage::create("menu.png", "menu.png",CC_CALLBACK_1(isContinueLastPlay::gameStart,this));
	left->setScale(0.5);
	auto leftMenu = Menu::create(left, NULL);
	leftMenu->setPosition(size.width / 2-15, size.height / 2 + 150);
	addChild(leftMenu, 6);

	right = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(isContinueLastPlay::gameStartNew, this));
	right->setScale(0.5);
	auto rightMenu = Menu::create(right, NULL);
	rightMenu->setPosition(size.width / 2 +130, size.height / 2 + 150);
	addChild(rightMenu, 6);

	//是和否的label
	const char *str_yes = ((String*)dic->objectForKey("yes"))->_string.c_str();
	yes = Label::createWithTTF(str_yes, "fonts/b.ttf", 25);
	yes->setPosition(size.width / 2 - 15, size.height / 2 + 150);
	addChild(yes, 7);

	const char *str_no = ((String*)dic->objectForKey("no"))->_string.c_str();
	no = Label::createWithTTF(str_no, "fonts/b.ttf", 25);
	no->setPosition(size.width / 2 + 130, size.height / 2 + 150);
	addChild(no, 7);

	//关卡数
	char c[10];
	int tempLevel = UserDefault::getInstance()->getIntegerForKey("level_Global");
	sprintf(c, "%d", tempLevel);
	string str = c;
	levelNum = Label::createWithTTF(str, "fonts/b.ttf", 25);
	levelNum->setColor(Color3B(242, 221, 28));
	levelNum->setPosition(size.width / 2+110, size.height / 2 + 210);
	addChild(levelNum, 7);


	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){

		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);



	return true;
}
