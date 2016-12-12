# include "Units.h"
/*
virtual void moveBy(float delta) = 0;
virtual void act(float delta) {};
*/

using namespace cocos2d;

bool UnitSprite::isCollidedWith(UnitSprite* unit) {
	Rect selfRect = sprite->getBoundingBox();
	Rect unitRect = unit->getSprite()->getBoundingBox();

	if (selfRect.intersectsRect(unitRect))
		return true;
	else return false;
};

void BlueJet::moveBy(float delta, DIREC direct) {
	Vec2 pos = getSprite()->getPosition();

	if ((direct & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direct & 0x4) >> 2) pos.y -= (SPEED_BASE / 3 * 2) * getSpeed() * delta; // down
	if ((direct & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direct & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	// display shadows when speed up
	double initSpeed;

	if (getSpeed() > JET_INIT_SPEED && direct != DIREC_NONE) {
		if (counterCheck(JET_SHADOW_TAG)) {
			auto shadow = Sprite::create(getSrcname());
			shadow->setScale(0.4f);
			shadow->setAnchorPoint(Vec2(0, 0));
			shadow->setPosition(getSprite()->getPosition());
			Vector<FiniteTimeAction*> actions;
			auto parent = getSprite()->getParent();
			actions.pushBack(FadeTo::create(0.5f, 0));
			actions.pushBack(CallFunc::create([=]() { parent->removeChild(shadow, true); }));
			shadow->runAction(Sequence::create(actions));
			getSprite()->getParent()->addChild(shadow, 0);
		}
	}

	getSprite()->setPosition(pos);
};
void StrongJet::moveBy(float delta, DIREC direct) {
	Vec2 pos = getSprite()->getPosition();

	if ((direct & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direct & 0x4) >> 2) pos.y -= (SPEED_BASE / 3 * 2) * getSpeed() * delta; // down
	if ((direct & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direct & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	// display shadows when speed up
	double initSpeed;

	if (getSpeed() > JET2_INIT_SPEED && direct != DIREC_NONE) {
		if (counterCheck(JET_SHADOW_TAG)) {
			auto shadow = Sprite::create(getSrcname());
			shadow->setScale(0.4f);
			shadow->setAnchorPoint(Vec2(0, 0));
			shadow->setPosition(getSprite()->getPosition());
			Vector<FiniteTimeAction*> actions;
			auto parent = getSprite()->getParent();
			actions.pushBack(FadeTo::create(0.5f, 0));
			actions.pushBack(CallFunc::create([=]() { parent->removeChild(shadow, true); }));
			shadow->runAction(Sequence::create(actions));
			getSprite()->getParent()->addChild(shadow, 0);
		}
	}

	getSprite()->setPosition(pos);
};
void SmallJet::moveBy(float delta, DIREC direct) {
	Vec2 pos = getSprite()->getPosition();

	if ((direct & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direct & 0x4) >> 2) pos.y -= (SPEED_BASE / 3 * 2) * getSpeed() * delta; // down
	if ((direct & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direct & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	// display shadows when speed up
	double initSpeed;

	if (getSpeed() > JET3_INIT_SPEED && direct != DIREC_NONE) {
		if (counterCheck(JET_SHADOW_TAG)) {
			auto shadow = Sprite::create(getSrcname());
			shadow->setScale(0.4f);
			shadow->setAnchorPoint(Vec2(0, 0));
			shadow->setPosition(getSprite()->getPosition());
			Vector<FiniteTimeAction*> actions;
			auto parent = getSprite()->getParent();
			actions.pushBack(FadeTo::create(0.5f, 0));
			actions.pushBack(CallFunc::create([=]() { parent->removeChild(shadow, true); }));
			shadow->runAction(Sequence::create(actions));
			getSprite()->getParent()->addChild(shadow, 0);
		}
	}

	getSprite()->setPosition(pos);
};
void MagicJet::moveBy(float delta, DIREC direct) {
	Vec2 pos = getSprite()->getPosition();

	if ((direct & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direct & 0x4) >> 2) pos.y -= (SPEED_BASE / 3 * 2) * getSpeed() * delta; // down
	if ((direct & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direct & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	// display shadows when speed up
	double initSpeed;

	if (getSpeed() > JET4_INIT_SPEED && direct != DIREC_NONE) {
		if (counterCheck(JET_SHADOW_TAG)) {
			auto shadow = Sprite::create(getSrcname());
			shadow->setScale(0.4f);
			shadow->setAnchorPoint(Vec2(0, 0));
			shadow->setPosition(getSprite()->getPosition());
			Vector<FiniteTimeAction*> actions;
			auto parent = getSprite()->getParent();
			actions.pushBack(FadeTo::create(0.5f, 0));
			actions.pushBack(CallFunc::create([=]() { parent->removeChild(shadow, true); }));
			shadow->runAction(Sequence::create(actions));
			getSprite()->getParent()->addChild(shadow, 0);
		}
	}

	getSprite()->setPosition(pos);	
	if (mjBullet) {
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - mjBullet->getSprite()->getBoundingBox().size.width) / 2;
		mjBullet->getSprite()->setPosition(pos);
	}
};

std::vector<UnitSprite*> BlueJet::shoot(float delta) {
	// jet always shoot upwards
	std::vector<UnitSprite*> ret;
	if (!counterCheck(JET_SHOOT_TAG))
		return ret;
	if (getUltimateStatus())
		return ret;

	int shootPower = getShootPower();
	Bullet* bullet = new Bullet(R_JBULLET_FILENAME, shootPower, DIREC_UP);
	bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
	getSprite()->getParent()->addChild(bullet->getSprite(), 1);

	Vec2 pos = getSprite()->getPosition();
	pos.y += getSprite()->getBoundingBox().size.height;
	pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
	bullet->getSprite()->setPosition(pos);

	ret.push_back(bullet);

	if (shootPower >= 2) {
		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower - BULLET_POWER_LV1, DIREC_UPLEFT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV2) * 0.2f);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += getSprite()->getBoundingBox().size.width;
		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower - BULLET_POWER_LV1, DIREC_UPRIGHT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV2) * 0.2f);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}

	if (shootPower >= 3) {
		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height / 2;
		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower - BULLET_POWER_LV2, DIREC_LEFT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV3) * 0.2f);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height / 2;
		pos.x += getSprite()->getBoundingBox().size.width;
		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower - BULLET_POWER_LV2, DIREC_RIGHT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV3) * 0.2f);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}

	return ret;
}
std::vector<UnitSprite*> StrongJet::shoot(float delta) {
	std::vector<UnitSprite*> ret;
	if (!counterCheck(JET_SHOOT_TAG))
		return ret;

	int shootPower = getShootPower();
	Bullet* bullet = new Bullet(R_JBULLET_FILENAME, shootPower, DIREC_UP);
	bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
	getSprite()->getParent()->addChild(bullet->getSprite(), 1);

	Vec2 pos = getSprite()->getPosition();
	pos.y += getSprite()->getBoundingBox().size.height;
	bullet->getSprite()->setPosition(pos);

	ret.push_back(bullet);

	bullet = new Bullet(R_JBULLET_FILENAME, shootPower, DIREC_UP);
	bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
	getSprite()->getParent()->addChild(bullet->getSprite(), 1);

	pos = getSprite()->getPosition();
	pos.y += getSprite()->getBoundingBox().size.height;
	pos.x += getSprite()->getBoundingBox().size.width * 3 / 4 - bullet->getSprite()->getBoundingBox().size.width / 4;
	bullet->getSprite()->setPosition(pos);

	ret.push_back(bullet);

	return ret;
}
std::vector<UnitSprite*> SmallJet::shoot(float delta) {
	std::vector<UnitSprite*> ret;
	if (!counterCheck(JET_SHOOT_TAG))
		return ret;

	if (getUltimateStatus()) {
		int shootPower = getShootPower();
		Bullet* bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_UP);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		Vec2 pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_DOWN);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_DOWNLEFT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_DOWNRIGHT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_UPLEFT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_UPRIGHT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_LEFT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, DIREC_RIGHT);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}
	else {
		DIREC direc;
		switch (shootDirect) {
		case 0:
			direc = DIREC_UPLEFT;
			break;
		case 1:
			direc = DIREC_UP;
			break;
		case 2:
			direc = DIREC_UPRIGHT;
			break;
		}
		int shootPower = getShootPower();
		Bullet* bullet = new Bullet(R_JBULLET_SMALL_FILENAME, shootPower, direc);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		Vec2 pos = getSprite()->getPosition();
		pos.y += getSprite()->getBoundingBox().size.height;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		if (shootPower >= 2) {
			switch (shootDirect) {
			case 0:
				direc = DIREC_DOWNRIGHT;
				break;
			case 1:
				direc = DIREC_DOWN;
				break;
			case 2:
				direc = DIREC_DOWNLEFT;
				break;
			}
			pos = getSprite()->getPosition();
			pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
			bullet = new Bullet(R_JBULLET_SMALL_FILENAME, BULLET_POWER_LV1, direc);
			getSprite()->getParent()->addChild(bullet->getSprite(), 1);
			bullet->getSprite()->setPosition(pos);
			ret.push_back(bullet);
		}

		shootDirect = (shootDirect + 1) % 3;
	}
	return ret;
}
std::vector<UnitSprite*> MagicJet::shoot(float delta) {
	std::vector<UnitSprite*> ret;
	if (!counterCheck(JET_SHOOT_TAG))
		return ret;

	int shootPower = getShootPower();
	Bullet* bullet = new Bullet(R_MJBULLET_FILENAME, BULLET_POWER_LV1, DIREC_UP);
	bullet->getSprite()->setScaleY(0.4f + (shootPower - BULLET_POWER_LV1) * 0.1f);
	bullet->getSprite()->setScaleX(0.4f + (shootPower - BULLET_POWER_LV1) * 0.1f);
	bullet->setSpeed(0);
	bullet->setHealth(INT_MAX);

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(ScaleTo::create(1.2f, 0, 0.4f + (shootPower - BULLET_POWER_LV1) * 0.1f));
	actions.pushBack(CallFunc::create([=]() { 
		bullet->setWaste(true); }));
	bullet->getSprite()->runAction(Sequence::create(actions));
	getSprite()->getParent()->addChild(bullet->getSprite(), 1);

	Vec2 pos = getSprite()->getPosition();
	pos.y += getSprite()->getBoundingBox().size.height;
	pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
	bullet->getSprite()->setPosition(pos);

	mjBullet = bullet;
	ret.push_back(bullet);

	return ret;
}

void BlueJet::ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	ultiCountDown();
	setUltimateStatus(true);
	counterAdd(JET_ULTIMATE_TAG, JET_ULTIMATE_DURATION);
	Bullet* bomb = new Bullet(R_BOMB_FILENAME, BULLET_POWER_LV3, DIREC_UP);
	bomb->setSpeed(BULLET_INIT_SPEED / 2);
	bomb->setHealth(INT_MAX);
	bomb->getSprite()->setScale(0.8f);
	getSprite()->getParent()->addChild(bomb->getSprite(), 1);
	getSprite()->getParent()->scheduleOnce([=](float delta) { bomb->takeDamage(INT_MAX); }, JET_ULTIMATE_DURATION, "boom");

	Vec2 pos = getSprite()->getPosition();
	pos.y += getSprite()->getBoundingBox().size.height;
	pos.x += (getSprite()->getBoundingBox().size.width - bomb->getSprite()->getBoundingBox().size.width) / 2;
	bomb->getSprite()->setPosition(pos);

	jbullets->push_back(bomb);
}
void BlueJet::ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	setUltimateStatus(false);
	counterRemove(JET_ULTIMATE_TAG);

	auto bomb = (*jbullets)[jbullets->size() - 1];
	Vec2 bombpos =bomb->getSprite()->getPosition();
	bombpos.y += bomb->getSprite()->getBoundingBox().size.height / 2;
	bombpos.x += bomb->getSprite()->getBoundingBox().size.width / 2;
	auto wave = Sprite::create(R_BOMBWAVE_FILENAME);
	wave->setPosition(bombpos);
	auto parent = getSprite()->getParent();
	parent->addChild(wave);

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(ScaleTo::create(0.5f, 20.0f));
	actions.pushBack(FadeTo::create(0.5f, 0));
	Vector<FiniteTimeAction*> actions2;
	actions2.pushBack(Spawn::create(actions));
	actions2.pushBack(CallFunc::create([=]()->void { parent->removeChild(wave); }));
	wave->runAction(Sequence::create(actions2));
	
	for (auto em : *enemies) {
		em->takeDamage(INT_MAX);
	}
	for (auto ebl : *ebullets) {
		ebl->takeDamage(INT_MAX);
	}
}
void StrongJet::ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	ultiCountDown();
	setUltimateStatus(true);
	counterAdd(JET_ULTIMATE_TAG, JET2_ULTIMATE_DURATION);

	getSprite()->setScale(0.6f);
	Vector<FiniteTimeAction*> actions;
	actions.pushBack(TintBy::create(0.5f, 0, 255, 0));
	actions.pushBack(TintBy::create(0.5f, 0, 0, 0));
	getSprite()->runAction(Repeat::create(Sequence::create(actions), JET2_ULTIMATE_DURATION / 1.0f));
}
void StrongJet::ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	setUltimateStatus(false);
	counterRemove(JET_ULTIMATE_TAG);

	getSprite()->setScale(0.4f);
}
void SmallJet::ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	ultiCountDown();
	setUltimateStatus(true);
	counterAdd(JET_ULTIMATE_TAG, JET3_ULTIMATE_DURATION);
}
void SmallJet::ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	setUltimateStatus(false);
	counterRemove(JET_ULTIMATE_TAG);
}
void MagicJet::ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	ultiCountDown();
	setUltimateStatus(true);
	counterAdd(JET_ULTIMATE_TAG, JET4_ULTIMATE_DURATION);

	Bullet* bullet = new Bullet(R_MJBULLET_FILENAME, BULLET_POWER_LV3, DIREC_UP, COLLISIONTYPE_ALL);
	bullet->getSprite()->setRotation(90);
	bullet->getSprite()->setScaleY(1.2f);
	bullet->getSprite()->setScaleX(0.8f);
	bullet->setHealth(INT_MAX);

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(ScaleTo::create(0.5f, 0.3f, 1.2f));
	actions.pushBack(ScaleTo::create(0.5f, 0.8f, 1.2f));
	bullet->getSprite()->runAction(RepeatForever::create(Sequence::create(actions)));
	getSprite()->getParent()->addChild(bullet->getSprite(), 1);

	Vec2 pos = Vec2(0,0);
	//pos.y -= bullet->getSprite()->getBoundingBox().size.height;
	pos.x -= bullet->getSprite()->getBoundingBox().size.width / 5;
	bullet->getSprite()->setPosition(pos);

	jbullets->push_back(bullet);
}
void MagicJet::ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
	setUltimateStatus(false);
	counterRemove(JET_ULTIMATE_TAG);
}

DIREC Enemy::calNextDirection(float delta) {
	switch (strategy) {
	case ENEMY_DIFFICULTY_NORMAL: // go zigzag
		movementThreshold -= delta;
		if (movementThreshold < 0) {
			movementThreshold = rand() % strategy + 1;
			if (direc == DIREC_DOWN)
				direc = DIREC_NONE;
			else direc = DIREC_DOWN;
		}
		break;
	case ENEMY_DIFFICULTY_HARD:  // might stop
		movementThreshold -= delta;
		if (movementThreshold < 0) {
			movementThreshold = rand() % strategy + 1;
			if (direc == DIREC_DOWNLEFT)
				direc = DIREC_DOWNRIGHT;
			else direc = DIREC_DOWNLEFT;
		}
	case ENEMY_DIFFICULTY_SPECIAL:
	case ENEMY_DIFFICULTY_EASY:
		break;
	}

	return direc;
};

void Enemy::moveBy(float delta, DIREC direct) {
	Vec2 pos = getSprite()->getPosition();
	DIREC direc = calNextDirection(delta);
	if (direct != 0x0)
		direc = direct;

	//enemy always go down
	//if ((direct & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direc & 0x4) >> 2) pos.y -= SPEED_BASE * getSpeed() * delta; // down
	if ((direc & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direc & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	getSprite()->setPosition(pos);
};

std::vector<UnitSprite*> Enemy::shoot(float delta) {
	// enemy always shoot downwards
	std::vector<UnitSprite*> ret;
	if (!counterCheck(ENEMY_SHOOT_TAG))
		return ret;

	int likelyhood;
	switch (strategy) {
	default:
	case ENEMY_DIFFICULTY_EASY:
		likelyhood = ENEMY_SHOOT_LIKELYHOOD_EASY;
		break;
	case ENEMY_DIFFICULTY_NORMAL:
		likelyhood = ENEMY_SHOOT_LIKELYHOOD_NORMAL;
		break;
	case ENEMY_DIFFICULTY_HARD:
		likelyhood = ENEMY_SHOOT_LIKELYHOOD_HARD;
		break;
	}

	if (random() % 10 >= likelyhood)  // random shoot
		return ret;

	Bullet* bullet;
	Vec2 pos;
	if (strategy != ENEMY_DIFFICULTY_HARD) {
		bullet = new Bullet(R_EBULLET_FILENAME, shootPower, DIREC_DOWN);
		bullet->getSprite()->setScale(0.4f + (shootPower - BULLET_POWER_LV1) * 0.2f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y -= (bullet->getSprite()->getBoundingBox().size.height / 5) * 2;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}

	if (strategy == ENEMY_DIFFICULTY_NORMAL) {
		bullet = new Bullet(R_EBULLET_FILENAME, BULLET_POWER_LV1, DIREC_DOWNLEFT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);

		bullet = new Bullet(R_EBULLET_FILENAME, BULLET_POWER_LV1, DIREC_DOWNRIGHT);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}

	if (strategy == ENEMY_DIFFICULTY_HARD) {
		bullet = new Bullet(R_EHBULLET_FILENAME, shootPower, DIREC_DOWN, COLLISIONTYPE_ALL);
		bullet->setSpeed(0.1f);
		bullet->setHealth(6*ENEMY_INIT_HEALTH_BASE);
		bullet->getSprite()->setScale(0.4f);
		getSprite()->getParent()->addChild(bullet->getSprite(), 1);

		pos = getSprite()->getPosition();
		pos.y -= (bullet->getSprite()->getBoundingBox().size.height / 5) * 2;
		pos.x += (getSprite()->getBoundingBox().size.width - bullet->getSprite()->getBoundingBox().size.width) / 2;
		bullet->getSprite()->setPosition(pos);
		ret.push_back(bullet);
	}

	return ret;
}

void Bullet::moveBy(float delta, DIREC direct = 0x0) {
	// bullet dont care input direction
	// once it's shot, it will go that direction forever
	Vec2 pos = getSprite()->getPosition();

	if (direct == 0x0)
		direct = direc;
	if ((direc & 0x8) >> 3) pos.y += SPEED_BASE * getSpeed() * delta; // up
	if ((direc & 0x4) >> 2) pos.y -= SPEED_BASE * getSpeed() * delta; // down
	if ((direc & 0x2) >> 1) pos.x -= SPEED_BASE * getSpeed() * delta; // left
	if (direc & 0x1) pos.x += SPEED_BASE * getSpeed() * delta; // right

	getSprite()->setPosition(pos);
};