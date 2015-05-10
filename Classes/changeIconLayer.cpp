#include "DataManager.h"
#include "changeIconLayer.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

changeIconLayer::changeIconLayer()
{
}


changeIconLayer::~changeIconLayer()
{
}

bool changeIconLayer::init()
{
   Layer::init();
  

   //用xml保存中文，记得xml文件要存在resource文件夹里面才行
   Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

   Size visibleSize = Director::getInstance()->getVisibleSize();

   bg = Sprite::create("changeLayerBg.png");
   bg->setPosition(visibleSize.width/2,visibleSize.height+250);
   auto mt_bg = MoveTo::create(0.5,Vec2(visibleSize.width / 2, visibleSize.height - 100));
   bg->runAction(mt_bg);
   addChild(bg);

   //说明用法的label
   const char *str_des = ((String*)dic->objectForKey("description"))->_string.c_str();
   descriptionLabel = Label::createWithTTF(str_des, "fonts/b.ttf", 20);
   descriptionLabel->setDimensions(380, 200);
   descriptionLabel->setPosition(visibleSize.width / 2+10, visibleSize.height + 220);
   auto mt_des = MoveTo::create(0.5, Vec2(visibleSize.width / 2+10, visibleSize.height - 130));
   descriptionLabel->runAction(mt_des);
   addChild(descriptionLabel, 2);

   //关闭按钮
   close = MenuItemImage::create("menu.png","menu.png",CC_CALLBACK_1(changeIconLayer::closeMenuCallback,this));
   close->setScale(0.4);
   closeMenu = Menu::create(close,NULL);
   closeMenu->setPosition(visibleSize.width / 2, visibleSize.height + 180);
   auto mt_close = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height - 170));
   addChild(closeMenu, 2);
   closeMenu->runAction(mt_close);

   //关闭按钮的文字
   const char *str_shut = ((String*)dic->objectForKey("shutdown"))->_string.c_str();
   closeLabel = Label::createWithTTF(str_shut, "fonts/b.ttf", 20);
   closeLabel->setPosition(visibleSize.width / 2-3, visibleSize.height + 180);
   auto mt_closeLabel = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height -170));
   addChild(closeLabel, 3);
   closeLabel->runAction(mt_closeLabel);

   //5个不同的icon
   
   vec = getDataManager().getIconVec();
  
   char c1[10];
   sprintf(c1, "%d", vec[0]);
   string str1 = c1;
   icon1 = Sprite::create("level1_" + str1 + ".png");
   icon1->setScale(0.36);
   icon1->setTag(vec[0]);
   icon1->setPosition(90, visibleSize.height +240);
   auto mt_1 = MoveTo::create(0.5, Vec2(90, visibleSize.height - 110));
   icon1->runAction(mt_1);
   addChild(icon1, 3);
   iconVec.pushBack(icon1);

   char c2[10];
   sprintf(c2, "%d", vec[1]);
   string str2 = c2;
   icon2 = Sprite::create("level1_" + str2 + ".png");
   icon2->setScale(0.36);
   icon2->setTag(vec[1]);
   icon2->setPosition(90+80, visibleSize.height +240);
   auto mt_2 = MoveTo::create(0.5, Vec2(90+80, visibleSize.height - 110));
   icon2->runAction(mt_2);
   addChild(icon2, 3);
   iconVec.pushBack(icon2);

   char c3[10];
   sprintf(c3, "%d", vec[2]);
   string str3 = c3;
   icon3 = Sprite::create("level1_" + str3 + ".png");  
   icon3->setScale(0.36);
   icon3->setTag(vec[2]);
   icon3->setPosition(90 + 160, visibleSize.height +240);
   auto mt_3 = MoveTo::create(0.5, Vec2(90 + 160, visibleSize.height - 110));
   icon3->runAction(mt_3);
   addChild(icon3, 3);
   iconVec.pushBack(icon3);

   char c4[10];
   sprintf(c4, "%d", vec[3]);
   string str4 = c4;
   icon4 = Sprite::create("level1_" + str4 + ".png");
   icon4->setScale(0.36);
   icon4->setTag(vec[3]);
   icon4->setPosition(90 + 240, visibleSize.height +240);
   auto mt_4 = MoveTo::create(0.5, Vec2(90 + 240, visibleSize.height - 110));
   icon4->runAction(mt_4);
   addChild(icon4, 3);
   iconVec.pushBack(icon4);

   char c5[10];
   sprintf(c5, "%d", vec[4]);
   string str5 = c5;
   icon5 = Sprite::create("level1_" + str5 + ".png");  
   icon5->setScale(0.36);
   icon5->setTag(vec[4]);
   icon5->setPosition(90 + 320, visibleSize.height +240);
   auto mt_5 = MoveTo::create(0.5, Vec2(90 + 320, visibleSize.height - 110));
   icon5->runAction(mt_5);
   addChild(icon5, 3);
   iconVec.pushBack(icon5);
   

  
   last = nullptr;
   auto listener = EventListenerTouchOneByOne::create();
   //listener->setSwallowTouches(true);
   listener->onTouchBegan = [=](Touch * t,Event * e){
   
	   if (last != nullptr)
	   {
		   last->stopAllActions();
		   last->setScale(0.36);
	   }
	   for (int i = 0; i < iconVec.size(); i++)
	   {
		   if (iconVec.at(i)->getBoundingBox().containsPoint(t->getLocation()))
		   {
			   
			   auto st = ScaleBy::create(0.5, 1.2);
			   auto sq = Sequence::create(st, st->reverse(),NULL);
			   auto rp = RepeatForever::create(sq);
			   iconVec.at(i)->runAction(rp);
			   last = iconVec.at(i);
			   getDataManager().setSelectedIconValue(iconVec.at(i)->getTag()); //通过tag获取其board值
			   break;
		   }
	   }
	   
	   return false;
   };
   Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
   return true;
}

void changeIconLayer::closeMenuCallback(Ref * r)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	SimpleAudioEngine::getInstance()->playEffect("button.OGG");

	auto mt_des = MoveTo::create(0.5, Vec2(visibleSize.width / 2+10, visibleSize.height + 220));
	descriptionLabel->runAction(mt_des);

	auto mt_close = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 180));
	closeMenu->runAction(mt_close);

	auto mt_bg = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 250));
	bg->runAction(mt_bg);

	auto mt_cl = MoveTo::create(0.5, Vec2(visibleSize.width / 2, visibleSize.height + 180));
	closeLabel->runAction(mt_cl);

	//5个icon的移除
	auto mt_1 = MoveTo::create(0.5, Vec2(90, visibleSize.height + 240));
	icon1->runAction(mt_1);
	auto mt_2 = MoveTo::create(0.5, Vec2(90+80, visibleSize.height + 240));
	icon2->runAction(mt_2);
	auto mt_3 = MoveTo::create(0.5, Vec2(90+160, visibleSize.height + 240));
	icon3->runAction(mt_3);
	auto mt_4 = MoveTo::create(0.5, Vec2(90+240, visibleSize.height + 240));
	icon4->runAction(mt_4);
	auto mt_5 = MoveTo::create(0.5, Vec2(90+320, visibleSize.height + 240));
	icon5->runAction(mt_5);

	auto deleteLayer = CallFunc::create([=](){

		this->removeFromParentAndCleanup(true);
		getDataManager().setChangeLayerAdded(false);
		getDataManager().setSelectedIconValue(-1);
	});

	ActionInterval * delay = DelayTime::create(0.5);
	auto sq = Sequence::create(delay, deleteLayer, NULL);

	this->runAction(sq);

}

/*vector<int> changeIconLayer::getIconVector()
{
	return HelloWorld::icon;
}*/

