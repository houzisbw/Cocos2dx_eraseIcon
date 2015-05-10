#include "FailLayer.h"
#include "WelcomeScene.h"
#include "HelloWorldScene.h"
#include "DataManager.h"
USING_NS_CC;

bool FailLayer::init()
{
	Layer::init();
	
	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size size = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("mainback.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg, 10);

	fail = Sprite::create("fail.png");
	fail->setPosition(size.width / 2+100, size.height / 2 + 100);
	fail->setScale(0.5);
	addChild(fail, 10);


	//半透明黑色背景
	transBg = Sprite::create("transparent.png");
	transBg->setPosition(size.width / 2, size.height / 2);
	transBg->setScaleX(1.5);
	transBg->setScaleY(0.001);
	addChild(transBg, 7);
	
	auto st_bg = ScaleTo::create(0.5, 1.5, 0.8);  //这里的x y比例不是原来图片的大小了，是setscale之后的大小了
	auto sq_bg = Sequence::create(st_bg, NULL);
	transBg->runAction(sq_bg);

	const char *str_restart = ((String*)dic->objectForKey("restartGame"))->_string.c_str();
	title = Label::createWithTTF(str_restart, "fonts/b.ttf", 32);
	title->setPosition(size.width/2,size.height/2+40);
	addChild(title, 12);

	//是 和 否的按钮
	left = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(FailLayer::yesButton, this));
	left->setScale(0.7);
	left->setScaleX(0.5);
	auto leftMenu = Menu::create(left, NULL);
	leftMenu->setPosition(size.width / 2 - 105,size.height/2-20);
	addChild(leftMenu,12);

	right = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(FailLayer::noButton, this));
	right->setScale(0.7);
	right->setScaleX(0.5);
	auto rightMenu = Menu::create(right, NULL);
	rightMenu->setPosition(size.width / 2 +80, size.height / 2 - 20);
	addChild(rightMenu, 12);

	//是 和 否的文字
	const char *str_yes = ((String*)dic->objectForKey("yes"))->_string.c_str();
	leftLabel = Label::createWithTTF(str_yes, "fonts/b.ttf", 25);
	leftLabel->setPosition(size.width / 2 - 105, size.height / 2 - 20);
	addChild(leftLabel,13);

	const char *str_no = ((String*)dic->objectForKey("no"))->_string.c_str();
	rightLabel = Label::createWithTTF(str_no, "fonts/b.ttf", 25);
	rightLabel->setPosition(size.width / 2 + 80, size.height / 2 - 20);
	addChild(rightLabel,13);


	auto listener = EventListenerTouchOneByOne::create();
	/*这里为了防止在该层点击能够点击到该层下面的精灵，所以要
	设置吞没函数，为true，而且ontouchbegin函数返要是true*/

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){

		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void FailLayer::yesButton(Ref * r)
{
	//初始化当前分数为未玩这关时的分数
	int tempCurrent = getDataManager().getInitCurrentScore();
	getDataManager().setCurrentScore(tempCurrent);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, HelloWorld::createScene()));
}
void FailLayer::noButton(Ref * r)
{
	//将每一关初始化分数存进文件，下次进游戏要用
	int tempCurrent = getDataManager().getInitCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("levelInitScore", tempCurrent);
		
	//这里更新每一次玩到的关卡数 
	int tempLevel = getDataManager().getGameLevel();
	UserDefault::getInstance()->setIntegerForKey("level_Global", tempLevel);

	//更新当前分数
	int tempScore = getDataManager().getCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("level_Global_currentScore", tempScore);

	//更新目标分数
	int tempTargetScore = getDataManager().getLevelTargetScore();
	UserDefault::getInstance()->setIntegerForKey("level_Global_targetScore", tempTargetScore);


	//检测历史最高分是否更新
	int tempBestHistory = getDataManager().getCurrentScore();
	int bestHistoryScore = UserDefault::getInstance()->getIntegerForKey("historyBestScore");
	if (tempBestHistory > bestHistoryScore)
	{
		UserDefault::getInstance()->setIntegerForKey("historyBestScore", tempBestHistory);

	}

	UserDefault::getInstance()->flush();
	Director::getInstance()->replaceScene(TransitionFade::create(1, WelcomeScene::createScene()));
}


FailLayer::FailLayer()
{
}


FailLayer::~FailLayer()
{
}
