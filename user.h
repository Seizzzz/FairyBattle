#pragma once

#include <string>
#include <list>
#include <ctime>
#include "fairy.h"
using namespace std;

const int numInitialFairy = 3;

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
	void giveOut(const int num);
	Fairy* getFairybyNum(const int num);
	int constructInfo(char* buf);
	int constructAttackName(char* buf, const int num);

	void checkNoFairy();
	void addGiveOut();
	void setName(const string nickname) { name = nickname; };
	void healFairy();
	string getName() { return name; };
	time_t timeLastOpt;
	list<Fairy*> listFairy;
	list<Fairy*> listGiveOut;

	User();

	int timesBattleTol;
	int timesBattleSuc;

	string nameCRC;
private:
	string name;
};