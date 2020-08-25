#include "auth.h"

int Auth::regist(string username, string password, string nickname)
{
	auto ptr = registMap.find(username);
	if (ptr == registMap.end()) {
		User* nUser = new User;
		nUser->setName(nickname);
		registMap.insert(pair<string, string>(username, password));
		savedataMap.insert(pair<string, User*>(username, nUser));
		return 102;
	}
	return 203;
}

int Auth::login(string username, string password)
{
	int ret = verify(username, password);
	if (ret == 101)
	{
		for (auto i : onlineList)
		{
			if (i->getName() == username) return ret; //���������б���
		}

		auto ptr = savedataMap.find(username); //���������б��У����Ҳ����
		onlineList.push_back(ptr->second);
	}
	return ret;
}

int Auth::logout(string username, string password)
{
	int ret = verify(username, password);
	if (ret == 101)
	{
		for (auto i = onlineList.begin(); i != onlineList.end(); ++i)
		{
			if ((*i)->getName() == username) onlineList.erase(i);
		}
		return 105;
	}
	return ret;
}

int Auth::verify(string username, string password)
{
	auto ptr = registMap.find(username);
	if (ptr == registMap.end()) return 201; //�����ڵ��û�
	else if (ptr->second != password) return 202; //�������

	auto online = savedataMap.find(username); //��������״̬
	if (online != savedataMap.end()) time(&online->second->timeLastOpt);

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