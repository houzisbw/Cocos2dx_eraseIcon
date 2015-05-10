#include "WorldMapScene.h"
#include "WelcomeScene.h"

USING_NS_CC;

WorldMapScene::WorldMapScene()
{
}


WorldMapScene::~WorldMapScene()
{
}

bool WorldMapScene::init()
{
	Layer::init();
	Size size = Director::getInstance()->getVisibleSize();

	bg = Sprite::create("worldmap.jpg");
	bg->setPosition(size.width / 2, size.height / 2);
	addChild(bg);

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");


	//前进后退
	previous = MenuItemImage::create("map_next.png", "map_next.png", CC_CALLBACK_1(WorldMapScene::previousLocation, this));
	previous->setScale(0.5);
	previous->setRotation(-180);
	auto menuPrevious = Menu::create(previous, NULL);
	menuPrevious->setPosition(size.width - 27, size.height / 2);
	menuPrevious->setVisible(false);
	addChild(menuPrevious, 4);

	next = MenuItemImage::create("map_next.png", "map_next.png", CC_CALLBACK_1(WorldMapScene::nextLocation, this));
	next->setScale(0.5);
	auto menuNext = Menu::create(next, NULL);
	menuNext->setPosition(size.width - 27, size.height / 2+70);
	menuNext->setVisible(false);
	addChild(menuNext, 4);



	menuBg = Sprite::create("mapMenu.png");
	menuBg->setPosition(size.width + 100, size.height / 2);
	addChild(menuBg, 3);
	auto mt_bg = MoveTo::create(1, Vec2(size.width - 40, size.height / 2));
	ActionInterval * delay_bg = DelayTime::create(1.0);
	auto seq_bg = Sequence::create(delay_bg, mt_bg,NULL);
	menuBg->runAction(seq_bg);

	back = MenuItemImage::create("worldmap_back.png", "worldmap_back.png", CC_CALLBACK_1(WorldMapScene::backToMenu, this));
	back->setScale(0.5);
	auto btm = Menu::create(back,NULL);
	btm->setPosition(size.width - 27, size.height / 2 - 70);
	btm->setVisible(false);
	addChild(btm, 4);

	ActionInterval * delay_vis = DelayTime::create(2.2);
	auto vis = CallFunc::create([=](){
	
		btm->setVisible(true);
		menuPrevious->setVisible(true);
		menuNext->setVisible(true);
		
	});
	auto seq = Sequence::create(delay_vis, vis, NULL);
	this->runAction(seq);

	//读取玩到了多少关
	curLevelNum = UserDefault::getInstance()->getIntegerForKey("level_Global");

	//初始化位置vector
	pos_vec.push_back({ 141, 800-142});
	pos_vec.push_back({ 154, 800-95 });
	pos_vec.push_back({ 246, 800 - 166 });
	pos_vec.push_back({ 197, 800 - 744 });
	pos_vec.push_back({ 89, 800 - 656 });

	pos_vec.push_back({ 134, 800 - 559 });
	pos_vec.push_back({ 147, 800 - 668 });
	pos_vec.push_back({ 310, 800 - 504 });
	pos_vec.push_back({ 100, 800 - 400 });

	pos_vec.push_back({ 107, 800 - 372 });
	pos_vec.push_back({ 121, 800 - 357 });
	pos_vec.push_back({ 90, 800 - 377 });
	pos_vec.push_back({ 145, 800 - 350 });

	pos_vec.push_back({ 88, 800 - 309 });
	pos_vec.push_back({ 120, 800 - 395 });
	pos_vec.push_back({ 134, 800 - 370 });
	pos_vec.push_back({ 246, 800 - 343 });
	
	pos_vec.push_back({ 173, 800 - 294 });
	pos_vec.push_back({ 263, 800 - 235 });
	pos_vec.push_back({ 347, 800 - 65 });
	


	//22娘
	gt = GirlTwo::create();
	gt->setRotation(-90);
	gt->setScale(0.3);
	gt->setPosition(pos_vec[curLevelNum-1]+Point(-10,-40)+Point(-400,0));
	addChild(gt, 3);

	ActionInterval * delay = DelayTime::create(2.0 + curLevelNum*0.3);
	auto mt_in = MoveTo::create(1.0, pos_vec[curLevelNum - 1] + Point(-10, -10));
	auto seq1 = Sequence::create(delay, mt_in, NULL);
	gt->runAction(seq1);


	for (int i = 0; i <= curLevelNum-1; i++)
	{
		auto locator = Sprite::create("map_locator.png");
		locator->setScale(0.2);
		locator->setPosition(pos_vec[i]-Point(10,0)-Point(400,0));
		addChild(locator, 4);
		loc_vec.push_back(locator);

		//变大变小
		auto st = ScaleBy::create(1.0, 1.2);
		auto seq = Sequence::create(st, st->reverse(), NULL);
		auto rp = RepeatForever::create(seq);
		//locator->runAction(rp);

		//从外面飞进屏幕
		ActionInterval * delay = DelayTime::create(2.0+i*0.3);
		auto mt_in = MoveTo::create(1.0, pos_vec[i] - Point(10, 0));
		auto seq1 = Sequence::create(delay,mt_in, NULL);
		locator->runAction(seq1);


	}

	loc = 0;
	//对话框显示地名
	dialog = Sprite::create("menu.png");
	dialog->setScaleX(0.6);
	dialog->setScaleY(0.6);
	dialog->setRotation(-90);
	dialog->setPosition(pos_vec[0].x-60,pos_vec[0].y);
	addChild(dialog, 4);

	const char *str_loc = ((String*)dic->objectForKey("map1"))->_string.c_str();
	locLabel = Label::createWithTTF(str_loc, "fonts/b.ttf", 25);
	locLabel->setRotation(-90);
	locLabel->setPosition(pos_vec[0].x - 60, pos_vec[0].y);
	addChild(locLabel, 5);



	return true;
}

void WorldMapScene::nextLocation(Ref  * r)
{

	if (UserDefault::getInstance()->getIntegerForKey("level_Global") > 0)
	{

		//用xml保存中文，记得xml文件要存在resource文件夹里面才行
		Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

		loc++;
		if (loc > curLevelNum - 1)
		{
			loc = 0;
		}
		auto mt = MoveTo::create(0.5, Vec2(pos_vec[loc].x - 60, pos_vec[loc].y));
		dialog->runAction(mt);

		//文字描述
		auto mt_label = MoveTo::create(0.5, Vec2(pos_vec[loc].x - 60, pos_vec[loc].y));
		locLabel->runAction(mt_label);
		char c[10];
		sprintf(c, "map%d", loc + 1);
		std::string str = c;
		const char *str_loc = ((String*)dic->objectForKey(str))->_string.c_str();
		locLabel->setString(str_loc);


		auto tempLoc = loc_vec[loc];
		//变大变小
		auto st = ScaleBy::create(1.0, 1.5);
		auto seq = Sequence::create(st, st->reverse(), NULL);
		auto rp = RepeatForever::create(seq);
		tempLoc->runAction(rp);

		//之前的取消动画
		int temp = loc - 1;
		if (temp == -1)
		{
			temp = curLevelNum - 1;
		}
		auto preLoc = loc_vec[temp];
		preLoc->stopAllActions();
		preLoc->setScale(0.2);
	}
}
void WorldMapScene::previousLocation(Ref  * r)
{


	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	if (UserDefault::getInstance()->getIntegerForKey("level_Global")>0)
	{

		loc--;
		if (loc < 0)
		{
			loc = curLevelNum - 1;
		}
		auto mt = MoveTo::create(0.5, Vec2(pos_vec[loc].x - 60, pos_vec[loc].y));
		dialog->runAction(mt);

		//文字描述
		auto mt_label = MoveTo::create(0.5, Vec2(pos_vec[loc].x - 60, pos_vec[loc].y));
		locLabel->runAction(mt_label);
		char c[10];
		sprintf(c, "map%d", loc + 1);
		std::string str = c;
		const char *str_loc = ((String*)dic->objectForKey(str))->_string.c_str();
		locLabel->setString(str_loc);

		auto tempLoc = loc_vec[loc];
		//变大变小
		auto st = ScaleBy::create(1.0, 1.5);
		auto seq = Sequence::create(st, st->reverse(), NULL);
		auto rp = RepeatForever::create(seq);
		tempLoc->runAction(rp);

		//之前的取消动画
		int temp = loc+1;
		if (temp == curLevelNum )
		{
			temp = 0;
		}
		auto preLoc = loc_vec[temp];
		preLoc->stopAllActions();
		preLoc->setScale(0.2);
	}

	
}

void WorldMapScene::traceMove()
{
	
	   ActionInterval * delay = DelayTime::create(1);

		auto mt1 = MoveTo::create(1, pos_vec[1]);
		auto mt2 = MoveTo::create(1, pos_vec[2]);
		auto mt3 = MoveTo::create(1, pos_vec[3]);
		auto mt4 = MoveTo::create(1, pos_vec[4]);

		auto mt5 = MoveTo::create(1, pos_vec[5]);
		auto mt6 = MoveTo::create(1, pos_vec[6]);
		auto mt7 = MoveTo::create(1, pos_vec[7]);
		auto mt8 = MoveTo::create(1, pos_vec[8]);

		auto mt9 = MoveTo::create(1, pos_vec[9]);
		auto mt10 = MoveTo::create(1, pos_vec[10]);
		auto mt11 = MoveTo::create(1, pos_vec[11]);
		auto mt12 = MoveTo::create(1, pos_vec[12]);

		auto mt13 = MoveTo::create(1, pos_vec[13]);
		auto mt14 = MoveTo::create(1, pos_vec[14]);
		auto mt15 = MoveTo::create(1, pos_vec[15]);
		auto mt16 = MoveTo::create(1, pos_vec[16]);

		auto mt17 = MoveTo::create(1, pos_vec[17]);
		auto mt18 = MoveTo::create(1, pos_vec[18]);
		auto mt19 = MoveTo::create(1, pos_vec[19]);
		


		//ActionInterval * delay = DelayTime::create(1);
		//char * c[10] = {"mt1","mt2"};
		//auto seq = Sequence::createWithVariableList(mt1, c[10]);  
		//gt->runAction(seq);
	
}

Scene*  WorldMapScene::createScene()
{
	auto scene = Scene::create();
	auto layer = WorldMapScene::create();
	scene->addChild(layer);
	return scene;
}

void WorldMapScene::backToMenu(Ref * r)
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, WelcomeScene::createScene()));
}
