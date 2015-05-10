#include "bombIconLayer.h"
#include "DataManager.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

bombIconLayer::bombIconLayer()
{
}


bombIconLayer::~bombIconLayer()
{
}

bool bombIconLayer::init()
{
	Layer::init();

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size visibleSize = Director::getInstance()->getVisibleSize();

	bg = Sprite::create("changeLayerBg.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height + 250);
	auto mt_bg = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height - 100));
	bg->runAction(mt_bg);
	addChild(bg);

	//说明用法的label
	const char *str_des = ((String*)dic->objectForKey("descriptionBomb"))->_string.c_str();
	descriptionLabel = Label::createWithTTF(str_des, "fonts/b.ttf", 20);
	descriptionLabel->setDimensions(380, 200);
	descriptionLabel->setPosition(visibleSize.width / 2 + 10, visibleSize.height + 220);
	auto mt_des = MoveTo::create(0.5, Vec2(visibleSize.width / 2 + 10, visibleSize.height - 130));
	descriptionLabel->runAction(mt_des);
	addChild(descriptionLabel, 2);

	//关闭按钮
	close = MenuItemImage::create("menu.png", "menu.png", CC_CALLBACK_1(bombIconLayer::closeMenuCallback, this));
	close->setScale(0.4);
	closeMenu = Menu::create(close, NULL);
	closeMenu->setPosition(visibleSize.width / 2, visibleSize.height + 180);
	auto mt_close = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height - 170));
	addChild(closeMenu, 2);
	closeMenu->runAction(mt_close);

	//关闭按钮的文字
	const char *str_shut = ((String*)dic->objectForKey("shutdown"))->_string.c_str();
	closeLabel = Label::createWithTTF(str_shut, "fonts/b.ttf", 20);
	closeLabel->setPosition(visibleSize.width / 2 - 3, visibleSize.height + 180);
	auto mt_closeLabel = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height - 170));
	addChild(closeLabel, 3);
	closeLabel->runAction(mt_closeLabel);

	//飞弹和炮塔
	turret = Sprite::create("tower.png");
	turret->setScale(0.4);
	turret->setRotation(-90);
	turret->setPosition(visibleSize.width/2-60, visibleSize.height +240);
	auto mt_turret = MoveTo::create(0.5,Vec2(visibleSize.width / 2 - 60, visibleSize.height - 110));
	turret->runAction(mt_turret);
	addChild(turret, 3);

	missile = Sprite::create("missile.png");
	missile->setScale(0.9);
	missile->setRotation(-90);
	missile->setPosition(visibleSize.width / 2 + 30, visibleSize.height +240);
	auto mt_missile = MoveTo::create(0.5, Vec2(visibleSize.width / 2 +30, visibleSize.height - 110));
	missile->runAction(mt_missile);
	addChild(missile, 3);

	


	return true;
}

void bombIconLayer::closeMenuCallback(Ref * r)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	SimpleAudioEngine::getInstance()->playEffect("button.OGG");

	auto mt_des = MoveTo::create(0.5, Vec2(visibleSize.width / 2 + 10, visibleSize.height + 220));
	descriptionLabel->runAction(mt_des);

	auto mt_close = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 180));
	closeMenu->runAction(mt_close);

	auto mt_bg = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 250));
	bg->runAction(mt_bg);

	auto mt_cl = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 180));
	closeLabel->runAction(mt_cl);

	auto mt_turret = MoveTo::create(0.5, Vec2(visibleSize.width / 2 - 60, visibleSize.height +240));
	turret->runAction(mt_turret);

	auto mt_missile = MoveTo::create(0.5, Vec2(visibleSize.width / 2 + 30, visibleSize.height +240));
	missile->runAction(mt_missile);

	

	auto deleteLayer = CallFunc::create([=](){

		this->removeFromParentAndCleanup(true);
		getDataManager().setBombLayerAdded(false);
	});

	ActionInterval * delay = DelayTime::create(0.5);
	auto sq = Sequence::create(delay, deleteLayer, NULL);

	this->runAction(sq);
}
