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
	}
	if (ans >= 15) return 15;
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
			valueATK *= (float)pow(timesLevelUp, up);
			break;
		}
		case DEF: {
			valueDEF *= (float)pow(timesLevelUp, up);
			break;
		}
		case HP: {
			valueHP *= (float)pow(timesLevelUp, up);
			break;
		}
		case AGL: {
			valueAGL *= (float)pow(timesLevelUp, up);
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
	if (rnd < probMiss) return 1;
	else if (rnd >= 100 - probCritic)
	{
		val *= timesCritical;
		cri = true;
	}

	if (val > valueDEF)
	{
		nowHP -= (val - valueDEF);
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
	if (otr->adv == DEF) return otr->getHurt(valueATK * timesSuperior);
	return otr->getHurt(valueATK);
}

FairyATK::FairyATK()
{
	name = "ª±¨∫Ô";
	attackName = "ª—Ê≥Â»≠";
	
	adv = ATK;

	valueATK = (float)(rand() % 50) / 10;
	valueDEF = (float)(rand() % 30) / 10;
	valueHP = (float)(rand() % 30) / 10;
	valueAGL = (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 5;
	probMiss = 5;
}

int FairyDEF::attackOtr(Fairy* otr)
{
	if (otr->adv == AGL) return otr->getHurt(valueATK * timesSuperior);
	return otr->getHurt(valueATK);
}

FairyDEF::FairyDEF()
{
	name = "∏÷Ã˙ ﬁ";
	attackName = "∏÷—¿À∫“ß";
	
	adv = DEF;
	
	valueDEF = (float)(rand() % 50) / 10;
	valueATK = (float)(rand() % 30) / 10;
	valueHP = (float)(rand() % 30) / 10;
	valueAGL = (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 5;
	probMiss = 5;
}

int FairyHP::attackOtr(Fairy* otr)
{
	if (otr->adv == ATK) return otr->getHurt(valueATK * timesSuperior);
	return otr->getHurt(valueATK);
}

FairyHP::FairyHP()
{
	name = "Ã©Ãπ–‹";
	attackName = "Ã©…Ω—π∂•";
	
	adv = HP;
	
	valueHP = (float)(rand() % 50) / 10;
	valueATK = (float)(rand() % 30) / 10;
	valueDEF = (float)(rand() % 30) / 10;
	valueAGL = (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 5;
	probMiss = 5;
}

int FairyAGL::attackOtr(Fairy* otr)
{
	if (otr->adv == HP) return otr->getHurt(valueATK * timesSuperior);
	return otr->getHurt(valueATK);
}

FairyAGL::FairyAGL()
{
	name = "æ´¡È¬π";
	attackName = "Ã¯‘æ≤»Ã§";
	
	adv = AGL;
	
	valueAGL = (float)(rand() % 50) / 10;
	valueATK = (float)(rand() % 30) / 10;
	valueDEF = (float)(rand() % 30) / 10;
	valueHP = (float)(rand() % 30) / 10;

	nowHP = valueHP * timesHP;
	exp = 0;
	probCritic = 5;
	probMiss = 5;
}

int FairyEnemy::attackOtr(Fairy* otr)
{
	return otr->getHurt(valueATK);
}

FairyEnemy::FairyEnemy()
{
	name = enemyName[rand() % enemyCnt];
	
	adv = (advantage)((rand() % 4) + 1);
	valueHP = (float)(rand() % 20) / 10;
	valueATK = (float)(rand() % 20) / 10;
	valueAGL = (float)(rand() % 20) / 10;
	valueDEF = (float)(rand() % 20) / 10;

	nowHP = valueHP * timesHP;
	probCritic = 5;
	probMiss = 5;

	exp = 0;
	addExp(rand() % 2000);
}
