#include "finalScene.h"
#include "WelcomeScene.h"
USING_NS_CC;


finalScene::finalScene()
{
}


finalScene::~finalScene()
{
}

bool finalScene::init()
{
	Layer::init();

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size vs = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("bg.jpg");
	bg->setPosition(vs.width / 2, vs.height / 2);
	addChild(bg);

	dialog = Sprite::create("gameEnd.png");
	dialog->setPosition(vs.width / 2, -500);
	dialog->setScale(0.8);
	dialog->setScaleY(1.1);
	auto mt_dialog = MoveTo::create(2.0, Vec2(vs.width / 2, vs.height / 2));
	EaseOut * dialog_slow = EaseOut::create(mt_dialog, 7.0);
	dialog->runAction(dialog_slow);
	addChild(dialog,2);

	ok = Sprite::create("menu.png");
	ok->setPosition(vs.width / 2, vs.height / 2 - 190);
	ok->setScale(0.5);
	ok->setVisible(false);
	addChild(ok,2);

	const char *str1 = ((String*)dic->objectForKey("mainMenu"))->_string.c_str();
	okLabel = Label::createWithTTF(str1, "fonts/b.ttf", 20);
	okLabel->setPosition(vs.width / 2, vs.height / 2 - 190);
	addChild(okLabel,3);
	okLabel->setVisible(false);
	
	clear = Sprite::create("clear.png");
	clear->setPosition(vs.width / 2+100, vs.height - 130);
	clear->setVisible(false);
	clear->setScale(0.6);
	addChild(clear, 2);

	title = Sprite::create("praise2.png");
	title->setPosition(vs.width / 2, vs.height - 250);
	title->setScale(1.2);
	title->setVisible(false);
	addChild(title, 3);

	girl1 = Sprite::create("22-h.png");
	girl1->setPosition(vs.width - 160, 60);
	girl1->setVisible(false);
	addChild(girl1, 3);

	girl2 = Sprite::create("33-h.png");
	girl2->setPosition(vs.width - 80, 60);
	girl2->setVisible(false);
	addChild(girl2, 3);
	
	const char *str_web = ((String*)dic->objectForKey("web"))->_string.c_str();
	web = Label::createWithTTF(str_web, "fonts/b.ttf", 20);
	web->setPosition(vs.width / 2, vs.height / 2);
	web->setVisible(false);
	addChild(web, 3);

	web2 = Label::createWithTTF("http://www.galacg.me/archives/14773.html", "fonts/b.ttf", 14);
	web2->setPosition(vs.width / 2, vs.height / 2-40);
	web2->setVisible(false);
	addChild(web2, 3);

	auto vis = CallFunc::create([=](){
	
		okLabel->setVisible(true);
		ok->setVisible(true);
		clear->setVisible(true);
		title->setVisible(true);
		girl1->setVisible(true);
		girl2->setVisible(true);
		web->setVisible(true);
		web2->setVisible(true);
	});
	ActionInterval * delayVis = DelayTime::create(2.2);
	auto seqVis = Sequence::create(delayVis, vis, NULL);
	this->runAction(seqVis);


	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){ //info要按引用访问&
		if (ok->getBoundingBox().containsPoint(t->getLocation()))
		{
			Director::getInstance()->replaceScene(TransitionFade::create(2.0, WelcomeScene::createScene()));
		}
		return false;


	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

Scene* finalScene::createScene()
{
	auto scene = Scene::create();
	auto layer = finalScene::create();
	scene->addChild(layer);
	return scene;
}