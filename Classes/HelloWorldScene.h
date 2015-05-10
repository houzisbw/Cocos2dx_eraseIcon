#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "changeIconLayer.h"
#include "GirlTwo.h"
#include <cstring>



class HelloWorld : public cocos2d::Layer
{
public:
    
	friend class changeIconLayer;
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	//���װ��(10*10)
	void addEquipment();

	//�������е���
	void generatePoint();

	//�õ���Ӧ��װ��
	cocos2d::Sprite* getEquipWithCoordinate(cocos2d::Sprite * e[][10], int row, int column);

	//�ҳ����ڵ�װ���ĺ���
	void findAdjacentEquip(int b[][10], int row, int column,int cur,bool single);

	//ȷ���Ƿ񵥶�һ��װ��
	bool isSingleIcon(int b[][10], int row, int column, int cur);

	//���ƺ���
	void moveDown(int b[][10]);

	//��ӡģ������ĺ���
	void printBoard(int b[][10]);

	//�����ƶ��ĺ���
	void moveLeft(int b[][10]);

	//�����Ϸ�Ƿ��������û��2��������װ��
	bool isGameOver();

	//װ����ȥ��Ч
	void equipmentEffect(int boardValue,cocos2d::Point pos);
	void equipmentEffectWithNoGravity(int boardValue, cocos2d::Point pos);

	//ʤ����Ч
	void winEffect(cocos2d::Point pos);

	//�ص����˵�
	void returnToMenu(cocos2d::Ref* r);

	//���¿�ʼ
	void restartGame(cocos2d::Ref* r);

	//��ʱ���õĺ��������ڶ�̬����
	void gameStep(float dt);

	//��Ϸ����ʱ�ݻ�ʣ�µķ���
	void destoryRemaining();

	//��ʼ��ʱ����
	void startTimer();
	
	//������ʱ

	void endTimer();
	
	//��ʱ����
	virtual void update(float dt);

	//ȥ��һ�غ���
	void goToNextLevel(Ref * r);

	//���ߺ����� �ı�����
	void changeIconType(Ref * r);

	//���ߺ����� ��ըicon
	void bombIconType(Ref * r);

	//�ɵ�����icon�ĺ�����0.2�����һ��,type��ʾ��row����column����
	void hitIconByMissile(int column,int row,int type);

	//�ɵ�����icon�õ��ķ���,һ��iconֻ��5�֣������ͱ�֤����Ƶ��ʹ�÷ɵ�
	void scoreGetFromMissile(int posX,int posY);
    
	//�滻iconʱ�Ķ�������
	void changeIconFlyAnimation(int temp,cocos2d::Point target);
	
	//warning�ص�����
	void warningCallback(cocos2d::Ref *r);

	//���߲�����ʾ����
	void notEnoughItem();


    std::vector<int> icon;

	//gamePauseLayer ���õ�ǰ��level��
	static int getGlobalLevel();



    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	
	
	
	cocos2d::PointArray *pointArray;//������Ÿ���װ�����������
	int board[10][10];//ģ��ڷ�װ��������
	cocos2d::Vector<cocos2d::Sprite*> equip;//�������װ�������� ��ָ��
	cocos2d::Sprite* equipment[10][10];//���װ��������
	int count;//����������������2����������
	cocos2d::MenuItemImage * btm;//�������˵�
	cocos2d::MenuItemImage * res;//���¿�ʼ
	cocos2d::Label * label_score;//��ʾ�����Ŀؼ�
	int currentScore; // ��Ϸ��ǰ����
	cocos2d::Label * lb;
	cocos2d::Vector<cocos2d::Label*> vec_label;
	bool isDouble;
	int tempScore;
	int totalScore;//һ���������ܷ���
	//cocos2d::Label* totalScoreLabel;//���������ﶨ�壬������ʱ�������
	cocos2d::Sprite * gameEnd;
	
	//����ͳ��
	int statistic_LeftIcon;
	int statistic_LeftBonus;
	int statistic_SingleMost;
	int statistic_MostIcon;

	int four_kill;
	int five_kill;
	int six_kill;
	int seven_kill;
	int legendary_kill;

	double statistic_Time;
	cocos2d::Label * leftIcon_label;
	cocos2d::Label * leftBonus_label;
	cocos2d::Label * singleMost_label;
	cocos2d::Label * mostIcon_label;
	cocos2d::Label * timer_label;//���label���������ͳ����ʾʱ���
	cocos2d::Label * timerLabel; //���label��ʵʱ��ʾʱ���

	cocos2d::Label * fourKill_label;
	cocos2d::Label * fiveKill_label;
	cocos2d::Label * sixKill_label;
	cocos2d::Label * sevenKill_label;
	cocos2d::Label * legendaryKill_label;
	//ʣ�ཱ������
	std::vector<int> bonus_vec;
	//��¼5��icon�ֱ���ȥ�ĸ�����map
	std::map<int, int> icon_map;
	//��ʱ��
	bool timerRunning;
	float startTime;//��Ϸ��ʼ��ʱ��
	bool gameIsOver;//��־��Ϸ�����ı���
	//��Ϸicon֪ʶ�Ĵ洢
	//std::string info[19][4];
	//std::vector<std::string*> info_vec; //vector����string����
	cocos2d::Label * knowledge_label;
	cocos2d::Label * name_label;
	cocos2d::Sprite * small_pic;

	//ÿһ�ص�ǰĿ�����
	int target_score;
	int curLevel;
	cocos2d::Label * targetScore_label;
	cocos2d::Label * level_label;
	cocos2d::Label * levelNum_label;
	
	//��һ��menu��ť
	cocos2d::MenuItemImage * nextLevel;
	cocos2d::Label * nextLevelLabel;

	//ready go Ʈ��label
	cocos2d::Label * readyLabel;
	cocos2d::Label * goLabel;
	cocos2d::Sprite * transBg;

	bool isGameReady;//��ʼ�������˲��ܵ����������
	bool isTargetReached;//�Ƿ񵽴�Ŀ�����

	cocos2d::Sprite * girl;//ÿһ�ؿ�ʼǰ���ܵ�Ů��
	cocos2d::Label * welcome;//��ӭ����
	cocos2d::Label * spots;//�ص�

	//2�������Ե���
	cocos2d::MenuItemImage * changeIcon;
	cocos2d::MenuItemImage * bombIcon;

	//2�����ߵ������Լ�label
	
	cocos2d::Label * changeIconLabel;
	cocos2d::Label * bombIconLabel;

	//2����������
	cocos2d::Sprite * tower1;
	cocos2d::Sprite * tower2;
	//2���ɵ�
	cocos2d::Sprite * missile1;
	cocos2d::Sprite * missile2;

	//���߲������ʾͼƬ
	cocos2d::Sprite * notEnough;
	cocos2d::MenuItemImage * notEnoughMII;
	cocos2d::Menu * notEnoughMenu;
	cocos2d::Label * ok;
	cocos2d::Sprite * yellow;
	cocos2d::Label * notEnoughDes;

	//ʣ�ཱ������
	std::vector<int> leftBonus;
	//ʣ�ཱ��ͼ��box
	cocos2d::Sprite * leftBonusBox;

	bool firstHit;//�ж��Ƿ��һ�ε��

	cocos2d::Sprite * clear;

	bool isGameOverHit;//��ֹ��Ϸ����ʱ��ε��

	//�˹����� ��ϵŮ�� ������
	cocos2d::Sprite * lqy;
	float vy_lqy; //����y���ٶ�
	void moveLqy(float dt);
	float posY_lqy;//lqy��y���λ��
	cocos2d::Sprite * lqy_dialog;
	cocos2d::Point lastPos;//λ������仯�����϶��ˣ����϶���Ч
	cocos2d::Point thisPos;

	//22��
	GirlTwo * tg;
	bool isPlayed;
	cocos2d::Label * girlTalk;

	//����ͳ�Ƶ�label��
	cocos2d::Label * statisticTitle;
	cocos2d::Label * title1;
	cocos2d::Sprite * pointer1;
	cocos2d::Label * title2;
	cocos2d::Sprite * pointer2;
	cocos2d::Label * title3;
	cocos2d::Sprite * pointer3;
	cocos2d::Label * title4;
	cocos2d::Sprite * pointer4;
	cocos2d::Label * title5;
	cocos2d::Sprite * pointer5;
	cocos2d::Label * title6;
	cocos2d::Sprite * pointer6;
	cocos2d::Label * title7;
	cocos2d::Sprite * pointer7;
	cocos2d::Label * title8;
	cocos2d::Sprite * pointer8;
	cocos2d::Label * title9;
	cocos2d::Sprite * pointer9;
	cocos2d::Label * title10;
	cocos2d::Sprite * pointer10;

	cocos2d::Menu * menuNextLevel;
	cocos2d::Sprite * clear2;

	


};



#endif // __HELLOWORLD_SCENE_H__
