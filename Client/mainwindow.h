#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <string>
#include "crc32.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define PORT_SRV 62100

struct sockHeder
{
    short tag;
    short cnt;
    unsigned int username;
    unsigned int password;
};

struct fairyInfo
{
    char name[16];
    int adv;
    int level;
    float nowhp;
    float atk, def, hp, agl;
    int cri, miss;
};

struct infoOnlinePlayer
{
    char name[16];
    long long lastOpt;
    int totalBattle;
    int winBattle;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    char ADDR_SRV[32] = "192.168.132.1";
    QUdpSocket* sock;

    void sockListen();
    void getAttackName(const int num);
    void requestArena();
    void requestSelfFairy();
    void requestOthersFairy();
    void report(const QString& s);
    QString time_t2QString(const time_t t);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
