#pragma once
#include <WinSock2.h>
#include <string>
#include <list>
#include "auth.h"
#include "user.h"
#include "fairy.h"
#include "battle.h"
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

const int enemyCntDefault = 20;

class Server
{
public:
	static Server* getInstance() {
		return &_instance;
	}
	int assign(char*, int, SOCKET&, SOCKADDR&);

	Fairy* getEnemybyNum(const int);
	list<Fairy*> listEnemy;
	int buildInfoEnemy(char*);
	int informReport(char*, const char*);

private:
	static Server _instance;
	Server();
	~Server();
};