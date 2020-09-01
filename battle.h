#pragma once

#include <thread>
#include "user.h"
#include "fairy.h"
#include "server.h"
#define BUF_SIZE 32768
using namespace std;

class Battle
{
public:
	int pk(char* buf, Fairy* const f1, Fairy* const f2, User* const u, const int type); //0 level; 1 death
	static Battle* getInstance() {
		return &_instance;
	}

private:
	Battle() {};
	static Battle _instance;
};