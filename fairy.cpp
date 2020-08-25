#include "fairy.h"

int Fairy::getExp()
{
	return exp;
}

int Fairy::getLevel()
{
	/*int susp = 100, ans = 1;
	for (int i = 0; i < exp; i += susp)
	{
		susp += 100;
		++ans;
	}
	return ans;*/
}

void Fairy::addExp(int val)
{
	int oldLevel = getLevel();
	exp += val;
	int newLevel = getLevel();

	int up = newLevel - oldLevel;
	if (up > 0)
	{
		valueATK *= (float)pow(timesLevelUp, up);
		valueDEF *= (float)pow(timesLevelUp, up);
		valueHP *= (float)pow(timesLevelUp, up);
		valueAGL *= (float)pow(timesLevelUp, up);
	}
}

bool Fairy::getHurt(float val)
{
	if(val > valueDEF) nowHP -= (val - valueDEF);
	if (nowHP < 0)
	{
		nowHP = 0;
		return true;
	}
	return false;
}

void FairyATK::attackOtr(Fairy* otr)
{
	if (otr->adv == DEF) otr->getHurt(valueATK * timesSuperior);
	else otr->getHurt(valueATK);
}

FairyATK::FairyATK()
{
	name = "»ð±¬ºï";
	attackName = "»ðÑæ³åÈ­";
	
	adv = ATK;
	valueATK = 1.1;
	valueHP = valueDEF = valueAGL = 1;
	nowHP = valueHP * timesHP;
}

void FairyDEF::attackOtr(Fairy* otr)
{
	if (otr->adv == AGL) otr->getHurt(valueATK * timesSuperior);
	else otr->getHurt(valueATK);
}

FairyDEF::FairyDEF()
{
	name = "¸ÖÌúÊÞ";
	attackName = "¸ÖÑÀ¹¥»÷";
	
	adv = DEF;
	valueDEF = 1.1;
	valueATK = valueHP = valueAGL = 1;
	nowHP = valueHP * timesHP;
}

void FairyHP::attackOtr(Fairy* otr)
{
	if (otr->adv == ATK) otr->getHurt(valueATK * timesSuperior);
	else otr->getHurt(valueATK);
}

FairyHP::FairyHP()
{
	name = "Ì©Ì¹ÐÜ";
	attackName = "Ì©É½Ñ¹¶¥";
	
	adv = HP;
	valueHP = 1.1;
	valueATK = valueDEF = valueAGL = 1;
	nowHP = valueHP * timesHP;
}

void FairyAGL::attackOtr(Fairy* otr)
{
	if (otr->adv == HP) otr->getHurt(valueATK * timesSuperior);
	else otr->getHurt(valueATK);
}

FairyAGL::FairyAGL()
{
	name = "¾«ÁéÂ¹";
	attackName = "ÌøÔ¾¹¥»÷";
	
	adv = AGL;
	valueAGL = 1.1;
	valueATK = valueHP = valueDEF = 1;
	nowHP = valueHP * timesHP;
}

void FairyEnemy::attackOtr(Fairy* otr)
{
	otr->getHurt(valueATK);
}

FairyEnemy::FairyEnemy()
{
	name = enemyName[rand() % enemyCnt];
	
	adv = (advantage)((rand() % 4) + 1);
	valueHP = 0.1;
	valueATK = valueAGL = valueDEF = 0;

	nowHP = valueHP * timesHP;
}
