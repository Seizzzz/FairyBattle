#include <stdio.h>
#include "server.h"
#include <WinSock2.h>
#define BUF_SIZE 32768
#pragma comment(lib,"Ws2_32.lib")

#ifdef WIN32  
#pragma execution_character_set("utf-8")
#endif

char buf[BUF_SIZE];
char host[255];

void printBuf(const char* buf, const int conSize)
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

    gethostname(host, sizeof(host));
    hostent* _host = gethostbyname(host);
    for (int i = 0; _host->h_addr_list[i] != NULL; ++i)
    {
        char* p = inet_ntoa(*(struct in_addr*)_host->h_addr_list[i]);
        printf("Host name: %s\n", _host->h_name);
        printf("IP Address: %s\n", p);  //获取全部ip 
    }

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