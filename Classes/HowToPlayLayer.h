#ifndef _HOWTOPLAYLAYER_H_
#define _HOWTOPLAYLAYER_H_
#include "cocos2d.h"
class HowToPlayLayer:public cocos2d::Layer
{
public:
	HowToPlayLayer();
	~HowToPlayLayer();
	virtual bool init();
	CREATE_FUNC(HowToPlayLayer);

	//�����ƶ�����
	void move(float dt);

	//NEXT���ܻص�����
	void nextIntro(cocos2d::Ref * r);

	//���º�����girl�ƶ�����
	void girlMove(std::string str, float girlPosY, cocos2d::Point destination);
	//��ʼ��Ϸ��ɾ��layer
	void deleteLayer();

private:
	cocos2d::Sprite * bg;
	cocos2d::Sprite * girl;
	float vy_girl; //����y���ٶ�
	float girlPosY;//���õ�yλ�ã���Ϊmove�������ܴ��ݽ�pos����
	cocos2d::Sprite * dialog;//�Ի��򣬽���ÿһ���
	cocos2d::Label * function;//label˵������
	cocos2d::MenuItemSprite * next;//��һ�����
	cocos2d::Menu * menuNext;
	cocos2d::Sprite * hand;
	//����ÿһ���ı���
	int progress;
};
#endif

