#include "logoScene.h"
#include "WelcomeScene.h"
#include "WorldMapScene.h"
#include "Achievement.h"
#include "finalScene.h"
USING_NS_CC;

logoScene::logoScene()
{
}


logoScene::~logoScene()
{
}

bool logoScene::init()
{
	Layer::init();
	Size size = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("logoBg.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg);

	girl1 = Sprite::create("22-h.png");
	girl1->setPosition(size.width / 2 - 70, size.height / 2 + 40);
	girl1->setScale(1.2);
	addChild(girl1, 2);

	girl2 = Sprite::create("33-h.png");
	girl2->setPosition(size.width / 2 + 50, size.height / 2 + 40);
	girl2->setScale(1.2);
	addChild(girl2, 2);


	logo = Label::createWithTTF("Onisan Studio", "fonts/ethnocentricrg.ttf", 25);
	logo->setPosition(size.width / 2, size.height / 2 - 80);
	logo->setColor(Color3B(153, 217, 234));
	logo->setOpacity(0);
	addChild(logo, 2);

	auto fi = FadeIn::create(1.5);
	logo->runAction(fi);

	ActionInterval * delay = DelayTime::create(3.0);
	auto change = CallFunc::create([=](){
	
		Director::getInstance()->replaceScene(WelcomeScene::createScene());
		
	});
	auto seq = Sequence::create(delay, change, NULL);
	this->runAction(seq);

	return true;
}

Scene* logoScene::createScene()
{
	auto scene = Scene::create();
	auto layer = logoScene::create();
	scene->addChild(layer);
	return scene;
}