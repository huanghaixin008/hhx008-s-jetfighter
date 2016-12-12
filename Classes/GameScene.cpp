#include "GameScene.h"
#include "MenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* JFGame::createScene()
{
	auto scene = Scene::create();
	auto layer = JFGame::create();
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool JFGame::init()
{
	if (!Layer::init())
		return false;

	srand((unsigned)time(NULL));

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// read config here
	toggleMusic = UserDefault::getInstance()->getBoolForKey("toggleMusic", true);
	toggleSE = UserDefault::getInstance()->getBoolForKey("toggleSE", true);
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	//audio->preloadBackgroundMusic("Pacific Rim.mp3");
	//if (toggleMusic)
		//audio->playBackgroundMusic("Pacific Rim.mp3");

	//audio->preloadEffect("gunshot.wav");
	//audio->preloadEffect("explosion.wav");

	// init parameters
	currJet = UserDefault::getInstance()->getIntegerForKey("currJet", 0);
	isDead = false;
	directPressed = 0x0000;
	difficulty = ENEMY_DIFFICULTY_EASY;
	nextEnemySpawn = 1.0f;
	specialEnemySpawn = 20.0f;
	honor = 0;
	rank = 4;

	bg1 = Sprite::create(R_BG_FILENAME);
	bg1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg1, 0);
	bg2 = Sprite::create(R_BG_FILENAME);
	bg2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + visibleSize.height));
	this->addChild(bg2, 0);

	// HUD
	inbox = Sprite::create(R_INBOX_FILENAME);
	inbox->setScale(0.4f);
	inbox->setAnchorPoint(Vec2(0, 0));
	inbox->setPosition(Vec2(18, 18));
	this->addChild(inbox, 2);
	outbox = Sprite::create(R_OUTBOX_FILENAME);
	outbox->setScale(0.4f);
	outbox->setAnchorPoint(Vec2(0, 0));
	outbox->setPosition(Vec2(12, 12));
	this->addChild(outbox, 2);

	toast = Label::createWithTTF("#", "fonts/Marker Felt.ttf", 40);
	toast->setAnchorPoint(Vec2(0.5,0.5));
	toast->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
	toast->setOpacity(0);
	this->addChild(toast, 2);

	honorDisplay = Label::createWithTTF("#","fonts/Marker Felt.ttf", 30);
	honorDisplay->setAnchorPoint(Vec2(1.1,1.4));
	honorDisplay->setPosition(Vec2(visibleSize.width, visibleSize.height));
	refreshHonorDisplay();
	this->addChild(honorDisplay, 2);

	// pause menu init
	isPaused = false;
	auto itemResume = MenuItemLabel::create(Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFGame::menuResumeCallback, this));
	auto itemMainMenu = MenuItemLabel::create(Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFGame::menuBackCallback, this));
	auto itemQuit = MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24),
		[](Ref* pSender)->void { Director::getInstance()->end(); });
	pauseMenu = Menu::create(itemResume, itemMainMenu, itemQuit, NULL);
	pauseMenu->alignItemsVerticallyWithPadding(20);
	pauseMenu->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	pauseMenu->setOpacity(0);
	pauseMenu->setEnabled(false);
	this->addChild(pauseMenu, 2);

	// dead menu init
	auto itemLabel = MenuItemLabel::create(Label::createWithTTF("YOU ARE DEAD", "fonts/Marker Felt.ttf", 36));
	auto itemRetry = MenuItemLabel::create(Label::createWithTTF("Retry", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFGame::menuRetryCallback, this));
	auto itemMainMenu2 = MenuItemLabel::create(Label::createWithTTF("Main Menu", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFGame::menuBackCallback, this));
	auto itemQuit2 = MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24),
		[](Ref* pSender)->void { Director::getInstance()->end(); });
	deadMenu = Menu::create(itemLabel, itemRetry, itemMainMenu2, itemQuit2, NULL);
	deadMenu->alignItemsVerticallyWithPadding(20);
	deadMenu->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	deadMenu->setOpacity(0);
	deadMenu->setEnabled(false);
	this->addChild(deadMenu, 2);

	//main character init
	switch (currJet) {
	default:
	case 0:
		mcharacter = new BlueJet();
		break;
	case 1:
		mcharacter = new StrongJet();
		break;
	case 2:
		mcharacter = new SmallJet();
		break;
	case 3:
		mcharacter = new MagicJet();
		break;
	}
	Size mcsize = mcharacter->getSprite()->getBoundingBox().size;
	mcharacter->getSprite()->setPosition(Vec2(origin.x + visibleSize.width / 2 - mcsize.width / 2, 0 - mcsize.height / 2));
	this->scheduleOnce([=](float delta) {
		auto startAction = MoveTo::create(1, Vec2(origin.x + visibleSize.width / 2 - mcsize.width / 2, mcsize.height));
		mcharacter->getSprite()->runAction(startAction);
	}, 0.2f, "startIn");
	this->addChild(mcharacter->getSprite(), 1);

	auto keyEventListener = EventListenerKeyboard::create();
	keyEventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			if (!isDead)
				mcharacter->toFlyLeftTexture();
			directPressed |= 0x2;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			if (!isDead)
				mcharacter->toFlyRightTexture();
			directPressed |= 0x1;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			directPressed |= 0x8;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			directPressed |= 0x4;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			if (mcharacter->getUltiCount() > 0) {
				this->removeChild(bombIcons[mcharacter->getUltiCount() - 1]);
				mcharacter->ultimateOn(&enemies, &ebullets, &jbullets);
			}
		}
	};
	keyEventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			directPressed &= 0xd;
			if (!isDead && directPressed == DIREC_UP || directPressed == DIREC_DOWN || directPressed == DIREC_NONE)
				mcharacter->toNormalTexture();
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			directPressed &= 0xe;
			if (!isDead && directPressed == DIREC_UP || directPressed == DIREC_DOWN || directPressed == DIREC_NONE)
				mcharacter->toNormalTexture();
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			directPressed &= 0x7;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			directPressed &= 0xb;
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyEventListener, mcharacter->getSprite());

	auto pauseKeyEventListener = EventListenerKeyboard::create();
	pauseKeyEventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode != EventKeyboard::KeyCode::KEY_ESCAPE)
			return;
		if (isDead)
			return;
		if (isPaused) {
			isPaused = false;
			pauseMenu->setEnabled(false);
			auto appear = FadeTo::create(0.5f, 0);
			pauseMenu->runAction(appear);
			this->resumeSchedulerAndActions();
		}
		else {
			isPaused = true;
			pauseMenu->setEnabled(true);
			auto appear = FadeTo::create(0.5f, 255);
			pauseMenu->runAction(appear);
			this->pauseSchedulerAndActions();
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(pauseKeyEventListener, pauseMenu);
	/*
	if (toggleSE)
		this->scheduleOnce([=](float delta) {
				mcShootSEid = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("gunshot.mp3", true);
		}, 0.2f, "seasbgm");*/

	this->scheduleUpdate();

	return true;
}

void JFGame::update(float delta) {
	// background scroll
	bg1->setPosition(Vec2(bg1->getPosition().x, bg1->getPosition().y - delta * BG_SCROLL_SPEED));
	bg2->setPosition(Vec2(bg2->getPosition().x, bg2->getPosition().y - delta * BG_SCROLL_SPEED));
	if (bg1->getPosition().y <= 0 - visibleSize.height / 2) {
		bg1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + visibleSize.height));
		bg1->runAction(cocos2d::FlipX::create(true));
	}
	else if (bg2->getPosition().y <= 0 - visibleSize.height / 2) {
		bg2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + visibleSize.height));
		bg2->runAction(cocos2d::FlipX::create(true));
	}

	handleCollision();
	reclaimUnits();

	// counter counts
	if (!isDead)
		mcharacter->counterCount(delta);
	for (UnitSprite* us : jbullets)
		us->counterCount(delta);
	for (UnitSprite* us : ebullets)
		us->counterCount(delta);
	for (UnitSprite* us : enemies)
		us->counterCount(delta);

	// bullets move
	for (UnitSprite* us : jbullets)
		us->moveBy(delta, 0x0);  // it will ignore direct input
	for (UnitSprite* us : ebullets)
		us->moveBy(delta, 0x0);  
	// enemies move
	for (UnitSprite* us : enemies)
		us->moveBy(delta, 0x0);

	std::vector<UnitSprite*> bullets;
	if (!isDead) {
		// jet moves
		DIREC finalDirec = directPressed;
		Vec2 pos = mcharacter->getSprite()->getPosition();
		Size size = mcharacter->getSprite()->getBoundingBox().size;
		// limited in visible size
		if (pos.x + size.width >= visibleSize.width)
			finalDirec &= 0xe;
		if (pos.x <= 0)
			finalDirec &= 0xd;
		if (pos.y + size.height >= visibleSize.height)
			finalDirec &= 0x7;
		if (pos.y <= 0)
			finalDirec &= 0xb;
		mcharacter->moveBy(delta, finalDirec);

		// jet shoot
		bullets = mcharacter->shoot(delta);
		if (bullets.size() != 0)
			for (auto bullet : bullets)
				jbullets.push_back(bullet);

		// check jet traits
		mcharacter->checkTraitDuration(delta);
		mcharacter->checkUltimateDuration(delta, &enemies, &ebullets, &jbullets);
	}

	// enemy shoot
	for (UnitSprite* us : enemies) {
		bullets = us->shoot(delta);
		if (bullets.size() != 0)
			for (auto bullet : bullets)
				ebullets.push_back(bullet);
	}

	enemySpawn(delta);
}

int JFGame::handleCollision() {
	int count = 0;

	// 1. handle coollision between jbullets and enemies
	for (UnitSprite* jb : jbullets) {
		if (((Bullet*)jb)->getCollisionType() == COLLISIONTYPE_BULLET)
			continue;
		for (UnitSprite* enemy : enemies)
			if (jb->isCollidedWith(enemy)) {
				jb->takeDamage(enemy->countDamege());
				enemy->takeDamage(jb->countDamege());
				count++;
				//log("ENEMY IS HIT");
			}
	}

	// 4. handle coollision between bullets
	for (UnitSprite* jb : jbullets) {
		for (UnitSprite* eb : ebullets) {
			if (((Bullet*)jb)->getCollisionType() == COLLISIONTYPE_AIRCRAFT && ((Bullet*)eb)->getCollisionType() == COLLISIONTYPE_AIRCRAFT)
				continue;
			if (jb->isCollidedWith(eb)) {
				jb->takeDamage(eb->countDamege());
				eb->takeDamage(jb->countDamege());
				count++;
				//log("ENEMY IS HIT");
			}
		}
	}

	if (!isDead) {
		// 2. handle collision between ebullets and jet
		for (UnitSprite* eb : ebullets) {
			if (((Bullet*)eb)->getCollisionType() == COLLISIONTYPE_BULLET)
				continue;
			if (eb->isCollidedWith(mcharacter)) {
				eb->takeDamage(mcharacter->countDamege());
				mcharacter->takeDamage(eb->countDamege());
				updateHealthBox();
				count++;
				//log("YOU ARE HIT");
			}
		}

		// 3. handle coollision between jet and enemies
		for (UnitSprite* enemy : enemies)
			if (enemy->isCollidedWith(mcharacter)) {
			enemy->takeDamage(mcharacter->countDamege());
			mcharacter->takeDamage(enemy->countDamege());
			updateHealthBox();
			count++;
			//log("ENEMY IS HIT");
			}
	}

	return count;
}

void JFGame::enemySpawn(float delta) {
	if (specialEnemySpawn <= 0) {
		specialEnemySpawn = SPECIAL_ENEMY_SPAWN_BASE;
		if (rand() % 2) {  // 50% chance
			int type = rand() % 4;
			std::string filename;
			switch (type) {
			case 0:
				filename = R_SENEMY_POWER_FILENAME;
				break;
			case 1:
				filename = R_SENEMY_SPEED_FILENAME;
				break;
			case 2:
				filename = R_SENEMY_HEALTH_FILENAME;
				break;
			case 3:
				filename = R_SENEMY_BOMB_FILENAME;
				break;
			}

			Enemy* specialEnemy = new Enemy(filename, ENEMY_DIFFICULTY_SPECIAL);
			specialEnemy->getSprite()->setPosition(Vec2((visibleSize.width / 10) * (rand() % 10),
				visibleSize.height  + specialEnemy->getSprite()->getContentSize().height));
			this->addChild(specialEnemy->getSprite(), 1);
			enemies.push_back(specialEnemy);
		}
	} else specialEnemySpawn -= delta;

	if (nextEnemySpawn <= 0) {
		// reset nextEnemySpawn
		nextEnemySpawn = NEXT_ENEMY_SPAWN_BASE / (float)(random() % 2 + 1);

		int spawnNum = random() % 4 + 5; // 5 ~ 8
		int spawnPosBase = visibleSize.width / spawnNum;
		int widthShift = visibleSize.width / (spawnNum * 10);
		// random spawns
		for (int i = 0; i < spawnNum; i++)
			if (random() % 10 < ENEMY_SPAWN_LIKELYHOOD) { // spawn one
				Enemy* newEnemy = new Enemy(R_ENEMY_FILENAME, difficulty - rand() % difficulty);
				int heightShift = random() % (int)(nextEnemySpawn * SPEED_BASE * newEnemy->getSpeed()
																	- newEnemy->getSprite()->getContentSize().height);
				newEnemy->getSprite()->setPosition(Vec2(spawnPosBase * i + widthShift, 
																	visibleSize.height + heightShift));
				this->addChild(newEnemy->getSprite(), 1);
				enemies.push_back(newEnemy);
				//log("spawn one enemy");
			}
	} else nextEnemySpawn -= delta;
}

// reclaim one when it's dead or out of bound
void JFGame::reclaimUnits() {
	// jet
	if (!isDead && mcharacter->isDestroyed()) {
		isDead = true;
		mcharacter->getSprite()->pause();
		Vector<FiniteTimeAction*> actions;
		/*
		if (toggleSE) {
			CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(mcShootSEid);
			actions.pushBack(CallFunc::create([=]() {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.mp3"); }));
		}*/
		actions.pushBack(generateDeathAnimation("explo.png", 142, 150));
		actions.pushBack(CallFunc::create([=]() { delete mcharacter; }));
		mcharacter->getSprite()->runAction(Sequence::create(actions));

		// show dead menu here
		toast->pause();
		toast->setOpacity(0);
		deadMenu->setEnabled(true);
		deadMenu->runAction(FadeTo::create(0.5f, 255));
	} 

	UnitSprite* tmp;
	// enemies
	for (auto it = enemies.begin(); it != enemies.end();) {
		auto esprite = (*it)->getSprite();
		if ((*it)->isDestroyed()) {
			// destroy special enemies
			if (!isDead) {
				switch (((Enemy*)(*it))->getStrategy()) {
				case ENEMY_DIFFICULTY_SPECIAL_BOMB:
					toast->setString("Get Bomb!");
					toast->setOpacity(255);
					toast->runAction(FadeTo::create(1.5f, 0));
					if (mcharacter->ultiCountUp()) {
						// update HUD
						int currCount = mcharacter->getUltiCount() - 1;
						Sprite* newBombIcon = Sprite::create(R_BOMB_ICON_FILENAME);
						newBombIcon->setScale(0.3f);
						newBombIcon->setAnchorPoint(Vec2(1.0f, 0));
						newBombIcon->setPosition(Vec2(visibleSize.width - currCount * (newBombIcon->getBoundingBox().size.width + 12) - 12, 12));
						bombIcons[currCount] = newBombIcon;
						this->addChild(newBombIcon, 2);
					}
					break;
				case ENEMY_DIFFICULTY_SPECIAL_POWER:
					toast->setString("Power Up!");
					toast->setOpacity(255);
					toast->runAction(FadeTo::create(1.5f, 0));
					mcharacter->powerUp();
					break;
				case ENEMY_DIFFICULTY_SPECIAL_SPEED:
					toast->setString("Speed Up!");
					toast->setOpacity(255);
					toast->runAction(FadeTo::create(1.5f, 0));
					mcharacter->speedUp();
					break;
				case ENEMY_DIFFICULTY_SPECIAL_HEALTH:
					toast->setString("Restore!");
					toast->setOpacity(255);
					toast->runAction(FadeTo::create(1.5f, 0));
					mcharacter->restore();
					updateHealthBox();
				default:
					break;
				}
			}

			honorIncrease(((Enemy*)(*it))->countHonor());
			Vector<FiniteTimeAction*> actions;
			/*
			if (toggleSE)
				actions.pushBack(CallFunc::create([=]() {
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("explosion.mp3"); })); */
			actions.pushBack(generateDeathAnimation("explo.png", 142, 150));
			tmp = *it;
			actions.pushBack(CallFunc::create([tmp]() {delete tmp; }));
			esprite->runAction(Sequence::create(actions));
			it = enemies.erase(it);
		}
		else if ((*it)->getWaste() || esprite->getPosition().y <= 0 - esprite->getBoundingBox().size.height) {
			delete *it;
			it = enemies.erase(it);
		}
		else it++;
	}

	// bullet
	for (auto it = jbullets.begin(); it != jbullets.end();) {
		auto esprite = (*it)->getSprite();
		if ((*it)->isDestroyed()) {
			Vector<FiniteTimeAction*> actions;
			actions.pushBack(generateDeathAnimation("explo_small.png", 74, 79));
			tmp = *it;
			actions.pushBack(CallFunc::create([tmp]() {delete tmp; }));
			esprite->runAction(Sequence::create(actions));
			it = jbullets.erase(it);
		} 
		else if ((*it)->getWaste() || esprite->getPosition().y >= visibleSize.height ||
			esprite->getPosition().y <= 0 - esprite->getBoundingBox().size.height ||
			esprite->getPosition().x <= 0 - esprite->getBoundingBox().size.width || 
			esprite->getPosition().x >= visibleSize.width) {
			delete *it;
			it = jbullets.erase(it);
		}
		else it++;
	}
	for (auto it = ebullets.begin(); it != ebullets.end();) {
		auto esprite = (*it)->getSprite();
		if ((*it)->isDestroyed()) {
			Vector<FiniteTimeAction*> actions;
			actions.pushBack(generateDeathAnimation("explo_small.png", 74, 79));
			tmp = *it;
			actions.pushBack(CallFunc::create([tmp]() { delete tmp; }));
			esprite->runAction(Sequence::create(actions));
			it = ebullets.erase(it);
		}
		else if ((*it)->getWaste() || esprite->getPosition().y >= visibleSize.height ||
			esprite->getPosition().y <= 0 - esprite->getBoundingBox().size.height ||
			esprite->getPosition().x <= 0 - esprite->getBoundingBox().size.width ||
			esprite->getPosition().x >= visibleSize.width) {
			delete *it;
			it = ebullets.erase(it);
		} 
		else it++;
	}
};

void JFGame::honorIncrease(int delta) {
	honor += delta;

	// update record
	reorderScores();

	if (honor >= LEVEL_HARD_THRESHOLD)
		difficulty = ENEMY_DIFFICULTY_HARD;
	else if (honor >= LEVEL_NORMAL_THRESHOLD)
		difficulty = ENEMY_DIFFICULTY_NORMAL;

	refreshHonorDisplay();
}

void JFGame::updateHealthBox() {
	int initHealth;
	switch (currJet) {
	case 0:
		initHealth = JET_INIT_HEALTH;
		break;
	case 1:
		initHealth = JET2_INIT_HEALTH;
		break;
	case 2:
		initHealth = JET3_INIT_HEALTH;
		break;
	case 3:
		initHealth = JET4_INIT_HEALTH;
		break;
	}

	double scale = 0.4f * ((double)mcharacter->getHealth() / (double)initHealth);
	inbox->setScaleX(scale > 0 ? scale : 0);
	if (scale <= 0.16f)
		inbox->setColor(Color3B(144,0,0));
	else inbox->setColor(Color3B(255,255,255));
}

void JFGame::reorderScores() {
	int first = UserDefault::getInstance()->getIntegerForKey("rankFirst");
	int second = UserDefault::getInstance()->getIntegerForKey("rankSecond");
	int third = UserDefault::getInstance()->getIntegerForKey("rankThird");

	int jetLevel = UserDefault::getInstance()->getIntegerForKey("jetLevel", 0);
	switch (jetLevel) {
	case 0:
		if (honor >= JET_LEVEL1_THRESHOLD)
			UserDefault::getInstance()->setIntegerForKey("jetLevel", 1);
		break;
	case 1:
		if (honor >= JET_LEVEL2_THRESHOLD)
			UserDefault::getInstance()->setIntegerForKey("jetLevel", 2);
		break;
	case 2:
		if (honor >= JET_LEVEL3_THRESHOLD)
			UserDefault::getInstance()->setIntegerForKey("jetLevel", 3);
		break;
	case 3:
		break;
	}

	if (rank == 4) {
		if (honor > first) {
			third = second;
			second = first;
			first = honor;
			rank = 1;
		}
		else if (honor > second) {
			third = second;
			second = honor;
			rank = 2;
		}
		else if (honor > third) {
			third = honor;
			rank = 3;
		}
	}
	else {
		switch (rank) {
		case 1:
			first = honor;
			break;
		case 2:
			second = honor;
			if (second > first) {
				int tmp = first;
				first = second;
				second = tmp;
				rank--;
			}
			break;
		case 3:
			third = honor;
			if (third > second) {
				int tmp = second;
				second = third;
				third = tmp;
				rank--;
			}
			break;
		}
	}

	UserDefault::getInstance()->setIntegerForKey("rankFirst", first);
	UserDefault::getInstance()->setIntegerForKey("rankSecond", second);
	UserDefault::getInstance()->setIntegerForKey("rankThird", third);
}

void JFGame::refreshHonorDisplay() {
	std::string display = std::to_string(honor);
	int len = display.length();
	if (len > HONOR_DISPLAY_LENGTH) {
		display = "";
		for (int i = 0;i<HONOR_DISPLAY_LENGTH; i++)
			display += "9";
	}
	else for (int i = 0; i < HONOR_DISPLAY_LENGTH - len; i++)
		display = "0" + display;

	honorDisplay->setString(display);
}

void JFGame::menuResumeCallback(cocos2d::Ref* pSender) {
	isPaused = false;
	pauseMenu->setEnabled(false);
	auto appear = FadeTo::create(0.5f, 0);
	pauseMenu->runAction(appear);
	this->resumeSchedulerAndActions();
}

void JFGame::menuRetryCallback(cocos2d::Ref* pSender) {
	auto newScene = JFGame::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, newScene));
}

void JFGame::menuBackCallback(cocos2d::Ref* pSender) {
	// record scores
	// TODO
	// change scene
	auto newScene = JFMenu::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1, newScene));
}

Animate* JFGame::generateDeathAnimation(std::string filename, int width, int height) {
	frames.clear();

	//frames.pushBack(SpriteFrame::create(FRAME_BOOM_1_NAME, Rect(0, 0, FRAME_BOOM_1_WIDTH, FRAME_BOOM_1_HEIGHT)));
	//frames.pushBack(SpriteFrame::create(FRAME_BOOM_2_NAME, Rect(0, 0, FRAME_BOOM_2_WIDTH, FRAME_BOOM_2_HEIGHT)));
	//frames.pushBack(SpriteFrame::create(FRAME_BOOM_3_NAME, Rect(0, 0, FRAME_BOOM_3_WIDTH, FRAME_BOOM_3_HEIGHT)));
	//frames.pushBack(SpriteFrame::create(FRAME_BOOM_4_NAME, Rect(0, 0, FRAME_BOOM_4_WIDTH, FRAME_BOOM_4_HEIGHT)));
	frames.pushBack(SpriteFrame::create(filename, Rect(0, 0, width, height)));

	auto explosionAnimation = Animation::createWithSpriteFrames(frames, 0.4f);

	return Animate::create(explosionAnimation);
}