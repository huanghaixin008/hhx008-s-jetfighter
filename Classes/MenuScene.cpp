#include "MenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* JFMenu::createScene()
{
    auto scene = Scene::create();
	auto layer = JFMenu::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool JFMenu::init()
{
    if (!Layer::init())
        return false;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// read config here
	bool toggleMusic = UserDefault::getInstance()->getBoolForKey("toggleMusic", true);
	bool toggleSE = UserDefault::getInstance()->getBoolForKey("toggleSE", true);
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	//audio->preloadBackgroundMusic("The Avengers.mp3");
	//if (toggleMusic)
		//audio->playBackgroundMusic("The Avengers.mp3");

	auto background = Sprite::create(R_BG_FILENAME);
	background->setPosition(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2);
	this->addChild(background, 0);

	author = Label::createWithTTF("HHX008's", "fonts/Marker Felt.ttf", 20);
    label = Label::createWithTTF("Jet Fighter", "fonts/Marker Felt.ttf", 40);
    // position the label on the center of the screen
	author->setPosition(Vec2(origin.x + visibleSize.width / 2,
		visibleSize.height - author->getContentSize().height / 2 * 3));
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
		visibleSize.height - label->getContentSize().height - author->getContentSize().height / 2 * 4));
	this->addChild(author, 1);
    this->addChild(label, 1);

	// jet and its HUD
	jetLevel = UserDefault::getInstance()->getIntegerForKey("jetLevel", 0);
	currJet = UserDefault::getInstance()->getIntegerForKey("currJet", 0);
	std::string srcname;
	if (currJet == 0)
		srcname = R_JET_FILENAME;
	else if (currJet == 1)
		srcname = R_JET2_FILENAME;
	else if (currJet == 2)
		srcname = R_JET3_FILENAME;
	else srcname = R_JET4_FILENAME;
	jet = Sprite::create(srcname);
	jet->setScale(0.6f);
	jet->setPosition(Vec2(origin.x + visibleSize.width / 2,
								jet->getBoundingBox().size.height));
	registerJetMove();
	this->addChild(jet, 1);
	leftArrow = Sprite::create(R_HUD_LEFTARROW);
	leftArrow->setScale(0.4f);
	leftArrow->setAnchorPoint(Vec2(0,0.5));
	leftArrow->setPosition(Vec2(leftArrow ->getBoundingBox().size.width,
											jet->getBoundingBox().size.height));
	this->addChild(leftArrow, 1);
	rightArrow = Sprite::create(R_HUD_RIGHTARROW);
	rightArrow->setScale(0.4f);
	rightArrow->setAnchorPoint(Vec2(1, 0.5));
	rightArrow->setPosition(Vec2(origin.x + visibleSize.width - rightArrow->getBoundingBox().size.width,
											jet->getBoundingBox().size.height));
	this->addChild(rightArrow, 1);

	auto leftArrowListener = EventListenerMouse::create();
	leftArrowListener->onMouseDown = [=](cocos2d::Event* event) {
		auto bound = event->getCurrentTarget()->getBoundingBox();
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		if (!bound.containsPoint(mouseEvent->getLocationInView()))
			return;
		leftArrow->setScale(0.5f);
	};
	leftArrowListener->onMouseUp = [=](cocos2d::Event* event) {
		auto bound = event->getCurrentTarget()->getBoundingBox();
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		if (!bound.containsPoint(mouseEvent->getLocationInView()))
			return;
		leftArrow->setScale(0.4f);
		if (currJet == 1)
			jet->setTexture(R_JET_FILENAME);
		else if (currJet == 2)
			jet->setTexture(R_JET2_FILENAME);
		else if (currJet == 3)
			jet->setTexture(R_JET3_FILENAME);

		if (currJet != 0)
			currJet--;
		UserDefault::getInstance()->setIntegerForKey("currJet", currJet);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(leftArrowListener, leftArrow);

	auto rightArrowListener = EventListenerMouse::create();
	rightArrowListener->onMouseDown = [=](cocos2d::Event* event) {
		auto bound = event->getCurrentTarget()->getBoundingBox();
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		if (!bound.containsPoint(mouseEvent->getLocationInView()))
			return;
		rightArrow->setScale(0.5f);
	};
	rightArrowListener->onMouseUp = [=](cocos2d::Event* event) {
		auto bound = event->getCurrentTarget()->getBoundingBox();
		EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
		if (!bound.containsPoint(mouseEvent->getLocationInView()))
			return;

		bool flag = false;
		rightArrow->setScale(0.4f);
		if (currJet == 0 && jetLevel >= 1) {
			jet->setTexture(R_JET2_FILENAME);
			flag = true;
		}
		else if (currJet == 1 && jetLevel >= 2) {
			jet->setTexture(R_JET3_FILENAME);
			flag = true;
		}
		else if (currJet == 2 && jetLevel >= 3) {
			jet->setTexture(R_JET4_FILENAME);
			flag = true;
		}

		if (flag)
			currJet++;
		UserDefault::getInstance()->setIntegerForKey("currJet", currJet);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(rightArrowListener, rightArrow);

	// main menu
	auto itemStart = MenuItemLabel::create(Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24), 
		CC_CALLBACK_1(JFMenu::menuStartCallback, this));
	auto itemClose = MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFMenu::menuCloseCallback, this));	
	auto itemScore = MenuItemLabel::create(Label::createWithTTF("High Scores", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFMenu::menuHScoreCallback, this));
	auto itemOption = MenuItemLabel::create(Label::createWithTTF("Options", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFMenu::menuOptionsCallback, this));
	menu = Menu::create(itemStart,itemScore,itemOption,itemClose,NULL);
	menu->alignItemsVerticallyWithPadding(20);
	menu->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
	this->addChild(menu, 1);

	// options
	auto itemMusicOn = MenuItemLabel::create(Label::createWithTTF("BGM: ON", "fonts/Marker Felt.ttf", 24));
	auto itemMusicOff = MenuItemLabel::create(Label::createWithTTF("BGM: OFF", "fonts/Marker Felt.ttf", 24));
	auto itemMusicToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(JFMenu::toggleMusicCallBack, this), itemMusicOn, itemMusicOff, NULL);
	itemMusicToggle->setSelectedIndex(toggleMusic ? 0 : 1);
	auto itemSEOn = MenuItemLabel::create(Label::createWithTTF("SE: ON", "fonts/Marker Felt.ttf", 24));
	auto itemSEOff = MenuItemLabel::create(Label::createWithTTF("SE: OFF", "fonts/Marker Felt.ttf", 24));
	auto itemSEToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(JFMenu::toggleSEffectCallBack, this), itemSEOn, itemSEOff, NULL);
	itemSEToggle->setSelectedIndex(toggleSE ? 0 : 1);
	auto itemBack = MenuItemLabel::create(Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFMenu::backCallBack, this));
	options = Menu::create(itemMusicToggle, itemSEToggle, itemBack, NULL);
	options->alignItemsVerticallyWithPadding(20);
	options->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	itemMusicToggle->setCascadeOpacityEnabled(true);
	itemSEToggle->setCascadeOpacityEnabled(true);
	options->setOpacity(0);
	options->setEnabled(false);
	this->addChild(options, 1);

	// high scores
	auto itemFirst = MenuItemLabel::create(Label::createWithTTF("1, " + toDisplayFormat(UserDefault::getInstance()->getIntegerForKey("rankFirst")), "fonts/Marker Felt.ttf", 24));
	auto itemSecond = MenuItemLabel::create(Label::createWithTTF("2, " + toDisplayFormat(UserDefault::getInstance()->getIntegerForKey("rankSecond")), "fonts/Marker Felt.ttf", 24));
	auto itemThird = MenuItemLabel::create(Label::createWithTTF("3, " + toDisplayFormat(UserDefault::getInstance()->getIntegerForKey("rankThird")), "fonts/Marker Felt.ttf", 24));
	auto itemHSBack = MenuItemLabel::create(Label::createWithTTF("Back", "fonts/Marker Felt.ttf", 24),
		CC_CALLBACK_1(JFMenu::backCallBack, this));
	hscore = Menu::create(itemFirst, itemSecond, itemThird, itemHSBack, NULL);
	hscore->alignItemsVerticallyWithPadding(20);
	hscore->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	hscore->setOpacity(0);
	hscore->setEnabled(false);
	this->addChild(hscore, 1);
	
    return true;
}

void JFMenu::menuCloseCallback(Ref* pSender) {
    Director::getInstance()->end();
}

void JFMenu::menuStartCallback(Ref* pSender) {
	startAnimation(Director::getInstance()->getVisibleSize());
	// change scene
	this->scheduleOnce(
		[](float delta)->void {
			auto newScene = JFGame::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(1, newScene));
		}, 1.f, "Transition");
}

void JFMenu::menuOptionsCallback(Ref* pSender) {
	auto fade = FadeTo::create(0.5f, 0);
	menu->runAction(fade);

	this->scheduleOnce(
		[=](float delta)->void {
			menu->setEnabled(false);
			options->setEnabled(true);
			auto appear = FadeTo::create(0.5f, 255);
			options->runAction(appear);
	}, 0.5f, "toOptions");
}

void JFMenu::menuHScoreCallback(Ref* pSender) {
	auto fade = FadeTo::create(0.5f, 0);
	menu->runAction(fade);

	this->scheduleOnce(
		[=](float delta)->void {
		menu->setEnabled(false);
		hscore->setEnabled(true);
		auto appear = FadeTo::create(0.5f, 255);
		hscore->runAction(appear);
	}, 0.5f, "toHScore");
}

void JFMenu::toggleMusicCallBack(Ref* pSender) {
	bool toggleMusic = UserDefault::getInstance()->getBoolForKey("toggleMusic", true);
	UserDefault::getInstance()->setBoolForKey("toggleMusic", !toggleMusic);
	//if (!toggleMusic)
		//CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	//else
		//CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void JFMenu::toggleSEffectCallBack(Ref* pSender) {
	bool toggleSE = UserDefault::getInstance()->getBoolForKey("toggleSE", true);
	UserDefault::getInstance()->setBoolForKey("toggleSE", !toggleSE);
	//if (!toggleSE)
		//CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
	//else
		//CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void JFMenu::backCallBack(Ref* pSender) {
	auto backer = (Menu *)((MenuItemLabel *)pSender)->getParent();
	auto fade = FadeTo::create(0.5f, 0);
	backer->runAction(fade);

	this->scheduleOnce(
		[=](float delta)->void {
			backer->setEnabled(false);
			menu->setEnabled(true);
			auto appear = FadeTo::create(0.5f, 255);
			menu->runAction(appear);
	}, 0.5f, "toMenu");
}

void JFMenu::registerJetMove() {
	cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
	auto moveup = MoveBy::create(0.5f, Vec2(0, 3));
	auto movedown = MoveBy::create(0.5f, Vec2(0, -3));
	actions.pushBack(moveup);
	actions.pushBack(movedown);
	actions.pushBack(movedown);
	actions.pushBack(moveup);
	auto sequence = Sequence::create(actions);
	auto repeatMove = RepeatForever::create(sequence);
	jet->runAction(repeatMove);
}

void JFMenu::startAnimation(Size visibleSize) {
	author->runAction(FadeTo::create(0.6f, 0));
	label->runAction(FadeTo::create(0.6f, 0));
	menu->runAction(FadeTo::create(0.6f, 0));
	leftArrow->runAction(FadeTo::create(0.6f, 0));
	rightArrow->runAction(FadeTo::create(0.6f, 0));

	auto jetPos = jet->getPosition();
	auto jetMoveTo = MoveTo::create(1, Vec2(jetPos.x,
		visibleSize.height + jet->getContentSize().height));
	jet->runAction(EaseExponentialIn::create(jetMoveTo));
}

std::string JFMenu::toDisplayFormat(int output) {
	std::string display = std::to_string(output);
	int len = display.length();
	if (len > HONOR_DISPLAY_LENGTH) {
		display = "";
		for (int i = 0; i<HONOR_DISPLAY_LENGTH; i++)
			display += "9";
	}
	else for (int i = 0; i < HONOR_DISPLAY_LENGTH - len; i++)
		display = "0" + display;

	return display;
}
