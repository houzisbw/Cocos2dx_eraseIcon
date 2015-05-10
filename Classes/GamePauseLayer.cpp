#include "GamePauseLayer.h"
#include "DataManager.h"
#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

GamePauseLayer::GamePauseLayer()
{
}


GamePauseLayer::~GamePauseLayer()
{
}

void GamePauseLayer::onEnter()
{
	Layer::onEnter();
	this->setSwallowsTouches(true);
}

void GamePauseLayer::onExit()
{
	Layer::onExit();
	
}
void GamePauseLayer::resumeGame(cocos2d::Ref *r)
{
	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
	//去除该层,就相当于恢复游戏了
	this->removeFromParentAndCleanup(true);
}
void GamePauseLayer::goBackToMenu(cocos2d::Ref *r)
{

	SimpleAudioEngine::getInstance()->playEffect("menu.OGG");
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

	//将每一关初始化分数存进文件，下次进游戏要用
	int tempCurrent = getDataManager().getInitCurrentScore();
	UserDefault::getInstance()->setIntegerForKey("levelInitScore", tempCurrent);


	UserDefault::getInstance()->flush();

	Director::getInstance()->replaceScene(TransitionFade::create(1, WelcomeScene::createScene()));
}

bool GamePauseLayer::init()
{
	Layer::init();
	
	Size size = Director::getInstance()->getVisibleSize();
	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	bg = Sprite::create("mainback.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg,10);

	dialog = Sprite::create("gameEnd.png");
	dialog->setPosition(size.width / 2, size.height / 2);
	dialog->setScale(0.85);
	addChild(dialog, 11);

	//一些label
	const char *str_pause = ((String*)dic->objectForKey("gamePause"))->_string.c_str();
	pauseLabel = Label::createWithTTF(str_pause, "fonts/b.ttf", 40);
	pauseLabel->setPosition(size.width / 2, size.height - 280);
	addChild(pauseLabel,12);

	pointer1 = Sprite::create("pointer.png");
	pointer1->setPosition(size.width / 2 - 100, size.height - 280);
	pointer1->setScale(1.5);
	pointer1->setRotation(90);
	addChild(pointer1,12);

	pointer2 = Sprite::create("pointer.png");
	pointer2->setPosition(size.width / 2 + 100, size.height - 280);
	pointer2->setScale(1.5);
	pointer2->setRotation(-90);
	addChild(pointer2, 12);

	//继续游戏菜单
	resume = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(GamePauseLayer::resumeGame, this));
	resume->setScale(0.7);
	auto resumeMenu = Menu::create(resume, NULL);
	resumeMenu->setPosition(size.width / 2, size.height - 450);
	addChild(resumeMenu,12);

	const char *str_resume = ((String*)dic->objectForKey("continue"))->_string.c_str();
	label1 = Label::createWithTTF(str_resume, "fonts/b.ttf", 30);
	label1->setPosition(size.width / 2-5, size.height - 450);
	addChild(label1, 13);

	//返回主菜单
	mainMenu = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(GamePauseLayer::goBackToMenu, this));
	mainMenu->setScale(0.7);
	auto mainMenuMenu = Menu::create(mainMenu, NULL);
	mainMenuMenu->setPosition(size.width / 2, size.height - 525);
	addChild(mainMenuMenu, 12);

	const char *str_menu = ((String*)dic->objectForKey("saveExit"))->_string.c_str();
	label2 = Label::createWithTTF(str_menu, "fonts/b.ttf", 22);
	label2->setPosition(size.width / 2 - 5, size.height - 525);
	addChild(label2, 13);

	//背景音乐打开
	bgmSprite = Sprite::create("bgm_on.png");
	bgmSprite->setScale(1.3);
	bgm_on = MenuItemSprite::create(bgmSprite, bgmSprite, CC_CALLBACK_1(GamePauseLayer::bgmOnOff, this));
	auto menuBGMON = Menu::create(bgm_on, NULL);
	menuBGMON->setPosition(size.width / 2 - 80, size.height / 2 + 40);
	addChild(menuBGMON, 13);

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) 
	{
		bgmSprite->setTexture("bgm_off.png");
	}

	//游戏音效打开
	effectSprite = Sprite::create("sound_on.png");
	effectSprite->setScale(1.3);
	effect_on = MenuItemSprite::create(effectSprite, effectSprite, CC_CALLBACK_1(GamePauseLayer::effectOnOff, this));
	auto menuEFFECTON = Menu::create(effect_on, NULL);
	menuEFFECTON->setPosition(size.width / 2 + 30, size.height / 2 + 40);
	addChild(menuEFFECTON, 13);

	if (getDataManager().getEffect() == false) 
	{
		effectSprite->setTexture("sound_off.png");
	}


	auto listener = EventListenerTouchOneByOne::create();
	/*这里为了防止在该层点击能够点击到该层下面的精灵，所以要
	设置吞没函数，为true，而且ontouchbegin函数返要是true*/
	
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){
	
		log("layer outouch");
		return true;
	
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GamePauseLayer::bgmOnOff(Ref *r)
{
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) //如果没有音乐播放，则开始播放音乐
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("air.OGG",true);
		bgmSprite->setTexture("bgm_on.png");
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		bgmSprite->setTexture("bgm_off.png");
	}
}
void GamePauseLayer::effectOnOff(Ref *r)
{
	if (getDataManager().getEffect()==true) //如果没有音乐播放，则开始播放音乐
	{
		getDataManager().enableEffectOrNot(false);
		effectSprite->setTexture("sound_off.png");
	}
	else
	{
		getDataManager().enableEffectOrNot(true);
		effectSprite->setTexture("sound_on.png");
	}
}
