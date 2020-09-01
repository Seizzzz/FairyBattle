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
	int regist(const string username, const string password, const char* nickname);
	int login(const string username, const string password);
	int logout(const string username, const string password);
	int verify(const string username, const string password);
	list<User*> onlineList;
	int getOnlinePlayer(char* buf);
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