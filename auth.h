#pragma once
#include "fairy.h"
#include "user.h"
#include <map>
#include <string>
#include <list>
#include <ctime>
using namespace std;

class Auth
{
public:
	int regist(string username, string password, string nickname);
	int login(string username, string password);
	int logout(string username, string password);
	list<User*> onlineList;
	map<string, string> registMap; //由用户名映射至密码
	map<string, User*> savedataMap; //由用户名映射至用户类
	int saveMap();
	static Auth* getInstance() {
		return &_instance;
	}

private:
	static Auth _instance;
	Auth();
	int loadMap();
	int verify(string username, string password);
};