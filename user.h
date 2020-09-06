#pragma once

#include <string>
#include <vector>
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
	void giveOut(const int);
	Fairy* getFairybyNum(const int);
	int buildInfoFairy(char*);
	int buildInfoBadage(char*);
	int buildInfoAttack(char*, const int);

	void checkNoFairy();
	void addGiveOut();
	void setName(const string nickname) { name = nickname; };
	void healFairy();
	void getBadage();
	string getName() { return name; };
	time_t timeLastOpt;
	list<Fairy*> listFairy;
	list<Fairy*> listGiveOut;
	vector<string> vecBadage;

	User();
	~User();

	int timesBattleTol;
	int timesBattleSuc;

	string nameCRC;
private:
	string name;
};