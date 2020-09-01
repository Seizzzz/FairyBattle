#include "battle.h"

int Battle::pk(char* buf, Fairy* const fairy, Fairy* const enemy, User* const user, const int type)
{
	char* oldbuf = buf;
	int percent = (int)((double)fairy->valueAGL * 100 / ((double)fairy->valueAGL + enemy->valueAGL));
	int round = 0; //回合数

	strcat_s(buf, BUF_SIZE, "======战斗开始======\n"); buf += strlen(buf);
	while (true)
	{
		if (++round > 100) {
			sprintf(buf, "经过100回合都没有分出胜负！下次再试试吧！\n");
			buf += strlen(buf);
			break;
		}

		if (rand() % 100 < percent)
		{
			float oldhp = enemy->nowHP;
			int state = fairy->attackOtr(enemy);
			float newhp = enemy->nowHP;
			if (state == 0) sprintf(buf, "精灵%s对怪物%s造成了%.1f点伤害！\n", fairy->name.c_str(), enemy->name.c_str(), oldhp - newhp);
			else if (state == 1) sprintf(buf, "闪避！怪物没有受到伤害！\n"); 
			else if (state == 2) sprintf(buf, "暴击！精灵%s对怪物%s造成了%.1f点伤害！\n", fairy->name.c_str(), enemy->name.c_str(), oldhp - newhp);
			buf += strlen(buf);
		}
		else
		{
			float oldhp = fairy->nowHP;
			int state = enemy->attackOtr(fairy);
			float newhp = fairy->nowHP;
			if (state == 0) sprintf(buf, "怪物%s对精灵%s造成了%.1f点伤害！\n", enemy->name.c_str(), fairy->name.c_str(), oldhp - newhp);
			else if(state == 1) sprintf(buf, "闪避！精灵没有受到伤害！\n");
			else if(state == 2) sprintf(buf, "暴击！怪物%s对精灵%s造成了%.1f点伤害！\n", enemy->name.c_str(), fairy->name.c_str(), oldhp - newhp);
			buf += strlen(buf);
		}

		if (fairy->nowHP <= 0)
		{
			if (type == 0) //用户升级赛败
			{ 
				++user->timesBattleTol;
				enemy->getHeal();
				strcat_s(buf, BUF_SIZE, "升级赛失败！\n"); buf += strlen(buf);
				break;
			}
			else //用户决斗赛败
			{
				user->addGiveOut();
				++user->timesBattleTol;
				enemy->getHeal();
				strcat_s(buf, BUF_SIZE, "决斗赛失败！\n"); buf += strlen(buf);
				//send 随机送出
				break;
			}
		}
		else if (enemy->nowHP <= 0)
		{
			if (type == 0) //用户升级赛胜
			{
				++user->timesBattleSuc;
				++user->timesBattleTol;
				fairy->addExp(enemy->getLevel() * 100);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "升级赛成功！\n"); buf += strlen(buf);
				sprintf(buf, "获得经验%d点！\n", enemy->getLevel() * 100); buf += strlen(buf);
				break;
			}
			else //用户决斗赛胜
			{
				++user->timesBattleSuc;
				++user->timesBattleTol;
				fairy->addExp(enemy->getLevel() * 100);
				user->listFairy.push_back(enemy);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "决斗赛成功！\n"); buf += strlen(buf);
				sprintf(buf, "获得经验%d点！\n", enemy->getLevel() * 100); buf += strlen(buf);
				sprintf(buf, "获得了新精灵\"%s\"！\n", enemy->name.c_str()); buf += strlen(buf);
				break;
			}

		}
	}
	strcat_s(buf, BUF_SIZE, "======战斗结束======\n"); buf += strlen(buf);

	return strlen(oldbuf);
}

Battle Battle::_instance;
