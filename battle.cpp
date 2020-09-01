#include "battle.h"

int Battle::pk(char* buf, Fairy* const fairy, Fairy* const enemy, User* const user, const int type)
{
	char* oldbuf = buf;
	int percent = (int)((double)fairy->valueAGL * 100 / ((double)fairy->valueAGL + enemy->valueAGL));
	int round = 0; //�غ���

	strcat_s(buf, BUF_SIZE, "======ս����ʼ======\n"); buf += strlen(buf);
	while (true)
	{
		if (++round > 100) {
			sprintf(buf, "����100�غ϶�û�зֳ�ʤ�����´������԰ɣ�\n");
			buf += strlen(buf);
			break;
		}

		if (rand() % 100 < percent)
		{
			float oldhp = enemy->nowHP;
			int state = fairy->attackOtr(enemy);
			float newhp = enemy->nowHP;
			if (state == 0) sprintf(buf, "����%s�Թ���%s�����%.1f���˺���\n", fairy->name.c_str(), enemy->name.c_str(), oldhp - newhp);
			else if (state == 1) sprintf(buf, "���ܣ�����û���ܵ��˺���\n"); 
			else if (state == 2) sprintf(buf, "����������%s�Թ���%s�����%.1f���˺���\n", fairy->name.c_str(), enemy->name.c_str(), oldhp - newhp);
			buf += strlen(buf);
		}
		else
		{
			float oldhp = fairy->nowHP;
			int state = enemy->attackOtr(fairy);
			float newhp = fairy->nowHP;
			if (state == 0) sprintf(buf, "����%s�Ծ���%s�����%.1f���˺���\n", enemy->name.c_str(), fairy->name.c_str(), oldhp - newhp);
			else if(state == 1) sprintf(buf, "���ܣ�����û���ܵ��˺���\n");
			else if(state == 2) sprintf(buf, "����������%s�Ծ���%s�����%.1f���˺���\n", enemy->name.c_str(), fairy->name.c_str(), oldhp - newhp);
			buf += strlen(buf);
		}

		if (fairy->nowHP <= 0)
		{
			if (type == 0) //�û���������
			{ 
				++user->timesBattleTol;
				enemy->getHeal();
				strcat_s(buf, BUF_SIZE, "������ʧ�ܣ�\n"); buf += strlen(buf);
				break;
			}
			else //�û���������
			{
				user->addGiveOut();
				++user->timesBattleTol;
				enemy->getHeal();
				strcat_s(buf, BUF_SIZE, "������ʧ�ܣ�\n"); buf += strlen(buf);
				//send ����ͳ�
				break;
			}
		}
		else if (enemy->nowHP <= 0)
		{
			if (type == 0) //�û�������ʤ
			{
				++user->timesBattleSuc;
				++user->timesBattleTol;
				fairy->addExp(enemy->getLevel() * 100);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "�������ɹ���\n"); buf += strlen(buf);
				sprintf(buf, "��þ���%d�㣡\n", enemy->getLevel() * 100); buf += strlen(buf);
				break;
			}
			else //�û�������ʤ
			{
				++user->timesBattleSuc;
				++user->timesBattleTol;
				fairy->addExp(enemy->getLevel() * 100);
				user->listFairy.push_back(enemy);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "�������ɹ���\n"); buf += strlen(buf);
				sprintf(buf, "��þ���%d�㣡\n", enemy->getLevel() * 100); buf += strlen(buf);
				sprintf(buf, "������¾���\"%s\"��\n", enemy->name.c_str()); buf += strlen(buf);
				break;
			}

		}
	}
	strcat_s(buf, BUF_SIZE, "======ս������======\n"); buf += strlen(buf);

	return strlen(oldbuf);
}

Battle Battle::_instance;
