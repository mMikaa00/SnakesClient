#pragma once
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#define MAXLINE 4096
#define SA struct sockaddr
#define LISTENQ 5
