#include "user.h"

void User::giveOut(const int num)
{
	if (listGiveOut.empty()) //普通移除
	{
		auto fairy = listFairy.begin(); advance(fairy, num);
		listFairy.remove(*fairy);
	}
	else //清除待送出的精灵，并将剩余返还
	{
		auto fairy = listGiveOut.begin(); advance(fairy, num);
		listGiveOut.remove(*fairy);
		for (auto i : listGiveOut) listFairy.push_back(i);
		listGiveOut.clear();
	}

	checkNoFairy();
}

Fairy* User::getFairybyNum(const int num)
{
	if (listGiveOut.empty())
	{
		auto fairy = listFairy.begin(); advance(fairy, num);
		return *fairy;
	}
	else
	{
		auto fairy = listGiveOut.begin(); advance(fairy, num);
		return *fairy;
	}
}

int User::constructInfo(char* buf)
{
	fairyInfo* ptr = (fairyInfo*)buf;

	if (!listGiveOut.empty()) //还有未送出的精灵
	{
		for (auto i : listGiveOut)
		{
			strcpy_s(ptr->name, 16, i->name.c_str());
			ptr->adv = i->adv;
			ptr->level = i->getLevel();
			ptr->nowhp = i->nowHP;
			ptr->atk = i->valueATK;
			ptr->def = i->valueDEF;
			ptr->hp = i->valueHP;
			ptr->agl = i->valueAGL;
			++ptr;
		}

		return listGiveOut.size();
	}
	else //普通获取精灵列表
	{
		for (auto i : listFairy)
		{
			strcpy_s(ptr->name, 16, i->name.c_str());
			ptr->adv = i->adv;
			ptr->level = i->getLevel();
			ptr->nowhp = i->nowHP;
			ptr->atk = i->valueATK;
			ptr->def = i->valueDEF;
			ptr->hp = i->valueHP;
			ptr->agl = i->valueAGL;
			++ptr;
		}

		return listFairy.size();
	}
}

int User::constructAttackName(char* buf, const int num)
{
	auto fairy = getFairybyNum(num);
	strcpy_s(buf, 16, fairy->attackName.c_str());
	return 0;
}

void User::checkNoFairy()
{
	if (listFairy.empty())
	{
		switch (rand() % 4)
		{
		case 0: {
			listFairy.push_back(new FairyATK);
			break;
		}
		case 1: {
			listFairy.push_back(new FairyDEF);
			break;
		}
		case 2: {
			listFairy.push_back(new FairyHP);
			break;
		}
		case 3: {
			listFairy.push_back(new FairyAGL);
			break;
		}
		}
	}
}

void User::addGiveOut()
{
	int togive = 0;
	if (listFairy.size() >= 3) togive = 3;
	else if (listFairy.size() < 3) togive = listFairy.size();

	while (togive--)
	{
		int rnd;
		auto ptr = listFairy.begin();
		if (listFairy.size() == 1) rnd = 0;
		else rnd = rand() % (listFairy.size() - 1);
		if(rnd) advance(ptr, rnd);

		listGiveOut.push_back(*ptr);
		listFairy.remove(*ptr);
	}
}

void User::healFairy()
{
	for (auto i : listFairy) i->getHeal();
}

User::User()
{
	timesBattleSuc = timesBattleTol = 0;

	for (int i = 0; i < numInitialFairy; ++i)
	{
		switch (rand() % 4)
		{
		case 0: {listFairy.push_back(new FairyATK); break; }
		case 1: {listFairy.push_back(new FairyDEF); break; }
		case 2: {listFairy.push_back(new FairyHP); break; }
		case 3: {listFairy.push_back(new FairyAGL); break; }
		}
	}

	time(&timeLastOpt);
}
