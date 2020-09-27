#include "mainwindow.h"
#include "ui_mainwindow.h"

void printBuf(char* buf, const int conSize)
{
    for (int i = 0; i < conSize; ++i) {
        printf("%2.2x ", (unsigned char)buf[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n");
}

void MainWindow::report(const QString& s)
{
    ui->plainTextEditState->insertPlainText(s);
    ui->plainTextEditState->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
}

void MainWindow::requestArena()
{
    sockHeder hed;
    hed.tag = 5;
    hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
    hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

    QByteArray buf;
    buf.append((char*)&hed,sizeof(sockHeder));
    sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
}

void MainWindow::requestSelfFairy()
{
    sockHeder hed;
    hed.tag = 4;
    hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
    hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

    QByteArray buf;
    buf.append((char*)&hed,sizeof(sockHeder));
    sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);

    ui->labelFairy->setText("当前拥有的精灵：");
}

void MainWindow::requestOthersFairy()
{
    if(ui->tableWidgetPlayer->selectedItems().isEmpty()) return;
    sockHeder hed;
    hed.tag = 4;
    //hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
    hed.username = crc32((unsigned char*)ui->tableWidgetPlayer->selectedItems().first()->text().toStdString().c_str(), ui->tableWidgetPlayer->selectedItems().first()->text().toStdString().size()).getCRC();

    QByteArray buf;
    buf.append((char*)&hed,sizeof(sockHeder));
    sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);

    ui->labelFairy->setText("玩家"+ui->tableWidgetPlayer->selectedItems().first()->text()+"拥有的精灵：");
}

QString MainWindow::time_t2QString(const time_t t)
{
    QString ret;
    tm time;
    localtime_s(&time, &t);

    time.tm_year += 1900;
    time.tm_mon += 1;

    ret.append(QString::number(time.tm_year)+"."+QString::number(time.tm_mon)+"."+QString::number(time.tm_mday)+" "+QString::number(time.tm_hour)+":"+QString::number(time.tm_min)+":"+QString::number(time.tm_sec));

    return ret;
}

void MainWindow::sockListen()
{
    QByteArray buf;

    buf.resize(sock->pendingDatagramSize());
    sock->readDatagram(buf.data(), buf.size());
    sockHeder* hed = (sockHeder*)buf.data();

    switch(hed->tag)
    {
    case 102:{
        report("注册成功！\n");
        break;
    }
    case 101:{ //注册、登录成功
        ui->pushButtonLogin->setEnabled(false);
        ui->pushButtonLogout->setEnabled(true);
        ui->pushButtonRegist->setEnabled(false);
        ui->lineEditUsername->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);

        ui->pushButtonRefreshArena->setEnabled(true);
        ui->pushButtonRefreshFairy->setEnabled(true);
        ui->pushButtonGiveOut->setEnabled(true);
        ui->pushButtonHeal->setEnabled(true);

        report("登录成功！\n");

        requestArena();
        requestSelfFairy();

        break;
    }
    case 105:{ //注销
        ui->pushButtonLogin->setEnabled(true);
        ui->pushButtonLogout->setEnabled(false);
        ui->pushButtonRegist->setEnabled(true);
        ui->lineEditUsername->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);

        ui->pushButtonRefreshArena->setEnabled(false);
        ui->pushButtonRefreshFairy->setEnabled(false);
        ui->pushButtonAttack->setEnabled(false);
        ui->pushButtonGiveOut->setEnabled(false);
        ui->pushButtonHeal->setEnabled(false);

        report("注销成功！\n");

        break;
    }

    case 106:{ //精灵列表
        fairyInfo* fairyptr = (fairyInfo*)(buf.data()+sizeof(sockHeder));
        ui->tableWidgetFairy->setRowCount(hed->cnt);
        ui->tableWidgetFairy->setColumnCount(8);

        for(int i = 0; i < hed->cnt; ++i)
        {
            QStringList info;
            info << QString::fromLocal8Bit(fairyptr->name) << QString::number(fairyptr->adv) << QString::number(fairyptr->level) << QString::number(fairyptr->nowhp, 'f', 2) << QString::number(fairyptr->atk, 'f', 2) << QString::number(fairyptr->def, 'f', 2) << QString::number(fairyptr->hp, 'f', 2) << QString::number(fairyptr->agl, 'f', 2) << QString::number(fairyptr->miss) << QString::number(fairyptr->cri);
            for(int col=0; col < info.size(); ++col)
            {
                if(col != 1) ui->tableWidgetFairy->setItem(i,col,new QTableWidgetItem(info[col]));
                else
                {
                    switch(fairyptr->adv)
                    {
                        case 1:{ui->tableWidgetFairy->setItem(i,col,new QTableWidgetItem("力量")); break;}
                        case 2:{ui->tableWidgetFairy->setItem(i,col,new QTableWidgetItem("肉盾")); break;}
                        case 3:{ui->tableWidgetFairy->setItem(i,col,new QTableWidgetItem("防御")); break;}
                        case 4:{ui->tableWidgetFairy->setItem(i,col,new QTableWidgetItem("敏捷")); break;}
                    }
                }
            }

            ++fairyptr;
        }
        break;
    }

    case 107:{ //决斗场列表
        fairyInfo* fairyptr = (fairyInfo*)(buf.data()+sizeof(sockHeder));
        if(hed->cnt > 0) ui->tableWidgetArena->setRowCount(hed->cnt);
        ui->tableWidgetArena->setColumnCount(8);

        for(int i = 0; i < hed->cnt; ++i)
        {
            QStringList info;
            info << QString::fromLocal8Bit(fairyptr->name) << QString::number(fairyptr->adv) << QString::number(fairyptr->level) << QString::number(fairyptr->nowhp, 'f', 2) << QString::number(fairyptr->atk, 'f', 2) << QString::number(fairyptr->def, 'f', 2) << QString::number(fairyptr->hp, 'f', 2) << QString::number(fairyptr->agl, 'f', 2);
            for(int col=0; col < info.size(); ++col)
            {
                if(col != 1) ui->tableWidgetArena->setItem(i,col,new QTableWidgetItem(info[col]));
                else ui->tableWidgetArena->setItem(i,col,new QTableWidgetItem("怪物"));
            }
            ++fairyptr;
        }
        break;
    }

    case 108:{ //攻击方式
        QString attack = QString::fromLocal8Bit(buf.data()+sizeof(sockHeder));
        report("选择了\""+ui->tableWidgetFairy->selectedItems().first()->text()+"\"其攻击方式为\""+attack+"\"\n");
        ui->pushButtonAttack->setText(attack);
        break;
    }

    case 109:{ //字符串
        char* charptr = (char*)(buf.data()+sizeof(sockHeder));
        report(QString::fromLocal8Bit(charptr));

        requestArena();
        requestSelfFairy();

        break;
    }

    case 110:{ //送出选择的精灵
        report("已送出！\n");
        ui->labelFairy->setText("当前拥有的精灵：");
        requestSelfFairy();
        break;
    }

    case 111:{ //玩家列表
        report("已更新玩家列表！\n");
        infoOnlinePlayer* playerptr = (infoOnlinePlayer*)(buf.data()+sizeof(sockHeder));
        if(hed->cnt > 0) ui->tableWidgetPlayer->setRowCount(hed->cnt);
        ui->tableWidgetPlayer->setColumnCount(3);

        for(int i = 0; i < hed->cnt; ++i)
        {
            QStringList info;

            info << QString::fromLocal8Bit(playerptr->name);
            if(playerptr->lastOpt == 0) info << "离线";
            else info << "在线";
            //QString(time_t2QString(playerptr->lastOpt));
            if(playerptr->totalBattle == 0) info << QString("0%");
            else info << QString::number((double)playerptr->winBattle * 100 / (double)playerptr->totalBattle, 'f', 2) + "%";

            qDebug() << playerptr->winBattle << playerptr->totalBattle;

            for(int col=0; col < info.size(); ++col)
            {
                ui->tableWidgetPlayer->setItem(i,col,new QTableWidgetItem(info[col]));
            }
            ++playerptr;
        }
        break;
    }

    case 112:{
        report("已治疗！\n");
        break;
    }

    case 113:{
        report("已更新徽章信息！\n");
        char* badageptr = buf.data()+sizeof(sockHeder);
        ui->listWidgetBadage->clear();

        for(int i=0;i<hed->cnt;++i)
        {
            ui->listWidgetBadage->addItem(QString::fromLocal8Bit(badageptr));
            badageptr+=32;
        }

        break;
    }

    case 201:{
        report("用户名不存在！\n");
        break;
    }
    case 202:{
        report("密码错误！\n");
        break;
    }
    case 203:{
        report("用户名已存在，注册失败！\n");
        break;
    }

    case 204:{ //有精灵待送出
        char* charptr = (char*)(buf.data()+sizeof(sockHeder));
        ui->labelFairy->setText("待送出的精灵：");
        report(QString::fromLocal8Bit(charptr));
        break;
    }
    }
}

void MainWindow::getAttackName(const int number)
{
    sockHeder hed;
    hed.tag = 6;
    hed.cnt = number;
    hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
    hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

    QByteArray buf;
    buf.append((char*)&hed,sizeof(sockHeder));
    sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sock = new QUdpSocket(this);

    connect(sock, &QUdpSocket::readyRead, this, &MainWindow::sockListen); //监听服务端报文

    connect(ui->lineEditServer, &QLineEdit::textEdited, [=](){ //更改服务器IP地址
        strcpy_s(ADDR_SRV, 32, ui->lineEditServer->text().toStdString().c_str());
    });

    connect(ui->pushButtonRegist, &QPushButton::clicked, [=](){ //注册
        sockHeder hed;
        hed.tag = 1;
        hed.cnt = ui->lineEditUsername->text().toStdString().size();
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));

        buf.append(ui->lineEditUsername->text().toLocal8Bit().data(), ui->lineEditUsername->text().toStdString().size());
        buf.append(QByteArray("\0", 1));

        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonLogin, &QPushButton::clicked, [=](){ //登录
        sockHeder hed;
        hed.tag = 2;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonLogout, &QPushButton::clicked, [=](){ //登出
        sockHeder hed;
        hed.tag = 3;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonRefreshFairy, &QPushButton::clicked, [=](){ // 请求用户精灵列表
        requestSelfFairy();
    });

    connect(ui->pushButtonRefreshArena, &QPushButton::clicked, [=](){ // 请求决斗场列表
        requestArena();
    });

    connect(ui->tableWidgetArena, &QTableWidget::itemSelectionChanged, [=](){ //选择精灵
        report("选择了怪物\"" + ui->tableWidgetArena->selectedItems().first()->text() + "\"\n");

        if(!ui->tableWidgetFairy->selectedItems().empty())
        {
            ui->radioButtonLevel->setEnabled(true);
            ui->radioButtonDeath->setEnabled(true);
        }
    });

    connect(ui->tableWidgetFairy, &QTableWidget::itemSelectionChanged, [=](){
        getAttackName(ui->tableWidgetFairy->row(ui->tableWidgetFairy->selectedItems().first()));
        if(!ui->tableWidgetArena->selectedItems().empty())
        {
            ui->radioButtonLevel->setEnabled(true);
            ui->radioButtonDeath->setEnabled(true);
        }
    });

    connect(ui->radioButtonDeath, &QRadioButton::clicked, [=](){
        ui->radioButtonLevel->setChecked(false);
        ui->pushButtonAttack->setEnabled(true);
    });

    connect(ui->radioButtonLevel, &QRadioButton::clicked, [=](){
        ui->radioButtonDeath->setChecked(false);
        ui->pushButtonAttack->setEnabled(true);
    });

    connect(ui->pushButtonAttack, &QPushButton::clicked, [=](){ //攻击
        sockHeder hed;
        hed.tag = 7;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();
        if(ui->radioButtonLevel->isChecked()) hed.cnt = 0;
        else hed.cnt = 1;

        int choiceEnemy = ui->tableWidgetArena->row(ui->tableWidgetArena->selectedItems().first());
        int choiceFairy = ui->tableWidgetFairy->row(ui->tableWidgetFairy->selectedItems().first());

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        buf.append((char*)&choiceFairy,sizeof(int));
        buf.append((char*)&choiceEnemy,sizeof(int));

        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonGiveOut, &QPushButton::clicked, [=](){
        int choiceFairy = ui->tableWidgetFairy->row(ui->tableWidgetFairy->selectedItems().first());

        sockHeder hed;
        hed.tag = 8;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        buf.append((char*)&choiceFairy,sizeof(int));

        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonRefreshPlayer, &QPushButton::clicked, [=](){ //获取玩家信息
        sockHeder hed;
        hed.tag = 9;
        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

    connect(ui->pushButtonInfoPlayer, &QPushButton::clicked, [=](){ //获取玩家精灵信息
        requestOthersFairy();
    });

    connect(ui->pushButtonHeal, &QPushButton::clicked, [=](){ //治疗
        sockHeder hed;
        hed.tag = 10;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);

        requestSelfFairy();
    });

    connect(ui->pushButtonRefreshBadage,&QPushButton::clicked, [=](){ //徽章
        sockHeder hed;
        hed.tag = 11;
        hed.username = crc32((unsigned char*)ui->lineEditUsername->text().toStdString().c_str(), ui->lineEditUsername->text().toStdString().size()).getCRC();
        hed.password = crc32((unsigned char*)ui->lineEditPassword->text().toStdString().c_str(),ui->lineEditPassword->text().toStdString().size()).getCRC();

        QByteArray buf;
        buf.append((char*)&hed,sizeof(sockHeder));
        sock->writeDatagram(buf, QHostAddress(ADDR_SRV), PORT_SRV);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

