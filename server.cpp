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
		ret = Auth::getInstance()->regist(to_string(hed->username), to_string(hed->password), (char*)hed + sizeof(sockHeder));
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

		if (ptrUser->second->listGiveOut.size())
		{
			srvhed->tag = 204;
			int len = informReport((char*)srvhed + sizeof(sockHeder), "有精灵待送出！\n");
			sendto(sock, buf, sizeof(sockHeder) + len, 0, (SOCKADDR*)&addr, sizeof(addr));
		}

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
		if (Auth::getInstance()->verify(to_string(hed->username), to_string(hed->password)) == 101)
		{
			sockHeder* srvhed = (sockHeder*)buf;
			srvhed->tag = 108;
			auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));
			if (ptrUser != Auth::getInstance()->savedataMap.end()) ptrUser->second->constructAttackName((char*)srvhed + sizeof(sockHeder), srvhed->cnt);
			sendto(sock, buf, sizeof(sockHeder) + 16, 0, (SOCKADDR*)&addr, sizeof(addr));
		}
		break;
	}

	case 7: { //战斗
		if (Auth::getInstance()->verify(to_string(hed->username), to_string(hed->password)) == 101)
		{
			sockHeder* srvhed = (sockHeder*)buf;
			srvhed->tag = 109;
			auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));

			int fairyNum = *((int*)buf + 3);
			int enemyNum = *((int*)buf + 4);

			auto fairy = ptrUser->second->getFairybyNum(fairyNum);
			auto enemy = getEnemybyNum(enemyNum);

			int len = Battle::getInstance()->pk((char*)buf + sizeof(sockHeder), fairy, enemy, ptrUser->second, (int)(*((short*)buf + 1)));
			srvhed->cnt = len;

			sendto(sock, buf, sizeof(sockHeder) + len, 0, (SOCKADDR*)&addr, sizeof(addr));
		}
		break;
	}

	case 8: { //送出
		if (Auth::getInstance()->verify(to_string(hed->username), to_string(hed->password)) == 101)
		{
			sockHeder* srvhed = (sockHeder*)buf;
			srvhed->tag = 110;
			auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));

			int fairyNum = *((int*)buf + 3);
			ptrUser->second->giveOut(fairyNum);

			sendto(sock, buf, sizeof(sockHeder), 0, (SOCKADDR*)&addr, sizeof(addr));
		}
		break;
	}

	case 9: { //发送玩家列表
		sockHeder* srvhed = (sockHeder*)buf;
		srvhed->tag = 111;

		srvhed->cnt = Auth::getInstance()->getOnlinePlayer((char*)srvhed + sizeof(sockHeder));

		sendto(sock, buf, sizeof(sockHeder) + srvhed->cnt * 32, 0, (SOCKADDR*)&addr, sizeof(addr));
		break;
	}

	case 10: { //治疗
		if (Auth::getInstance()->verify(to_string(hed->username), to_string(hed->password)) == 101)
		{
			sockHeder* srvhed = (sockHeder*)buf;
			srvhed->tag = 112;
			auto ptrUser = Auth::getInstance()->savedataMap.find(to_string(hed->username));
			if (ptrUser != Auth::getInstance()->savedataMap.end()) ptrUser->second->healFairy();

			sendto(sock, buf, sizeof(sockHeder), 0, (SOCKADDR*)&addr, sizeof(addr));
		}
		break;
	}
	}

	return ret;
}

Fairy* Server::getEnemybyNum(const int num)
{
	auto enemy = enemyList.begin(); advance(enemy, num);
	return *enemy;
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

int Server::informReport(char* buf, const char* text)
{
	strcpy_s(buf, BUF_SIZE, text);
	return strlen(text);
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