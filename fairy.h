#pragma once
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

typedef enum { ATK = 1, HP = 2, DEF = 3, AGL = 4, ENEMY = 5 } advantage;
const float timesSuperior = 1.2; //���Կ��Ʊ���
const float timesLevelUp = 1.1; //�����ӳɱ���
const float timesHP = 100; //������������ֵ����
const float timesCritical = 1.5; //�����˺�����

const string enemyName[] = { "Ģ����","������","��å��" };
const int enemyCnt = 3;

class Fairy
{
public:
	int getExp();
	int getLevel();

	virtual int attackOtr(Fairy* const otr) = 0;
	void addExp(int val);
	int getHurt(float val);
	void getHeal();

	string name;
	string attackName;
	int exp;
	int probMiss, probCritic;
	float nowHP;
	float valueATK, valueDEF, valueHP, valueAGL;
	advantage adv;
};

class FairyATK : public Fairy
{
public:
	virtual int attackOtr(Fairy* const otr) override;
	FairyATK();
};

class FairyDEF : public Fairy
{
public:
	virtual int attackOtr(Fairy* const otr) override;
	FairyDEF();
};

class FairyHP : public Fairy
{
public:
	virtual int attackOtr(Fairy* const otr) override;
	FairyHP();
};

class FairyAGL : public Fairy
{
public:
	virtual int attackOtr(Fairy* const otr) override;
	FairyAGL();
};

class FairyEnemy :public Fairy
{
public:
	virtual int attackOtr(Fairy* const otr) override;
	FairyEnemy();
};