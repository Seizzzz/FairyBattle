#include "battle.h"

int Battle::pk(char* buf, Fairy* fairy, Fairy* enemy, User* user, int type)
{
	char* oldbuf = buf;
	int percent = (int)((double)fairy->valueAGL * 100 / ((double)fairy->valueAGL + enemy->valueAGL));

	while (true)
	{
		if (rand() % 100 < percent)
		{
			fairy->attackOtr(enemy);
			sprintf(buf, "精灵%s对怪物%s造成了%.1f点伤害！\n", fairy->name.c_str(), enemy->name.c_str(), fairy->valueATK); buf += strlen(buf);
		}
		else
		{
			enemy->attackOtr(fairy);
			sprintf(buf, "怪物%s对精灵%s造成了%.1f点伤害！\n", enemy->name.c_str(), fairy->name.c_str(), enemy->valueATK); buf += strlen(buf);
		}

		if (fairy->nowHP <= 0)
		{
			if (type == 0) //用户升级赛败
			{ 
				strcat_s(buf, BUF_SIZE, "升级赛失败！\n"); buf += strlen(buf);
				break;
			}
			else //用户决斗赛败
			{
				strcat_s(buf, BUF_SIZE, "决斗赛失败！\n"); buf += strlen(buf);
				//send 随机送出
				break;
			}
		}
		else if (enemy->nowHP <= 0)
		{
			if (type == 0) //用户升级赛胜
			{
				fairy->addExp(enemy->getLevel() * 100);
				strcat_s(buf, BUF_SIZE, "升级赛成功！\n"); buf += strlen(buf);
				sprintf(buf, "获得经验%d点！\n", enemy->getLevel() * 100); buf += strlen(buf);
				break;
			}
			else //用户决斗赛胜
			{
				fairy->addExp(enemy->getLevel() * 100);
				user->listFairy.push_back(enemy);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "决斗赛成功！\n"); buf += strlen(buf);
				sprintf(buf, "获得了新精灵\"%s\"！\n", enemy->name.c_str()); buf += strlen(buf);
				break;
			}

		}
	}

	return strlen(oldbuf);
}

Battle Battle::_instance;
