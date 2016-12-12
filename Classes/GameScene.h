#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Units.h"

class JFGame : public cocos2d::Layer
{
private:
	cocos2d::Vec2 origin;
	cocos2d::Size visibleSize;

	cocos2d::Label* honorDisplay;
	bool toggleMusic;
	bool toggleSE;
	int mcShootSEid = -1;

	bool isDead;
	int currJet;
	Jet* mcharacter;
	std::vector<UnitSprite*> enemies;
	std::vector<UnitSprite*> jbullets; // jet bullets
	std::vector<UnitSprite*> ebullets; // enemy bullets

	cocos2d::Sprite* bg1;
	cocos2d::Sprite* bg2;
	cocos2d::Sprite* inbox;
	cocos2d::Sprite* outbox;
	cocos2d::Sprite* bombIcons[3];

	cocos2d::Label* toast;

	bool isPaused;
	cocos2d::Menu* pauseMenu;
	cocos2d::Menu* deadMenu;
	void menuResumeCallback(cocos2d::Ref* pSender);
	void menuRetryCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);

	cocos2d::Vector<cocos2d::SpriteFrame*> frames;

	DIREC directPressed;
	int difficulty;
	float nextEnemySpawn;
	float specialEnemySpawn;

	int rank;
	int honor;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float) override;

	int handleCollision();
	void enemySpawn(float);
	void reclaimUnits();

	void honorIncrease(int);
	void updateHealthBox();
	void reorderScores();
	void refreshHonorDisplay();

	cocos2d::Animate* generateDeathAnimation(std::string, int, int);

	CREATE_FUNC(JFGame);
};

#endif // __GAME_SCENE_H__