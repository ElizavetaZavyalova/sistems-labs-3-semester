#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/msg.h>

#define DATA_SIZE 3000 //размер сообщения

////////////////ФАЙЛ//////////////////
void rename(std::string &name);
//переиминование файла
int clientfile(int &sock, char *read_massege);
//общение с файловым сервером
////////////////ИГРА//////////////////
int clientgame(int &sock, char *read_massege);
//игра на сервере
