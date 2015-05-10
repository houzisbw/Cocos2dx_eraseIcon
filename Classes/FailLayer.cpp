#include "FailLayer.h"
#include "WelcomeScene.h"
#include "HelloWorldScene.h"
#include "DataManager.h"
USING_NS_CC;

bool FailLayer::init()
{
	Layer::init();
	
	//��xml�������ģ��ǵ�xml�ļ�Ҫ����resource�ļ����������
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size size = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("mainback.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg, 10);

	fail = Sprite::create("fail.png");
	fail->setPosition(size.width / 2+100, size.height / 2 + 100);
	fail->setScale(0.5);
	addChild(fail, 10);


	//��͸����ɫ����
	transBg = Sprite::create("transparent.png");
	transBg->setPosition(size.width / 2, size.height / 2);
	transBg->setScaleX(1.5);
	transBg->setScaleY(0.001);
	addChild(transBg, 7);
	
	auto st_bg = ScaleTo::create(0.5, 1.5, 0.8);  //�����x y��������ԭ��ͼƬ�Ĵ�С�ˣ���setscale֮��Ĵ�С��
	auto sq_bg = Sequence::create(st_bg, NULL);
	transBg->runAction(sq_bg);

	const char *str_restart = ((String*)dic->objectForKey("restartGame"))->_string.c_str();
	title = Label::createWithTTF(str_restart, "fonts/b.ttf", 32);
	title->setPosition(size.width/2,size.height/2+40);
	addChild(title, 12);

	//�� �� ��İ�ť
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

	//�� �� �������
	const char *str_yes = ((String*)dic->objectForKey("yes"))->_string.c_str();
	leftLabel = Label::createWithTTF(str_yes, "fonts/b.ttf", 25);
	leftLabel->setPosition(size.width / 2 - 105, size.height / 2 - 20);
	addChild(leftLabel,13);

	const char *str_no = ((String*)dic->objectForKey("no"))->_string.c_str();
	rightLabel = Label::createWithTTF(str_no, "fonts/b.ttf", 25);
	rightLabel->setPosition(size.width / 2 + 80, size.height / 2 - 20);
	addChild(rightLabel,13);


	auto listener = EventListenerTouchOneByOne::create();
	/*����Ϊ�˷�ֹ�ڸò����ܹ�������ò�����ľ��飬����Ҫ
	������û������Ϊtrue������ontouchbegin������Ҫ��true*/

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){

		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}
void FailLayer::yesButton(Ref * r)
{
	//��ʼ����ǰ����Ϊδ�����ʱ�ķ���
	int tempCurrent = getDataManager().getInitCurrentScore();
	getDataManager().setCurrentScore(tempCurrent);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(1.0f, HelloWorld::createScene()));
}
void FailLayer::noButton(Ref * r)
{
	//��ÿһ�س�ʼ����������ļ����´ν���ϷҪ��
	int tempCurrent = getDataManager().getInitCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("levelInitScore", tempCurrent);
		
	//�������ÿһ���浽�Ĺؿ��� 
	int tempLevel = getDataManager().getGameLevel();
	UserDefault::getInstance()->setIntegerForKey("level_Global", tempLevel);

	//���µ�ǰ����
	int tempScore = getDataManager().getCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("level_Global_currentScore", tempScore);

	//����Ŀ�����
	int tempTargetScore = getDataManager().getLevelTargetScore();
	UserDefault::getInstance()->setIntegerForKey("level_Global_targetScore", tempTargetScore);


	//�����ʷ��߷��Ƿ����
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
