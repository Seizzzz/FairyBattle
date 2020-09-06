#include "fairy.h"

int Fairy::getExp()
{
	return exp;
}

int Fairy::getLevel() // 100 300 600 1000...
{
	int susp = 100, ans = 1;
	for (int i = 0; i < exp; i += susp)
	{
		susp += 100;
		++ans;
		if (ans >= 15) return 15;
	}
	return ans;
}

void Fairy::addExp(int val)
{
	int oldLevel = getLevel();
	if (oldLevel == 15) return;

	exp += val;
	int newLevel = getLevel();

	int up = newLevel - oldLevel; //…˝º∂¡À
	if (up > 0)
	{
		valueATK *= (float)pow(timesLevelUp, up);
		valueDEF *= (float)pow(timesLevelUp, up);
		valueHP *= (float)pow(timesLevelUp, up);
		valueAGL *= (float)pow(timesLevelUp, up);
		switch (adv) //Ãÿ≥§∂ÓÕ‚÷µ
		{
		case ATK: {
			valueATK *= (float)pow(timesLevelUpMain, up);
			break;
		}
		case DEF: {
			valueDEF *= (float)pow(timesLevelUpMain, up);
			break;
		}
		case HP: {
			valueHP *= (float)pow(timesLevelUpMain, up);
			break;
		}
		case AGL: {
			valueAGL *= (float)pow(timesLevelUpMain, up);
			break;
		}
		default:
			break;
		}
	}
}

int Fairy::getHurt(float val) //ret=1 miss; ret=2 critical; ret=0 normal
{
	int rnd = rand() % 100; bool cri = false;
	if (rnd < probMiss) return 1; //…¡±‹
	else if (rnd >= 100 - probCritic) //±©ª˜
	{
		val *= timesCritical;
		cri = true;
	}

	if (val > valueDEF * timesDefend)
	{
		nowHP -= (val - valueDEF * timesDefend);
		if (nowHP < 0) nowHP = 0;
	}
	
	if (cri) return 2;
	return 0;
}

void Fairy::getHeal()
{
	nowHP = valueHP * 100;
}

int FairyATK::attackOtr(Fairy* otr)
{
	if (otr->adv == DEF) return otr->getHurt(valueATK * timesSuperior * timesAttack);
	return otr->getHurt(valueATK * timesAttack);
}

FairyATK::FairyATK()
{
	name = "ª±¨∫Ô";
	attackName = "ª—Ê≥Â»≠";
	
	adv = ATK;

	valueATK = 2 + (float)(rand() % 30) / 10;
	valueDEF = 1 + (float)(rand() % 30) / 10;
	valueHP = 1 + (float)(rand() % 30) / 10;
	valueAGL = 1 + (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 30;
	probMiss = 20;
}

int FairyDEF::attackOtr(Fairy* otr)
{
	if (otr->adv == AGL) return otr->getHurt(valueATK * timesSuperior * timesAttack);
	return otr->getHurt(valueATK * timesAttack);
}

FairyDEF::FairyDEF()
{
	name = "∏÷Ã˙ ﬁ";
	attackName = "∏÷—¿À∫“ß";
	
	adv = DEF;
	
	valueATK = 1 + (float)(rand() % 30) / 10;
	valueDEF = 2 + (float)(rand() % 30) / 10;
	valueHP = 1 + (float)(rand() % 30) / 10;
	valueAGL = 1 + (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 20;
	probMiss = 20;
}

int FairyHP::attackOtr(Fairy* otr)
{
	if (otr->adv == ATK) return otr->getHurt(valueATK * timesSuperior * timesAttack);
	return otr->getHurt(valueATK * timesAttack);
}

FairyHP::FairyHP()
{
	name = "Ã©Ãπ–‹";
	attackName = "Ã©…Ω—π∂•";
	
	adv = HP;
	
	valueATK = 1 + (float)(rand() % 30) / 10;
	valueDEF = 1 + (float)(rand() % 30) / 10;
	valueHP = 2 + (float)(rand() % 30) / 10;
	valueAGL = 1 + (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 20;
	probMiss = 20;
}

int FairyAGL::attackOtr(Fairy* otr)
{
	if (otr->adv == HP) return otr->getHurt(valueATK * timesSuperior * timesAttack);
	return otr->getHurt(valueATK * timesAttack);
}

FairyAGL::FairyAGL()
{
	name = "æ´¡È¬π";
	attackName = "Ã¯‘æ≤»Ã§";
	
	adv = AGL;
	
	valueATK = 1 + (float)(rand() % 30) / 10;
	valueDEF = 1 + (float)(rand() % 30) / 10;
	valueHP = 1 + (float)(rand() % 30) / 10;
	valueAGL = 2 + (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 20;
	probMiss = 30;
}

int FairyEnemy::attackOtr(Fairy* otr)
{
	return otr->getHurt(valueATK * timesAttack);
}

FairyEnemy::FairyEnemy()
{
	name = enemyName[rand() % enemyCnt];
	attackName = "π÷ŒÔÀ∫“ß";
	
	adv = (advantage)((rand() % 4) + 1);
	valueHP = (float)(rand() % 20) / 10;
	valueATK = (float)(rand() % 20) / 10;
	valueAGL = (float)(rand() % 20) / 10;
	valueDEF = (float)(rand() % 20) / 10;

	nowHP = valueHP * timesHP;
	probCritic = 20;
	probMiss = 20;

	exp = 0;
	addExp(rand() % 8000);
	getHeal();
}

int FairyDummy::attackOtr(Fairy* const otr)
{
	return otr->getHurt(0);
}

FairyDummy::FairyDummy()
{
	name = "ºŸ»À";
	attackName = "ºŸ»À";

	adv = (advantage)((rand() % 4) + 1);
	valueHP = 0.1;
	valueATK = 0;
	valueAGL = 0;
	valueDEF = 0;

	nowHP = valueHP * timesHP;
	probCritic = 0;
	probMiss = 0;

	exp = 0;
	addExp(50000);
	getHeal();
}
