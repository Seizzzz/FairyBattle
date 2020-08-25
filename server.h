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

const int enemyCntDefault = 10;

class Server
{
public:
	static Server* getInstance() {
		return &_instance;
	}
	int assign(char* buf, int size, SOCKET& sock, SOCKADDR& addr);

	list<Fairy*> enemyList;
	int constructInfo(char* buf);

private:
	static Server _instance;
	Server();
	~Server();
};