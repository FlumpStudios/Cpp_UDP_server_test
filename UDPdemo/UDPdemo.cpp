#include <iostream>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

#define BUFLEN 512  // max length of answer
#define IP_LENGTH 15

int main()
{
    WSADATA ws;
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed with error Code: %d", WSAGetLastError());
        return 1;
    }

    printf("Initialised successfully.\n");

    sockaddr_in server;
    int soc;
    if ((soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        printf("socket failed with error code: %d", WSAGetLastError());
        return 2;
    }
    u_short requested_port = 0;
    char requested_ip[IP_LENGTH];

    printf("Enter requested IP: ");

    std::cin.getline(requested_ip, IP_LENGTH);

    printf("Enter requested port number:");
    std::cin >> requested_port;

    // setup address structure
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(requested_port);
    server.sin_addr.s_addr = inet_addr(requested_ip);

    std::cin.ignore();

    while (1)
    {
        char message[BUFLEN] = {};
        printf("Enter message: ");
        std::cin.getline(message, BUFLEN);

        if (message)
        {
            if (sendto(soc, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
            {
                printf("sendto() failed with error code: %d \n", WSAGetLastError());
                return 3;
            }
            else
            {
                printf("Posted message to %s on port: %u \n", requested_ip, requested_port);
            }
        }
    }

    closesocket(soc);
    WSACleanup();
}