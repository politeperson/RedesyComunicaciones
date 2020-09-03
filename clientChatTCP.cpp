/* Client code in C */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
int main(void)
{
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
    char buffer[256];

    if (-1 == SocketFD)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(50001);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

    if (0 > Res)
    {
        perror("error: first parameter is not a valid address family");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
        perror("char string (second parameter does not contain valid ipaddress");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("connect failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    string msg;
    cout << "Bienvenido al chat eres el cliente, escribe un mensaje al servidor\n";
    cout << "para salir del chat escribe [exit]\n";
    for (;;)
    {

        cout << "Write: ";
        getline(cin, msg);
        n = write(SocketFD, msg.c_str(), msg.size());

        bzero(buffer, 256);
        // client wrote exit, so we leave the chat
        if (msg.compare("exit") == 0)
            break;

        /* perform read write operations ... */
        n = read(SocketFD, buffer, 255);
        if (n < 0)
            perror("ERROR reading from socket");
        printf("Server: [%s]\n", buffer);
        // server wrote exit so we leave the chat
        if (strcmp("exit", buffer) == 0)
            break;
    }

    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);
    return 0;
}