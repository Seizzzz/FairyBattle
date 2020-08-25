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
			sprintf(buf, "����%s�Թ���%s�����%.1f���˺���\n", fairy->name.c_str(), enemy->name.c_str(), fairy->valueATK); buf += strlen(buf);
		}
		else
		{
			enemy->attackOtr(fairy);
			sprintf(buf, "����%s�Ծ���%s�����%.1f���˺���\n", enemy->name.c_str(), fairy->name.c_str(), enemy->valueATK); buf += strlen(buf);
		}

		if (fairy->nowHP <= 0)
		{
			if (type == 0) //�û���������
			{ 
				strcat_s(buf, BUF_SIZE, "������ʧ�ܣ�\n"); buf += strlen(buf);
				break;
			}
			else //�û���������
			{
				strcat_s(buf, BUF_SIZE, "������ʧ�ܣ�\n"); buf += strlen(buf);
				//send ����ͳ�
				break;
			}
		}
		else if (enemy->nowHP <= 0)
		{
			if (type == 0) //�û�������ʤ
			{
				fairy->addExp(enemy->getLevel() * 100);
				strcat_s(buf, BUF_SIZE, "�������ɹ���\n"); buf += strlen(buf);
				sprintf(buf, "��þ���%d�㣡\n", enemy->getLevel() * 100); buf += strlen(buf);
				break;
			}
			else //�û�������ʤ
			{
				fairy->addExp(enemy->getLevel() * 100);
				user->listFairy.push_back(enemy);
				Server::getInstance()->enemyList.remove(enemy);
				Server::getInstance()->enemyList.push_back(new FairyEnemy);
				strcat_s(buf, BUF_SIZE, "�������ɹ���\n"); buf += strlen(buf);
				sprintf(buf, "������¾���\"%s\"��\n", enemy->name.c_str()); buf += strlen(buf);
				break;
			}

		}
	}

	return strlen(oldbuf);
}

Battle Battle::_instance;
