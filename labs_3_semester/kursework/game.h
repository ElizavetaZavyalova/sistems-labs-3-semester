#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#define DATA_SIZE 256 //размер данных

struct Message
{
    long type;      //тип сообщения
    char data[256]; //данные
};
//////////ФУНКЦИИ netserver.cpp ////////////////////////
void game(int &sock, char *read_messege);
//передача данных от сервера к клиенту
int clientgame(int &sock, char *read_messege);
//подключение к серверу

//////////ФУНКЦИИ gameserver.cpp ////////////////////////
void make_coloda(std::map<int, std::pair<std::string, int>> &desk);
//создание колоды карт
void start_game(std::map<int, std::pair<std::string, int>> &desk, std::map<long, std::pair<int, bool>> &players);
//игра пары игроков