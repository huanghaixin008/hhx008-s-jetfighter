#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "Constants.h"

class JFMenu : public cocos2d::Layer
{
private:
	cocos2d::Label *author;
	cocos2d::Label *label;
	cocos2d::Menu *menu;
	cocos2d::Menu *options;
	cocos2d::Menu *hscore;
	cocos2d::Sprite *leftArrow;
	cocos2d::Sprite *rightArrow;
	int jetLevel;
	int currJet;
	cocos2d::Sprite *jet;

	void registerJetMove();
	void startAnimation(cocos2d::Size visibleSize);

	std::string toDisplayFormat(int);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuStartCallback(cocos2d::Ref* pSender);
	void menuOptionsCallback(cocos2d::Ref* pSender);
	void menuHScoreCallback(cocos2d::Ref* pSender);

	void toggleMusicCallBack(cocos2d::Ref* pSender);
	void toggleSEffectCallBack(cocos2d::Ref* pSender);

	void backCallBack(cocos2d::Ref* pSender);

    CREATE_FUNC(JFMenu);
};

#endif // __MENU_SCENE_H__
