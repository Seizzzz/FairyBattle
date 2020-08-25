#include <stdio.h>
#include "server.h"
#include <WinSock2.h>
#define BUF_SIZE 32768
#pragma comment(lib,"Ws2_32.lib")

#ifdef WIN32  
#pragma execution_character_set("utf-8")
#endif

char buf[BUF_SIZE];

void printBuf(char* buf, const int conSize)
{
    for (int i = 0; i < conSize; ++i) {
        printf("%2.2x ", (unsigned char)buf[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n");
}

struct sockHeder
{
    short tag;
    short cnt;
    unsigned int username;
    unsigned int password;
};

int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(1, 1), &wsadata);
    ////

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    SOCKADDR_IN addr;
    int sizeAddr = sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(62100);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));

    while (true)
    {
        int sizeRecv = recvfrom(sock, buf, BUF_SIZE, 0, (SOCKADDR*)&addr, &sizeAddr);
        int ret = -1;
        if (sock != INVALID_SOCKET)
        {
            printBuf(buf, sizeRecv);
            ret = Server::getInstance()->assign(buf, sizeRecv, sock, *(SOCKADDR*)&addr);
        }

        sockHeder hed;
        hed.tag = ret;
        memcpy(buf, &ret, sizeof(hed));

        sendto(sock, buf, sizeof(hed), 0, (SOCKADDR*)&addr, sizeof(addr));
    }

    closesocket(sock);
    ////
    WSACleanup();
}