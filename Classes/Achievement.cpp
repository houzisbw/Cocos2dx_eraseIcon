#include "Achievement.h"
#include "WelcomeScene.h"
#include "DataManager.h"
#include "AchieveLayer.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;


Achievement::Achievement()
{
}


Achievement::~Achievement()
{
}

bool Achievement::init()
{
	Layer::init();

	isLayerAdded = false;
	//��xml�������ģ��ǵ�xml�ļ�Ҫ����resource�ļ����������
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size size = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("city2.png");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg);

	menu = Sprite::create("menuAchieve.png");
	menu->setPosition(size.width / 2+30, size.height / 2);
	menu->setScale(0.7);
	addChild(menu,2);

	back = MenuItemImage::create("backAchieve.png", "backAchieve.png", CC_CALLBACK_1(Achievement::goBackToMainmenu, this));
	back->setScale(1.3);
	auto backMenu = Menu::create(back, NULL);
	backMenu->setPosition(size.width / 2 - 70, size.height / 2 - 2);
	addChild(backMenu,2);

	//���ܸ�scene�Ķ���
	//��͸����ɫ����
	transBg = Sprite::create("transparent.png");
	transBg->setPosition(size.width / 2, size.height-110);
	transBg->setScaleX(1.5);
	transBg->setScaleY(0.001);
	addChild(transBg, 7);

	ActionInterval * delaytime_bg = CCDelayTime::create(1.5);
	auto st_bg = ScaleTo::create(0.5, 1.5, 0.8);  //�����x y��������ԭ��ͼƬ�Ĵ�С�ˣ���setscale֮��Ĵ�С��
	auto st_bg2 = ScaleTo::create(0.5, 1.5, 0.001);  //�����x y��������ԭ��ͼƬ�Ĵ�С�ˣ���setscale֮��Ĵ�С��
	ActionInterval * delay_bg = DelayTime::create(2.2);
	auto deleteBg = CallFunc::create([=](){
		transBg->removeFromParent();
	});
	auto sq_bg = Sequence::create(delaytime_bg, st_bg, delay_bg,st_bg2,deleteBg,NULL);
	transBg->runAction(sq_bg);

	//Ů��������ܳ���
	girl = Sprite::create("girl1.png");
	girl->setScale(0.2);
	girl->setPosition(-100, size.height -110);
	addChild(girl, 15);
	ActionInterval * delaytime_girl = CCDelayTime::create(2.0);
	auto mt_girl = MoveTo::create(0.5, Vec2(50, size.height -110));
	auto mt_girl2 = MoveTo::create(0.5, Vec2(-100, size.height - 110));
	ActionInterval * delay_girl = DelayTime::create(1.5);
	auto sq_girl = Sequence::create(delaytime_girl, mt_girl,delay_girl,mt_girl2, NULL);
	girl->runAction(sq_girl);

	const char *str = ((String*)dic->objectForKey("achieve1"))->_string.c_str();
	des = Label::createWithTTF(str, "fonts/b.ttf", 23);
	des->setPosition(size.width / 2-140 , size.height - 100);
	des->setAnchorPoint(Point(0, 0));
	
	des->setOpacity(0);
	addChild(des,8);
	auto fi = FadeIn::create(0.5);
	ActionInterval * delay = DelayTime::create(2.1);
	ActionInterval * delay_des = DelayTime::create(1.2);
	auto deleteDes = CallFunc::create([=](){
		des->removeFromParent();
	});
	auto seq = Sequence::create(delay, fi,delay_des,deleteDes, NULL);

	des->runAction(seq);

	//����
	trophy = Sprite::create("trophy.png");
	trophy->setScale(1.5);
	trophy->setPosition(240,512);
	addChild(trophy, 4);

	//��Ϣinfo
	info = Sprite::create("info.png");
	info->setScale(1.5);
	info->setPosition(305, 400);
	addChild(info, 4);

	//����item
	item = Sprite::create("present.png");
	item->setScale(1.5);
	item->setPosition(242, 295);
	addChild(item, 4);

	//����¼�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *t, Event *e){ //infoҪ�����÷���&
		//������������
		if (trophy->getBoundingBox().containsPoint(t->getLocation()) && getDataManager().getAdded() == 0)
		{
			//��parent��spriteΪ���ɼ�
			menu->setVisible(false);
			back->setVisible(false);
			trophy->setVisible(false);
			info->setVisible(false);
			item->setVisible(false);

			getDataManager().setAchieveLayer(1);

			//��Ӿ�����ʾ�㣬ֻ��һ���㣬����������3�����
			AchieveLayer * al = AchieveLayer::create();
			al->setPosition(0, 0);
			addChild(al,10);
		

		
		}
		//��������info��Ϣ
		else if (info->getBoundingBox().containsPoint(t->getLocation()) && getDataManager().getAdded() == 0) //0��ʾδ���
		{
			//��parent��spriteΪ���ɼ�
			menu->setVisible(false);
			back->setVisible(false);
			trophy->setVisible(false);
			info->setVisible(false);
			item->setVisible(false);

			getDataManager().setAchieveLayer(2);
			//��Ӿ�����ʾ�㣬ֻ��һ���㣬����������3�����
			AchieveLayer * al = AchieveLayer::create();
			al->setPosition(0, 0);
			addChild(al, 10);

			
		}
		//�����������߶һ�
		else if (item->getBoundingBox().containsPoint(t->getLocation()) && getDataManager().getAdded() == 0) //0��ʾδ���
		{
			//��parent��spriteΪ���ɼ�
			menu->setVisible(false);
			back->setVisible(false);
			trophy->setVisible(false);
			info->setVisible(false);
			item->setVisible(false);

			getDataManager().setAchieveLayer(3);
			//��Ӿ�����ʾ�㣬ֻ��һ���㣬����������3�����
			AchieveLayer * al = AchieveLayer::create();
			al->setPosition(0, 0);
			addChild(al, 10);


		}


		//�ָ����ص�sprite
		if (getDataManager().getAdded() == 0)
		{
			menu->setVisible(true);
			back->setVisible(true);
			trophy->setVisible(true);
			info->setVisible(true);
			item->setVisible(true);
				
		}
		
		

		
		return false;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}



Scene* Achievement::createScene()
{
	auto scene = Scene::create();
	auto layer = Achievement::create();
	scene->addChild(layer);
	return scene;
}

void Achievement::goBackToMainmenu(Ref * r)
{
	Director::getInstance()->replaceScene(TransitionFade::create(2, WelcomeScene::createScene()));

	//�ı�����
	//SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("mswlm.OGG", true);
}
