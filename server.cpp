#include "server.h"

struct sockHeder
{
	short tag;
	short cnt;
	unsigned int username;
	unsigned int password;
};

int Server::assign(char* buf, int size, SOCKET& sock, SOCKADDR& addr)
{
	sockHeder* hed = (sockHeder*)buf;
	int ret = -1;

	switch (hed->tag) {
	case 1: { //注册
		ret = Auth::getInstance()->regist(to_string(hed->username), to_string(hed->password), string(((char*)hed) + sizeof(hed), hed->cnt));
		break;
	}
	case 2: { //登录
		ret = Auth::getInstance()->login(to_string(hed->username), to_string(hed->password));
		break;
	} 
	case 3: { //注销
		ret = Auth::getInstance()->logout(to_string(hed->username), to_string(hed->password));
		break;
	}
	case 4: { //发送精灵列表
		sockHeder* srvhed = (sockHeder*)buf;
		srvhed->tag = 106;
		auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));
		if (ptrUser != Auth::getInstance()->savedataMap.end()) //找到了用户
		{
			srvhed->cnt = ptrUser->second->constructInfo((char*)srvhed + sizeof(sockHeder));
		}
		sendto(sock, buf, sizeof(sockHeder) + (hed->cnt * 44), 0, (SOCKADDR*)&addr, sizeof(addr));
		break;
	}

	case 5: { //发送决斗场列表
		sockHeder* srvhed = (sockHeder*)buf;
		srvhed->tag = 107;
		srvhed->cnt = constructInfo((char*)srvhed + sizeof(sockHeder));
		sendto(sock, buf, sizeof(sockHeder) + (hed->cnt * 44), 0, (SOCKADDR*)&addr, sizeof(addr));
		break;
	}

	case 6: { //发送攻击方式
		sockHeder* srvhed = (sockHeder*)buf;
		srvhed->tag = 108;
		auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));
		if (ptrUser != Auth::getInstance()->savedataMap.end()) ptrUser->second->constructAttackName((char*)srvhed + sizeof(sockHeder), srvhed->cnt);
		sendto(sock, buf, sizeof(sockHeder) + 16, 0, (SOCKADDR*)&addr, sizeof(addr));

		break;
	}

	case 7: { //战斗
		sockHeder* srvhed = (sockHeder*)buf;
		srvhed->tag = 109;
		auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));

		int fairyNum = *((int*)buf + 3);
		int enemyNum = *((int*)buf + 4);
		
		auto fairy = ptrUser->second->listFairy.begin(); advance(fairy, fairyNum);
		auto enemy = Server::getInstance()->enemyList.begin(); advance(enemy, enemyNum);

		int len = Battle::getInstance()->pk((char*)buf + sizeof(sockHeder), *fairy, *enemy, ptrUser->second, (int)(*((short*)buf + 1)));
		srvhed->cnt = len;

		sendto(sock, buf, sizeof(sockHeder) + len, 0, (SOCKADDR*)&addr, sizeof(addr));
		break;
	}

	}

	return ret;
}

int Server::constructInfo(char* buf)
{
	fairyInfo* ptr = (fairyInfo*)buf;
	for (auto i : enemyList)
	{
		strcpy_s(ptr->name, 16, i->name.c_str());
		ptr->adv = i->adv;
		ptr->level = i->getLevel();
		ptr->atk = i->valueATK;
		ptr->def = i->valueDEF;
		ptr->hp = i->valueHP;
		ptr->agl = i->valueAGL;
		ptr->nowhp = i->nowHP;
		++ptr;
	}

	return enemyList.size();
}

Server::Server()
{
	for (int i = 0; i < enemyCntDefault; ++i) enemyList.push_back(new FairyEnemy);
}

Server::~Server()
{
	for (auto i : enemyList)
	{
		delete i;
	}
}

Server Server::_instance;