#pragma once

#include <string>
#include <vector>
#include <ctime>
#include "fairy.h"
using namespace std;

struct fairyInfo
{
	char name[16];
	int adv;
	int level;
	float nowhp;
	float atk, def, hp, agl;
};

class User
{
public:
	void joinLevelBattle(Fairy* otr);
	void joinDeathBattle(Fairy* otr);
	void giveOut(Fairy* otr);
	int constructInfo(char* buf);
	int constructAttackName(char* buf, int num);

	void checkNoFairy();
	void setName(string nickname) { name = nickname; };
	string getName() { return name; };
	time_t timeLastOpt;
	vector<Fairy*> listFairy;
	User();

private:
	string name;
	int timesBattleTol;
	int timesBattleSuc;
};