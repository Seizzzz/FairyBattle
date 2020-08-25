#pragma once
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

typedef enum { ATK = 1, HP = 2, DEF = 3, AGL = 4, ENEMY = 5 } advantage;
const double timesSuperior = 1.2; //属性克制倍率
const double timesLevelUp = 1.1; //升级加成倍率
const float timesHP = 100; //生命力与生命值倍数

const string enemyName[] = { "蘑菇怪","钢牙鲨","流氓兔" };
const int enemyCnt = 3;

class Fairy
{
public:
	int getExp();
	int getLevel();

	virtual void attackOtr(Fairy* otr) = 0;
	void addExp(int val);
	bool getHurt(float val);

	string name;
	string attackName;
	int exp;
	float nowHP;
	float valueATK, valueDEF, valueHP, valueAGL;
	advantage adv;
};

class FairyATK : public Fairy
{
public:
	virtual void attackOtr(Fairy* otr) override;
	FairyATK();
};

class FairyDEF : public Fairy
{
public:
	virtual void attackOtr(Fairy* otr) override;
	FairyDEF();
};

class FairyHP : public Fairy
{
public:
	virtual void attackOtr(Fairy* otr) override;
	FairyHP();
};

class FairyAGL : public Fairy
{
public:
	virtual void attackOtr(Fairy* otr) override;
	FairyAGL();
};

class FairyEnemy :public Fairy
{
public:
	virtual void attackOtr(Fairy* otr) override;
	FairyEnemy();
};