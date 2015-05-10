#ifndef _GIRLTWO_H_
#define _GIRLTWO_H_
#include "cocos2d.h"
class GirlTwo :public cocos2d::Sprite
{
public:
	GirlTwo();
	~GirlTwo();
	virtual bool init();
	CREATE_FUNC(GirlTwo);
public:
	void girlAction();
};
#endif

