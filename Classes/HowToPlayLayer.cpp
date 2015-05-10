#include "HowToPlayLayer.h"
USING_NS_CC; 

HowToPlayLayer::HowToPlayLayer():
vy_girl(0.5),
progress(0)
{
}


HowToPlayLayer::~HowToPlayLayer()
{
}

bool HowToPlayLayer::init()
{
	Layer::init();

	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size vs = Director::getInstance()->getVisibleSize();
	bg = Sprite::create("mainback.png");
	bg->setPosition(vs.width/2,vs.height/2);
	addChild(bg);

	auto listener = EventListenerTouchOneByOne::create();
	/*这里为了防止在该层点击能够点击到该层下面的精灵，所以要
	设置吞没函数，为true，而且ontouchbegin函数返要是true*/

	//1 秋晓萌飞入动画：从左边外面进场
	girl = Sprite::create("girl1.png");
	girl->setScale(0.20);
	girl->setPosition(-100, vs.height / 2);
	addChild(girl, 4);

	auto mt_girl_1 = MoveTo::create(1, Vec2(100, vs.height / 2));
	EaseOut * eo_mt_girl_1 = EaseOut::create(mt_girl_1, 7.0);
	girl->runAction(eo_mt_girl_1);

	
	const char *str_1 = ((String*)dic->objectForKey("htp1"))->_string.c_str();
	function = Label::createWithTTF(str_1, "fonts/b.ttf", 20);
	function->setDimensions(190, 300);
	function->setPosition(vs.width / 2 + 40, vs.height / 2 - 80);
	function->setVisible(false);
	addChild(function, 5);
	
	
	//下一项按钮
	hand = Sprite::create("back.png");
	hand->setFlippedY(true);
	hand->setScale(0.7);

	next = MenuItemSprite::create(hand, hand, CC_CALLBACK_1(HowToPlayLayer::nextIntro, this));  //这样能够使x轴翻转
	menuNext = Menu::create(next, NULL);

	menuNext->setPosition(vs.width / 2 + 50, vs.height / 2 - 30);
	addChild(menuNext, 5);
	auto st = ScaleBy::create(1, 0.8);
	auto sq = Sequence::create(st, st->reverse(), NULL);
	auto rp = RepeatForever::create(sq);
	next->runAction(rp);
	menuNext->setVisible(false);
	
	
	dialog = Sprite::create("gameEnd.png");
	dialog->setScale(0.001);
	dialog->setPosition(vs.width / 2+30, vs.height / 2);
	//dialog动画
	auto dialog_st = ScaleTo::create(0.5, 0.45);
	ActionInterval * delay_dst = DelayTime::create(1.2);
	auto vis = CallFunc::create([=](){
	
		
		menuNext->setVisible(true);
		function->setVisible(true);


	});
	auto seq_dst = Sequence::create(delay_dst, dialog_st,vis, NULL);
	dialog->runAction(seq_dst);
	addChild(dialog, 4);


	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){

		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//调用girl 移动函数
	girlPosY = vs.height / 2;
	this->schedule(SEL_SCHEDULE(&HowToPlayLayer::move), 0.02f);

	return true;
}

void HowToPlayLayer::move(float dt)
{
	Size vs = Director::getInstance()->getVisibleSize();
	//萌妹沿着y轴上下移动
	int delta;
	if (progress == 0)
	{
		delta = 10;
	}
	else
	{
		delta = 4;
		
	}


	float nextGirlY = girl->getPositionY() - vy_girl;
	//log("nextGirlY: %f", nextGirlY);
	//log("girlPosY: %f", girlPosY);
	girl->setPositionY(nextGirlY);
	if (nextGirlY >= girlPosY + delta)
	{
		vy_girl = -vy_girl;
	}
	if (nextGirlY <= girlPosY-delta)
	{
		//log("vy_girl: %f",vy_girl);
		vy_girl = -vy_girl;
	}
}

void HowToPlayLayer::nextIntro(Ref * r)
{
	progress++;

	switch (progress)
	{
	case 1:
		girlMove("htp2",740,Point(30,740)); //这个函数要改变label的内容和girl的位置
		break;
	case 2:
		girlMove("htp3", 750, Point(150, 750));
		break;
	case 3:
		girlMove("htp4", 730, Point(300, 730));
		break;
	case 4:
		girlMove("htp5", 730, Point(360, 730));
		break;
	case 5:
		girlMove("htp6", 650, Point(30, 650));
		break;
	case 6:
		girlMove("htp7", 650, Point(150, 650));
		break;
	case 7:
		girlMove("htp8", 400, Point(100, 400));
		break;
	case 8:
		girlMove("htp9", 50, Point(450, 50));
		break;
	case 9:
		girlMove("htp10", 400, Point(100, 400));
		break;
	case 10:
		girlMove("htp11", 400, Point(100, 400));
		break;
	case 11:
		deleteLayer();
		break;
	}
}

void HowToPlayLayer::deleteLayer()
{
	this->removeFromParent();
}

void HowToPlayLayer::girlMove(std::string str,float girlPosY,Point destination)
{
	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");
	const char *str_1 = ((String*)dic->objectForKey(str))->_string.c_str();
	function->setString(StringUtils::format(str_1));

	this->girlPosY = girlPosY;
	vy_girl = 0.2;
	this->unschedule(SEL_SCHEDULE(&HowToPlayLayer::move));
	ActionInterval * delay = DelayTime::create(0.55);
	auto schedule = CallFunc::create([=](){
	
		this->schedule(SEL_SCHEDULE(&HowToPlayLayer::move), 0.02f);
	});
	auto seq = Sequence::create(delay, schedule, NULL);
	this->runAction(seq);
	auto mt = MoveTo::create(0.5, destination);
	ScaleTo * st;
	if (progress==7)
	   st = ScaleTo::create(0.5, 0.2);
	else if (progress==8)
	   st = ScaleTo::create(0.5, 0.15);
	else if (progress==9)
	   st = ScaleTo::create(0.5, 0.2); 
	else if (progress==10)
	   st = ScaleTo::create(0.5, 0.2);
	else
	   st = ScaleTo::create(0.5, 0.1);

	auto sp = Spawn::create(mt, st, NULL);
	girl->runAction(sp);

	
	if (progress == 10)
	{
		menuNext->removeFromParent();

		auto item = Sprite::create("menu.png");
		item->setScale(0.5);
		next = MenuItemSprite::create(item,item, CC_CALLBACK_1(HowToPlayLayer::nextIntro, this));  //这样能够使x轴翻转
		auto menuNext1 = Menu::create(next, NULL);
		menuNext1->setPosition(250 + 50+40, 400 - 30);
		addChild(menuNext1, 5);

		const char *str_1 = ((String*)dic->objectForKey("htp12"))->_string.c_str();
		auto label = Label::create(str_1, "fonts/b.ttf", 20);
		label->setPosition(250 + 37 , 400 - 52);
		addChild(label, 7);
		
	}
	


}

