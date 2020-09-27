#pragma once
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

typedef enum { ATK = 1, HP = 2, DEF = 3, AGL = 4, ENEMY = 5 } advantage;
const float timesSuperior = 1.2; //���Կ��Ʊ���
const float timesAttack = 10; //�����빥��������
const float timesDefend = 10; //���������������
const float timesLevelUp = 1.1; //�����ӳɱ���
const float timesLevelUpMain = 1.05; //���������Զ��ⱶ��
const float timesHP = 100; //������������ֵ����
const float timesCritical = 1.5; //�����˺�����

const string enemyName[] = { "Ģ����","������","��å��" };
const int enemyCnt = 3;

class Fairy
{
public:
	int getExp();
	int getLevel();

	virtual int attackOtr(Fairy* const) = 0;
	void addExp(int);
	int getHurt(float);
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
	virtual int attackOtr(Fairy* const) override;
	FairyATK();
};

class FairyDEF : public Fairy
{
public:
	virtual int attackOtr(Fairy* const) override;
	FairyDEF();
};

class FairyHP : public Fairy
{
public:
	virtual int attackOtr(Fairy* const) override;
	FairyHP();
};

class FairyAGL : public Fairy
{
public:
	virtual int attackOtr(Fairy* const) override;
	FairyAGL();
};

class FairyEnemy :public Fairy
{
public:
	virtual int attackOtr(Fairy* const) override;
	FairyEnemy();
};

class FairyDummy : public Fairy
{
public:
	virtual int attackOtr(Fairy* const) override;
	FairyDummy();
};