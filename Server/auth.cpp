#include "auth.h"

int Auth::regist(const string username, const string password, const char* nickname)
{
	string s(nickname);
	auto ptr = registMap.find(username);
	if (ptr == registMap.end()) {
		User* nUser = new User;
		nUser->setName(s);
		nUser->nameCRC = username;
		registMap.insert(pair<string, string>(username, password));
		savedataMap.insert(pair<string, User*>(username, nUser));
		return 102;
	}
	return 203;
}

int Auth::login(const string username, const string password)
{
	int ret = verify(username, password);
	if (ret == 101)
	{
		for (auto i : onlineList)
		{
			if (i->nameCRC == username) return ret; //已在在线列表中
		}

		auto ptr = savedataMap.find(username); //不在在线列表中，查找并添加
		onlineList.push_back(ptr->second);
	}
	return ret;
}

int Auth::logout(const string username, const string password)
{
	int ret = verify(username, password);
	if (ret == 101)
	{
		for (auto i : onlineList)
		{
			//if (time(nullptr) - i->timeLastOpt > 30) i->timeLastOpt = 0;

			if (i->nameCRC == username)
			{
				//onlineList.remove(i);
				i->timeLastOpt = 0;
				break;
			}
		}
		return 105;
	}
	return ret;
}

int Auth::getOnlinePlayer(char* buf)
{
	infoOnlinePlayer* ptr = (infoOnlinePlayer*)buf;

	for (auto i : onlineList)
	{
		strcpy_s(ptr->name, i->getName().c_str());
		ptr->lastOpt = i->timeLastOpt;
		ptr->totalBattle = i->timesBattleTol;
		ptr->winBattle = i->timesBattleSuc;
		++ptr;
	}

	return onlineList.size();
}

int Auth::verify(const string username, const string password)
{
	auto ptr = registMap.find(username);
	if (ptr == registMap.end()) return 201; //不存在的用户
	else if (ptr->second != password) return 202; //密码错误

	auto online = savedataMap.find(username); //更新在线状态
	time(&(online->second->timeLastOpt));

	return 101;
}

Auth::Auth() {
	loadMap();
}

int Auth::loadMap()
{
	return 200;
}

int Auth::saveMap()
{
	return 200;
}

Auth Auth::_instance;