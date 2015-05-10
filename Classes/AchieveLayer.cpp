#include "AchieveLayer.h"
#include "DataManager.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

AchieveLayer::AchieveLayer()
{
}


AchieveLayer::~AchieveLayer()
{
}

bool AchieveLayer::init()
{
	Layer::init(); 

	isClickedChange = false;
	isClickedBomb = false; 
	//用xml保存中文，记得xml文件要存在resource文件夹里面才行
	Dictionary* dic = Dictionary::createWithContentsOfFile("chinese.xml");

	Size size = Director::getInstance()->getVisibleSize();
	dialog = Sprite::create("gameEnd.png");
	dialog->setPosition(size.width / 2, -500);
	dialog->setScale(0.8);
	dialog->setScaleY(1.1);
	auto mt_dialog = MoveTo::create(2.0, Vec2(size.width / 2, size.height / 2));
	EaseOut * dialog_slow = EaseOut::create(mt_dialog, 7.0);
	dialog->runAction(dialog_slow);
	addChild(dialog,2);

	//添加标题
    const char *str = ((String*)dic->objectForKey("achieve2"))->_string.c_str();
	record = Label::createWithTTF(str, "fonts/b.ttf", 40);
	record->setPosition(size.width / 2, size.height / 2 + 150);
	record->setVisible(false);
	addChild(record, 2);

	pointerLeft = Sprite::create("pointer.png");
	pointerLeft->setRotation(90);
	pointerLeft->setScale(1.5);
	pointerLeft->setPosition(size.width / 2 - 100, size.height / 2 + 150);
	pointerLeft->setVisible(false);
	addChild(pointerLeft,3);

	pointerRight = Sprite::create("pointer.png");
	pointerRight->setRotation(-90);
	pointerRight->setScale(1.5);
	pointerRight->setPosition(size.width / 2 + 100, size.height / 2 + 150);
	addChild(pointerRight,3);
	pointerRight->setVisible(false);

	ok = Sprite::create("menu.png");
	ok->setPosition(size.width / 2, size.height / 2 -190);
	ok->setScale(0.5);
	ok->setVisible(false);
	addChild(ok, 3);

	//历史最高分
	icon1 = Sprite::create("achieveIcon.png");
	icon1->setPosition(size.width / 2 - 70, size.height / 2 +70);
	icon1->setVisible(false);
	addChild(icon1, 5);

	const char *str_bsl = ((String*)dic->objectForKey("achieve4"))->_string.c_str();
	historyBestScoreLabel = Label::createWithTTF(str_bsl, "fonts/b.ttf", 23);
	historyBestScoreLabel->setPosition(size.width / 2+10 , size.height / 2 + 80);
	historyBestScoreLabel->setVisible(false);
	addChild(historyBestScoreLabel, 5);

	char c1[10];
	int hbs = UserDefault::getInstance()->getIntegerForKey("historyBestScore");
	sprintf(c1, "%d", hbs);
	std::string str_hbln = c1;
	historyBestScoreLabelNum = Label::createWithTTF(str_hbln, "fonts/HANGTHEDJ.ttf", 40);
	historyBestScoreLabelNum->setPosition(size.width/2,size.height/2+20);
	historyBestScoreLabelNum->setVisible(false);
	addChild(historyBestScoreLabelNum, 5);

	//单次消去最多
	icon2 = Sprite::create("achieveIcon.png");
	icon2->setPosition(size.width / 2 - 70, size.height / 2 -50);
	icon2->setVisible(false);
	addChild(icon2, 5);

	const char *str_sb = ((String*)dic->objectForKey("achieve5"))->_string.c_str();
	singleBest = Label::createWithTTF(str_sb, "fonts/b.ttf", 23);
	singleBest->setPosition(size.width / 2 + 20, size.height / 2 -40);
	singleBest->setVisible(false);
	addChild(singleBest, 5);

	char c3[10];
	int sb = UserDefault::getInstance()->getIntegerForKey("singleBest");
	sprintf(c3, "%d", sb);
	std::string str_sb1 = c3;
    singleBestNum = Label::createWithTTF(str_sb1, "fonts/HANGTHEDJ.ttf", 40);
	singleBestNum->setPosition(size.width / 2, size.height / 2 - 100);
	singleBestNum->setVisible(false);
	addChild(singleBestNum, 5);

	const char *str1 = ((String*)dic->objectForKey("achieve3"))->_string.c_str();
	okLabel = Label::createWithTTF(str1, "fonts/b.ttf", 20);
	okLabel->setPosition(size.width / 2, size.height / 2 - 190);
	addChild(okLabel,4);
	okLabel->setVisible(false);

	//////////////////////////游戏信息的label
	//添加标题
	const char *str5 = ((String*)dic->objectForKey("achieve6"))->_string.c_str();
	info1 = Label::createWithTTF(str5, "fonts/b.ttf", 40);
	info1->setPosition(size.width / 2, size.height / 2 + 150);
	info1->setVisible(false);
	addChild(info1, 2);

	pointerLeft1 = Sprite::create("pointer.png");
	pointerLeft1->setRotation(90);
	pointerLeft1->setScale(1.5);
	pointerLeft1->setPosition(size.width / 2 - 100, size.height / 2 + 150);
	pointerLeft1->setVisible(false);
	addChild(pointerLeft1,2);

	pointerRight1 = Sprite::create("pointer.png");
	pointerRight1->setRotation(-90);
	pointerRight1->setScale(1.5);
	pointerRight1->setPosition(size.width / 2 + 100, size.height / 2 + 150);
	addChild(pointerRight1,2);
	pointerRight1->setVisible(false);

	//3个info
	icon3 = Sprite::create("achieveIcon.png");
	icon3->setPosition(size.width / 2 - 150, size.height / 2 + 70);
	icon3->setVisible(false);
	addChild(icon3, 5);

	const char *str_10 = ((String*)dic->objectForKey("achieve7"))->_string.c_str();
	infoLabel1 = Label::createWithTTF(str_10, "fonts/b.ttf", 23);
	infoLabel1->setAnchorPoint(Point(0, 0));	
	infoLabel1->setPosition(size.width / 2 - 120, size.height / 2 + 70);
	infoLabel1->setVisible(false);
	addChild(infoLabel1, 5);


	icon4 = Sprite::create("achieveIcon.png");
	icon4->setPosition(size.width / 2 - 150, size.height / 2 +10);
	icon4->setVisible(false);
	addChild(icon4, 5);

	const char *str_11 = ((String*)dic->objectForKey("achieve8"))->_string.c_str();
	infoLabel2 = Label::createWithTTF(str_11, "fonts/b.ttf", 23);
	infoLabel2->setAnchorPoint(Point(0, 0));
	infoLabel2->setPosition(size.width / 2 - 120, size.height / 2 +10);
	infoLabel2->setVisible(false);
	addChild(infoLabel2, 5);


	icon5 = Sprite::create("achieveIcon.png");
	icon5->setPosition(size.width / 2 - 150, size.height / 2 -50);
	icon5->setVisible(false);
	addChild(icon5, 5);

	const char *str_12 = ((String*)dic->objectForKey("achieve9"))->_string.c_str();
	infoLabel3 = Label::createWithTTF(str_12, "fonts/b.ttf", 23);
	infoLabel3->setAnchorPoint(Point(0, 0));
	infoLabel3->setPosition(size.width / 2 - 120, size.height / 2 -50);
	infoLabel3->setVisible(false);
	addChild(infoLabel3, 5);


	icon6 = Sprite::create("achieveIcon.png");

	icon6->setPosition(size.width / 2 - 150, size.height / 2 - 110);
	icon6->setVisible(false);
	addChild(icon6, 5);

	const char *str_13 = ((String*)dic->objectForKey("achieve10"))->_string.c_str();
	infoLabel4 = Label::createWithTTF(str_13, "fonts/b.ttf", 23);
	infoLabel4->setAnchorPoint(Point(0, 0));
	infoLabel4->setPosition(size.width / 2 - 120, size.height / 2 - 110);
	infoLabel4->setVisible(false);
	addChild(infoLabel4, 5);

	///////////////////////////////////道具兑换信息
	//添加标题
	const char *str6 = ((String*)dic->objectForKey("achieve11"))->_string.c_str();
	info2 = Label::createWithTTF(str6, "fonts/b.ttf", 40);
	info2->setPosition(size.width / 2, size.height / 2 + 150);
	info2->setVisible(false);
	addChild(info2, 5);

	pointerLeft2 = Sprite::create("pointer.png");
	pointerLeft2->setRotation(90);
	pointerLeft2->setScale(1.5);
	pointerLeft2->setPosition(size.width / 2 - 100, size.height / 2 + 150);
	pointerLeft2->setVisible(false);
	addChild(pointerLeft2,5);

	pointerRight2 = Sprite::create("pointer.png");
	pointerRight2->setRotation(-90);
	pointerRight2->setScale(1.5);
	pointerRight2->setPosition(size.width / 2 + 100, size.height / 2 + 150);
	addChild(pointerRight2,5);
    pointerRight2->setVisible(false);

	//兑换具体描述
	const char *str7 = ((String*)dic->objectForKey("achieve12"))->_string.c_str();
	des2 = Label::createWithTTF(str7, "fonts/b.ttf", 23);
	des2->setPosition(size.width / 2, size.height / 2 + 50);
	des2->setDimensions(300, 70);
	des2->setVisible(false);
	addChild(des2, 5);

	legendaryDialog = Sprite::create("menu.png");
	legendaryDialog->setPosition(size.width / 2, size.height - 80);
	legendaryDialog->setScaleY(0.9);
	legendaryDialog->setVisible(false);
	addChild(legendaryDialog, 2);

	const char *str8 = ((String*)dic->objectForKey("achieve13"))->_string.c_str();
	legendaryLabelDes = Label::createWithTTF(str8, "fonts/b.ttf", 23);
	legendaryLabelDes->setPosition(size.width / 2-27, size.height - 80);
	legendaryLabelDes->setVisible(false);
	addChild(legendaryLabelDes, 3);

	char legend[10];
	int tempLegend = UserDefault::getInstance()->getIntegerForKey("legendaryNum");
	sprintf(legend,"%d",tempLegend);
	std::string str_legend = legend;
	legendaryLabelNum = Label::createWithTTF(str_legend, "fonts/HANGTHEDJ.ttf", 26);
	legendaryLabelNum->setPosition(size.width / 2 + 40, size.height - 80);
	legendaryLabelNum->setVisible(false);
	addChild(legendaryLabelNum, 3);
	
	
	//数量
	itemChange = Sprite::create("changeIcon.png");
	itemChange->setPosition(size.width / 2 -100, size.height/2-40);
	itemChange->setVisible(false);
	addChild(itemChange, 5);

	char cNum[10];
	sprintf(cNum, "%d", UserDefault::getInstance()->getIntegerForKey("ChangeNum"));
	std::string str_cNum = cNum;
	changeNum = Label::createWithTTF(str_cNum, "fonts/b.ttf", 40);
	changeNum->setPosition(size.width / 2 - 100, size.height / 2 - 120);
	changeNum->setVisible(false);
	addChild(changeNum,5);

	itemBomb = Sprite::create("bomb.png");
	itemBomb->setPosition(size.width / 2 +100, size.height / 2 - 40);
	itemBomb->setVisible(false);
	addChild(itemBomb, 5);

	//数量
	char bNum[10];
	sprintf(bNum, "%d", UserDefault::getInstance()->getIntegerForKey("BombNum"));
	std::string str_bNum = bNum;
	bombNum = Label::createWithTTF(str_bNum, "fonts/b.ttf", 40);
	bombNum->setPosition(size.width / 2 + 100, size.height / 2 - 120);
	bombNum->setVisible(false);
	addChild(bombNum, 5);



	/////////////////////////////////////////换取道具的对话框
	buyDialog = Sprite::create("changeLayerBg.png");
	buyDialog->setPosition(size.width / 2, -100);
	buyDialog->setScaleY(0.65);
	buyDialog->setScaleX(0.9);
	addChild(buyDialog, 5);

	const char *str9 = ((String*)dic->objectForKey("achieve14"))->_string.c_str();
	isBuy = Label::createWithTTF(str9, "fonts/b.ttf", 28);
	isBuy->setPosition(size.width / 2 , -70);
	addChild(isBuy, 6);

	buyYes = Sprite::create("buyYes.png");
	buyYes->setScale(0.6);
	buyYes->setPosition(size.width / 2 - 80, -120);
	addChild(buyYes, 6);

	buyNo = Sprite::create("buyNo.png");
	buyNo->setScale(0.6);
	buyNo->setPosition(size.width / 2 + 70, -120);
	addChild(buyNo, 6);






//////////////////////////////////////////////////////////////////////////////////////////////
	auto vis = CallFunc::create([=](){
		record->setVisible(true);
		pointerLeft->setVisible(true);
		pointerRight->setVisible(true);
		okLabel->setVisible(true);
		ok->setVisible(true);
		icon1->setVisible(true);
		historyBestScoreLabel->setVisible(true);
		historyBestScoreLabelNum->setVisible(true);
		icon2->setVisible(true);
		singleBest->setVisible(true);
		singleBestNum->setVisible(true);
	});
	ActionInterval * delayVis = DelayTime::create(2.2);
	auto seqVis = Sequence::create(delayVis,vis,NULL);
	
	if (getDataManager().getAdded() == 1)
	{		
		this->runAction(seqVis);
	}

	auto vis2 = CallFunc::create([=](){
		info1->setVisible(true);
		pointerLeft1->setVisible(true);
		pointerRight1->setVisible(true);
		okLabel->setVisible(true);
		ok->setVisible(true);
		icon3->setVisible(true);
		icon4->setVisible(true);
		icon5->setVisible(true);
		icon6->setVisible(true);
		infoLabel1->setVisible(true);
		infoLabel2->setVisible(true);
		infoLabel3->setVisible(true);
		infoLabel4->setVisible(true);
	});
	ActionInterval * delayVis2 = DelayTime::create(2.2);
	auto seqVis2 = Sequence::create(delayVis2, vis2, NULL);

	if (getDataManager().getAdded() == 2)
	{
		this->runAction(seqVis2);
	}


	auto vis3 = CallFunc::create([=](){
		info2->setVisible(true);
		pointerLeft2->setVisible(true);
		pointerRight2->setVisible(true);
		okLabel->setVisible(true);
		ok->setVisible(true);
		des2->setVisible(true);
		itemChange->setVisible(true);
		changeNum->setVisible(true);
		itemBomb->setVisible(true);
		bombNum->setVisible(true);

		legendaryDialog->setVisible(true);
		legendaryLabelDes->setVisible(true);
		legendaryLabelNum->setVisible(true);
	});
	ActionInterval * delayVis3 = DelayTime::create(2.2);
	auto seqVis3 = Sequence::create(delayVis3, vis3, NULL);

	if (getDataManager().getAdded() == 3)
	{
		this->runAction(seqVis3);
	}

	auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *t, Event *e){ //info要按引用访问&
	    //删去该层
		if (ok->getBoundingBox().containsPoint(t->getLocation()))
		{
		
			getDataManager().setAchieveLayer(0);
			
			record->setVisible(false);
			pointerLeft->setVisible(false);
			pointerRight->setVisible(false);
			okLabel->setVisible(false);
			ok->setVisible(false);
			icon1->setVisible(false);
			historyBestScoreLabel->setVisible(false);
			historyBestScoreLabelNum->setVisible(false);
			icon2->setVisible(false);
			singleBest->setVisible(false);
			singleBestNum->setVisible(false);

			info1->setVisible(false);
			pointerLeft1->setVisible(false);
			pointerRight1->setVisible(false);
			okLabel->setVisible(false);
			ok->setVisible(false);
			icon3->setVisible(false);
			icon4->setVisible(false);
			icon5->setVisible(false);
			icon6->setVisible(false);
			infoLabel1->setVisible(false);
			infoLabel2->setVisible(false);
			infoLabel3->setVisible(false);
			infoLabel4->setVisible(false);


			info2->setVisible(false);
			pointerLeft2->setVisible(false);
			pointerRight2->setVisible(false);
			okLabel->setVisible(false);
			ok->setVisible(false);
			des2->setVisible(false);
			itemChange->setVisible(false);
			changeNum->setVisible(false);
			itemBomb->setVisible(false);
			bombNum->setVisible(false);

			buyDialog->setVisible(false);
			isBuy->setVisible(false);
			buyYes->setVisible(false);
			buyNo->setVisible(false);
			legendaryDialog->setVisible(false);
			legendaryLabelDes->setVisible(false);
			legendaryLabelNum->setVisible(false);


			auto mt_dialog1 = MoveTo::create(1.5, Vec2(size.width / 2, -500));
			EaseIn * dialog_slow1 = EaseIn::create(mt_dialog1, 7.0);
			
			auto deleteLayer = CallFunc::create([=](){
				this->removeFromParent();
			});
			auto seq = Sequence::create(dialog_slow1, deleteLayer, NULL);
			dialog->runAction(seq);

			
		}
		//点击换取道具:change 道具
		if (itemChange->getBoundingBox().containsPoint(t->getLocation()) && getDataManager().getAdded() == 3 && isClickedChange==false)
		{
			itemBomb->stopAllActions();
			changeOrBomb = 1;
			auto mt = MoveTo::create(0.5, Vec2(size.width / 2, 90));
			auto mt1 = MoveTo::create(0.5, Vec2(size.width / 2, 120));
			auto mt2 = MoveTo::create(0.5, Vec2(size.width / 2 - 80, 70));
			auto mt3 = MoveTo::create(0.5, Vec2(size.width / 2 +70, 70));
			buyDialog->runAction(mt);
			isBuy->runAction(mt1);
			buyYes->runAction(mt2);
			buyNo->runAction(mt3);

			//该道具放大缩小动画
			auto st = ScaleBy::create(1.0, 1.2);
			auto seq = Sequence::create(st, st->reverse(), NULL);
			auto rp = RepeatForever::create(seq);
			itemChange->runAction(rp);
			
			isClickedChange = true;
			SimpleAudioEngine::getInstance()->playEffect("button.OGG");


		}
		//点击换取 bomb道局
		if (itemBomb->getBoundingBox().containsPoint(t->getLocation()) && getDataManager().getAdded() == 3 && isClickedBomb == false)
		{
			itemChange->stopAllActions();
			changeOrBomb = 2;
			auto mt = MoveTo::create(0.5, Vec2(size.width / 2, 90));
			auto mt1 = MoveTo::create(0.5, Vec2(size.width / 2, 120));
			auto mt2 = MoveTo::create(0.5, Vec2(size.width / 2 - 80, 70));
			auto mt3 = MoveTo::create(0.5, Vec2(size.width / 2 + 70, 70));
			buyDialog->runAction(mt);
			isBuy->runAction(mt1);
			buyYes->runAction(mt2);
			buyNo->runAction(mt3);

			//该道具放大缩小动画
			auto st = ScaleBy::create(1.0, 1.2);
			auto seq = Sequence::create(st, st->reverse(), NULL);
			auto rp = RepeatForever::create(seq);
			itemBomb->runAction(rp);

			isClickedBomb = true;
			SimpleAudioEngine::getInstance()->playEffect("button.OGG");

		}
		//点击 是，兑换change道具
		if (buyYes->getBoundingBox().containsPoint(t->getLocation()))
		{
			if (changeOrBomb == 1)
			{
				itemBomb->stopAllActions();
				int tpl = UserDefault::getInstance()->getIntegerForKey("legendaryNum");
				if (tpl >= 3) //大于10才能兑换
				{
					//music
					SimpleAudioEngine::getInstance()->playEffect("buySuccess.OGG");

					UserDefault::getInstance()->setIntegerForKey("legendaryNum", tpl - 3);
					legendaryLabelNum->setString(StringUtils::format("%d", tpl - 3));
					//个数+1
					int tempChange = UserDefault::getInstance()->getIntegerForKey("ChangeNum");
					UserDefault::getInstance()->setIntegerForKey("ChangeNum", tempChange + 1);
					changeNum->setString(StringUtils::format("%d", tempChange + 1));

					UserDefault::getInstance()->flush();

					//动画效果
					auto plus = Sprite::create("plus1.png");
					plus->setPosition(size.width / 2 - 60, size.height / 2 - 40);
					plus->setScale(0.001);
					addChild(plus, 10);
					auto st = ScaleTo::create(0.5, 1);
					auto mt = MoveTo::create(1.0, Vec2(size.width / 2 - 60, size.height / 2 + 40));
					auto st2 = ScaleTo::create(0.5, 0.001);
					auto deleteP = CallFunc::create([=](){
						plus->removeFromParent();
					});
					auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
					plus->runAction(seq);

				}
				else
				{
					//music
					SimpleAudioEngine::getInstance()->playEffect("buyFail.OGG");

					auto warning = Sprite::create("yellowWarning.png");
					warning->setPosition(size.width / 2 + 140, size.height - 80);
					warning->setScale(0.8);
					warning->setOpacity(0);
					addChild(warning, 10);

					auto st = FadeIn::create(0.5);
					auto seq = Sequence::create(st, st->reverse(), NULL);
					auto rp = Repeat::create(seq, 2);
					auto deleteW = CallFunc::create([=](){
						warning->removeFromParent();
					});
					auto seq2 = Sequence::create(rp, deleteW, NULL);
					warning->runAction(seq2);

				}
			}
			else if (changeOrBomb == 2)
			{
				itemChange->stopAllActions();
				int tpl = UserDefault::getInstance()->getIntegerForKey("legendaryNum");
				if (tpl >= 3) //大于10才能兑换
				{
					//music
					SimpleAudioEngine::getInstance()->playEffect("buySuccess.OGG");

					UserDefault::getInstance()->setIntegerForKey("legendaryNum", tpl - 3);
					legendaryLabelNum->setString(StringUtils::format("%d", tpl - 3));
					//个数+1
					int tempChange = UserDefault::getInstance()->getIntegerForKey("BombNum");
					UserDefault::getInstance()->setIntegerForKey("BombNum", tempChange + 1);
					bombNum->setString(StringUtils::format("%d", tempChange + 1));

					UserDefault::getInstance()->flush();

					//动画效果
					auto plus = Sprite::create("plus1.png");
					plus->setPosition(size.width / 2 +120, size.height / 2 - 40);
					plus->setScale(0.001);
					addChild(plus, 10);
					auto st = ScaleTo::create(0.5, 1);
					auto mt = MoveTo::create(1.0, Vec2(size.width / 2 +120, size.height / 2 + 40));
					auto st2 = ScaleTo::create(0.5, 0.001);
					auto deleteP = CallFunc::create([=](){
						plus->removeFromParent();
					});
					auto seq = Sequence::create(st, mt, st2, deleteP, NULL);
					plus->runAction(seq);

				}
				else
				{
					//music
					SimpleAudioEngine::getInstance()->playEffect("buyFail.OGG");

					auto warning = Sprite::create("yellowWarning.png");
					warning->setPosition(size.width / 2 + 140, size.height - 80);
					warning->setScale(0.8);
					warning->setOpacity(0);
					addChild(warning, 10);

					auto st = FadeIn::create(0.5);
					auto seq = Sequence::create(st, st->reverse(), NULL);
					auto rp = Repeat::create(seq, 2);
					auto deleteW = CallFunc::create([=](){
						warning->removeFromParent();
					});
					auto seq2 = Sequence::create(rp, deleteW, NULL);
					warning->runAction(seq2);
				}
			}

		}
		//点击取消
		if (buyNo->getBoundingBox().containsPoint(t->getLocation()))
		{
			auto mt = MoveTo::create(0.5, Vec2(size.width / 2, -100));
			auto mt1 = MoveTo::create(0.5, Vec2(size.width / 2, -70));
			auto mt2 = MoveTo::create(0.5, Vec2(size.width / 2 - 80,-120));
			auto mt3 = MoveTo::create(0.5, Vec2(size.width / 2 + 70, -120));
			buyDialog->runAction(mt);
			isBuy->runAction(mt1);
			buyYes->runAction(mt2);
			buyNo->runAction(mt3);
			itemChange->stopAllActions();
			itemChange->setScale(1);
			itemBomb->stopAllActions();
			itemBomb->setScale(1);

			isClickedChange = false;
			isClickedBomb = false;

			SimpleAudioEngine::getInstance()->playEffect("button.OGG");

		}
		return true;

	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}
