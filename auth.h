#pragma once
#include "fairy.h"
#include "user.h"
#include "crc32.h"
#include <map>
#include <string>
#include <list>
#include <ctime>
using namespace std;

struct infoOnlinePlayer
{
	char name[16];
	time_t lastOpt;
	int totalBattle;
	int winBattle;
};

class Auth
{
public:
	int regist(const string , const string, const char*);
	int login(const string, const string);
	int logout(const string, const string);
	int verify(const string, const string);
	list<User*> onlineList;
	int getOnlinePlayer(char*);
	map<string, string> registMap; //���û���ӳ��������
	map<string, User*> savedataMap; //���û���ӳ�����û���
	int saveMap();
	static Auth* getInstance() {
		return &_instance;
	}

private:
	static Auth _instance;
	Auth();
	int loadMap();
};