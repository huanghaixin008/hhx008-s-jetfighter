#ifndef __UNITS_H__
#define __UNITS_H__

#include "cocos2d.h"
#include "Constants.h"
#include <stdlib.h>

class UnitSprite {
private:
	cocos2d::Sprite* sprite;
	float health;
	float speed;
	float power;
	bool waste;
	std::unordered_map<std::string, float> counter;
	std::unordered_map<std::string, float> counterInit;

public:
	UnitSprite(std::string& filename){
		waste = false;
		sprite = cocos2d::Sprite::create(filename);
		sprite->setScale(0.4f);
		sprite->setAnchorPoint(cocos2d::Vec2(0, 0));
		// sprite->autorelease();
	};
	~UnitSprite() {
		sprite->getParent()->removeChild(sprite, true);
		// sprite->release();
	};

	cocos2d::Sprite* getSprite() {
		return sprite;
	};
	bool counterAdd(std::string tag, float duration) {
		if (counter.find(tag) != counter.end())
			return false;
		counter[tag] = duration;
		counterInit[tag] = duration;
		return true;
	};
	bool counterRemove(std::string tag) {
		if (counter.find(tag) == counter.end())
			return false;
		counter.erase(tag);
		counterInit.erase(tag);
		return true;
	}
	void counterReset(std::string tag) {
		counter[tag] = counterInit[tag];
	}
	void counterCount(float delta) { 
		for (auto it = counter.begin(); it != counter.end(); it++)
			it->second -= delta;
	};
	bool counterCheck(std::string tag) {
		if (counter.find(tag) == counter.end())
			return false;
		if (counter[tag] <= 0) {
			counter[tag] = counterInit[tag];
			return true;
		}
		else return false;
	};
	
	bool getWaste() { return waste; }
	void setWaste(bool status) { waste = status;  }

	void setHealth(float h) { health = h; };
	void setSpeed(float s) { speed = s; };
	void setPower(float p) { power = p; };
	float getHealth() { return health; };
	float getSpeed() { return speed; };
	int getPower() { return power; };

	int countDamege() { return (speed + power) * POWER_BASE; };
	virtual float takeDamage(float damage) { return health -= damage; };
	bool isDestroyed() { return health <= 0; };
	bool isCollidedWith(UnitSprite* unit);

	virtual void moveBy(float delta, DIREC direct) = 0;
	virtual std::vector<UnitSprite*> shoot(float delta) { return std::vector<UnitSprite*>(); };
};

// bullets shot
class Bullet : public UnitSprite {
private:
	DIREC direc;
	int collisionType;
public:
	Bullet(std::string& filename, int power, DIREC d, int ct = COLLISIONTYPE_AIRCRAFT) : UnitSprite(filename) {
		collisionType = ct;
		setSpeed(BULLET_INIT_SPEED);
		setPower(power * BULLET_INIT_POWER);
		setHealth(BULLET_INIT_HEALTH);
		direc = d;
	};

	DIREC getDirec() { return direc; };
	void setCollisionType(int ct) { collisionType = ct;  };
	int getCollisionType() { return collisionType;  };
	void moveBy(float delta, DIREC direct);
};

// main character's jet
class Jet : public UnitSprite {
private:
	int honor;
	int ultiCount;
	int shootPower;
	bool inUltimate;
	std::string srcname;

public:
	Jet(std::string& filename): UnitSprite(filename) {
		honor = 0; 
		ultiCount = 0;
		srcname = filename;
		inUltimate = false;
		counterAdd(JET_SHOOT_TAG, JET_SHOOT_DURATION);
		counterAdd(JET_SHADOW_TAG, JET_SHADOW_DURATION);
	};

	std::string getSrcname() {
		return srcname;
	}

	int getHonor(int delta){
		return honor += delta;
	}

	int getUltiCount() {
		return ultiCount;
	}

	bool ultiCountUp() {
		if (ultiCount < 3) {
			ultiCount++;
			return true;
		}
		else return false;
	}

	void ultiCountDown() {
		ultiCount--;
	}

	int getShootPower() {
		return shootPower;
	}

	void setShootPower(int sp) {
		shootPower = sp;
	}

	bool getUltimateStatus() {
		return inUltimate;
	}

	void setUltimateStatus(bool status) {
		inUltimate = status;
	}

	void powerUp() {
		shootPower++;
		std::string tag = JET_POWER_TRAIT_TAG;
		if (!counterAdd(JET_POWER_TRAIT_TAG, JET_POWER_TRAIT_DURATION))
			counterReset(JET_POWER_TRAIT_TAG);
	}
	virtual void powerDown() = 0;
	void speedUp() {
		setSpeed(getSpeed() + 0.3f);
		if (!counterAdd(JET_SPEED_TRAIT_TAG, JET_SPEED_TRAIT_DURATION))
			counterReset(JET_SPEED_TRAIT_TAG);
	}
	virtual void speedDown() = 0;
	void checkTraitDuration(float delta) {
		if (counterCheck(JET_SPEED_TRAIT_TAG)) {
			speedDown();
			counterRemove(JET_SPEED_TRAIT_TAG);
		}

		if (counterCheck(JET_POWER_TRAIT_TAG)) {
			powerDown();
			counterRemove(JET_POWER_TRAIT_TAG);
		}
	};

	virtual void ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) = 0;
	virtual void ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) = 0;
	void checkUltimateDuration(float delta, std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets) {
		if (counterCheck(JET_ULTIMATE_TAG))
			ultimateOff(enemies, ebullets, jbullets);
	};

	virtual void restore() = 0;
	virtual void toNormalTexture() = 0;
	virtual void toFlyLeftTexture() = 0;
	virtual void toFlyRightTexture() = 0;

	virtual void moveBy(float delta, DIREC direct) = 0;
	virtual std::vector<UnitSprite*> shoot(float delta) = 0;
};

class BlueJet : public Jet {
public:
	BlueJet(std::string& filename=R_JET_FILENAME) : Jet(filename) {
		setShootPower(BULLET_POWER_LV1);
		setPower(JET_INIT_POWER);
		setSpeed(JET_INIT_SPEED);
		setHealth(JET_INIT_HEALTH);
	}

	void powerDown() {
		setShootPower(BULLET_POWER_LV1);
	}

	void speedDown() {
		setSpeed(JET_INIT_SPEED);
	}

	void restore() {
		setHealth(JET_INIT_HEALTH);
	}

	void toNormalTexture() {
		getSprite()->setTexture(getSrcname());
	}
	void toFlyLeftTexture() {
		getSprite()->setTexture(R_JET_LEFT_FILENAME);
	}
	void toFlyRightTexture() {
		getSprite()->setTexture(R_JET_RIGHT_FILENAME);
	}

	void ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void moveBy(float delta, DIREC direct);
	std::vector<UnitSprite*> shoot(float delta);
};

class StrongJet : public Jet {
public:
	StrongJet(std::string& filename = R_JET2_FILENAME) : Jet(filename) {
		setShootPower(BULLET_POWER_LV2);
		setPower(JET2_INIT_POWER);
		setSpeed(JET2_INIT_SPEED);
		setHealth(JET2_INIT_HEALTH);
		counterRemove(JET_SHOOT_TAG);
		counterAdd(JET_SHOOT_TAG, JET2_SHOOT_DURATION);
	}

	float takeDamage(float damage) {
		if (!getUltimateStatus()) {
			setHealth(getHealth() - damage);
			return getHealth();
		}
		else return getHealth();
	}

	void powerDown() {
		setShootPower(BULLET_POWER_LV3);
	}

	void speedDown() {
		setSpeed(JET2_INIT_SPEED);
	}

	void restore() {
		setHealth(JET2_INIT_HEALTH);
	}

	void toNormalTexture() {
		getSprite()->setTexture(getSrcname());
	}
	void toFlyLeftTexture() {
		getSprite()->setTexture(R_JET2_LEFT_FILENAME);
	}
	void toFlyRightTexture() {
		getSprite()->setTexture(R_JET2_RIGHT_FILENAME);
	}

	void ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void moveBy(float delta, DIREC direct);
	std::vector<UnitSprite*> shoot(float delta);
};

class SmallJet : public Jet {
private:
	int shootDirect = 0;
public:
	SmallJet(std::string& filename = R_JET3_FILENAME) : Jet(filename) {
		setShootPower(BULLET_POWER_LV1);
		setPower(JET3_INIT_POWER);
		setSpeed(JET3_INIT_SPEED);
		setHealth(JET3_INIT_HEALTH);
		counterRemove(JET_SHOOT_TAG);
		counterAdd(JET_SHOOT_TAG, JET3_SHOOT_DURATION);
	}

	void powerDown() {
		setShootPower(BULLET_POWER_LV1);
	}

	void speedDown() {
		setSpeed(JET3_INIT_SPEED);
	}

	void restore() {
		setHealth(JET3_INIT_HEALTH);
	}

	void toNormalTexture() {
		getSprite()->setTexture(getSrcname());
	}
	void toFlyLeftTexture() {
		getSprite()->setTexture(R_JET3_LEFT_FILENAME);
	}
	void toFlyRightTexture() {
		getSprite()->setTexture(R_JET3_RIGHT_FILENAME);
	}

	void ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void moveBy(float delta, DIREC direct);
	std::vector<UnitSprite*> shoot(float delta);
};

class MagicJet : public Jet {
private:
	Bullet* mjBullet;
public:
	MagicJet(std::string& filename = R_JET4_FILENAME) : Jet(filename) {
		mjBullet = NULL;
		setShootPower(BULLET_POWER_LV2);
		setPower(JET4_INIT_POWER);
		setSpeed(JET4_INIT_SPEED);
		setHealth(JET4_INIT_HEALTH);
		counterRemove(JET_SHOOT_TAG);
		counterAdd(JET_SHOOT_TAG, JET4_SHOOT_DURATION);
	}

	void powerDown() {
		setShootPower(BULLET_POWER_LV2);
	}

	void speedDown() {
		setSpeed(JET4_INIT_SPEED);
	}

	void restore() {
		setHealth(JET4_INIT_HEALTH);
	}

	void toNormalTexture() {
		getSprite()->setTexture(getSrcname());
	}
	void toFlyLeftTexture() {
		getSprite()->setTexture(R_JET4_LEFT_FILENAME);
	}
	void toFlyRightTexture() {
		getSprite()->setTexture(R_JET4_RIGHT_FILENAME);
	}

	void ultimateOn(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void ultimateOff(std::vector<UnitSprite*>* enemies, std::vector<UnitSprite*>* ebullets, std::vector<UnitSprite*>* jbullets);
	void moveBy(float delta, DIREC direct);
	std::vector<UnitSprite*> shoot(float delta);
};

// enemies
class Enemy : public UnitSprite {
private:
	int strategy;
	int shootPower;
	float movementThreshold;
	DIREC direc;

public:
	Enemy(std::string& filename, int difficulty) : UnitSprite(filename) {
		this->strategy = difficulty;
		this->movementThreshold = (float)difficulty / 2.0;
		counterAdd(ENEMY_SHOOT_TAG, ENEMY_SHOOT_DURATION);
		if (difficulty == ENEMY_DIFFICULTY_HARD)
			if (rand() % 2)
				direc = DIREC_DOWNLEFT;
			else direc = DIREC_DOWNRIGHT;
		else direc = DIREC_DOWN;

		switch (strategy) {
		case ENEMY_DIFFICULTY_EASY: 
			this->shootPower = BULLET_POWER_LV1;
			setSpeed(ENEMY_INIT_SPEED_BASE);
			setPower(ENEMY_INIT_POWER_BASE);
			setHealth(ENEMY_INIT_HEALTH_BASE);
			break;
		case ENEMY_DIFFICULTY_NORMAL: 
			this->shootPower = BULLET_POWER_LV2;
			setSpeed(difficulty  * ENEMY_INIT_SPEED_BASE);
			setPower((difficulty + 1) * ENEMY_INIT_POWER_BASE);
			setHealth((difficulty + 1) * ENEMY_INIT_HEALTH_BASE);
			break;
		case ENEMY_DIFFICULTY_HARD: 
			this->shootPower = BULLET_POWER_LV1;
			setSpeed(difficulty * ENEMY_INIT_SPEED_BASE);
			setPower(ENEMY_INIT_POWER_BASE);
			setHealth(difficulty * ENEMY_INIT_HEALTH_BASE);
			break;
		case ENEMY_DIFFICULTY_SPECIAL:
			this->shootPower = BULLET_POWER_LV3;
			setSpeed(ENEMY_INIT_SPEED_BASE);
			setPower(2 * difficulty * ENEMY_INIT_POWER_BASE);
			setHealth(2 * difficulty * ENEMY_INIT_HEALTH_BASE);

			if (filename == R_SENEMY_BOMB_FILENAME)
				strategy = ENEMY_DIFFICULTY_SPECIAL_BOMB;
			else if (filename == R_SENEMY_HEALTH_FILENAME)
				strategy = ENEMY_DIFFICULTY_SPECIAL_HEALTH;
			else if (filename == R_SENEMY_POWER_FILENAME)
				strategy = ENEMY_DIFFICULTY_SPECIAL_POWER;
			else if (filename == R_SENEMY_SPEED_FILENAME)
				strategy = ENEMY_DIFFICULTY_SPECIAL_SPEED;

			break;
		}
	}

	int countHonor() { return HONOR_BASE * (getSpeed() + getPower() + strategy); };
	int getStrategy() { return strategy;  };

	DIREC calNextDirection(float);

	void moveBy(float delta, DIREC direct);
	std::vector<UnitSprite*> shoot(float delta);
};

#endif