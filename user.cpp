#include "user.h"

int User::constructInfo(char* buf)
{
	fairyInfo* ptr = (fairyInfo*)buf;
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

int User::constructAttackName(char* buf, int num)
{
	strcpy_s(buf, 16, listFairy[num]->attackName.c_str());
	return 0;
}

void User::checkNoFairy()
{
	if (listFairy.empty())
	{
		//listFairy.insert(new Fairy());
	}
}

User::User()
{
	timesBattleSuc = timesBattleTol = 0;
	listFairy.push_back(new FairyATK);
	listFairy.push_back(new FairyDEF);
	listFairy.push_back(new FairyHP);
	listFairy.push_back(new FairyAGL);
	time(&timeLastOpt);
}
