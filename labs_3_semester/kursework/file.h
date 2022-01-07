#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
#include <sys/stat.h>
/////идентификаторы семафоров/////
#define SEM_ID1 2001
#define SEM_ID2 2002
/////идентификаторы разделяемой памяти/////
#define SHM_ID1 2003
#define SHM_ID2 2004
//////права доступа//////
#define PERMS 0666
//////индексы масивов семафоров///////
#define WRITE 1
#define READ 0
///////типы соообщений///////////
#define MSG_TYPE_EMPTY 0    //свободно
#define MSG_TYPE_NO_EMPTY 1 //не свободно
#define GET_FILE 2          //получить файл
#define UPLOAD_FILE 3       //загрузить файл
#define GET_LIST 4          //получить список
///////размеры//////////////
#define NAME_SIZE 120  //имя файла
#define DATA_SIZE 3000 //данные

typedef struct
{
  int type;               //тип сообщения
  char name_f[NAME_SIZE]; //имя файла
  char string[DATA_SIZE]; //данные
} message_t;

//////////ФУНКЦИИ fileserver.cpp ////////////////
void rename(std::string &name);
//переиминование файла
void open_semafory(message_t **msg_p, int *semid);
//создание семафоров и разделяемой памяти
void name_files_in_server(std::string &name);
//возврат списка файлов которые есть на сервере
void get_list_file(message_t **msg_p, int *semid);
////получение списка файлов на сервере
void writing_file(message_t **msg_p, int *semid, std::string &name);
//запись файла на сервер
void upload_file_to_server(message_t **msg_p, int *semid);
//загрузка файлов на сервер
void reding_file(message_t **msg_p, int *semid, std::string &name);
//чтение файла с сервера
void get_file_from_server(message_t **msg_p, int *semid);
//отправка файла коиенту
void fileserver();
//файловый сервер

//////////ФУНКЦИИ netserver.cpp ///////////////
int clientfile(int &sock, char *read_messege);
//общение с файлообменным сервером и клиентом
void get_list(int &sock, char *read_messege, message_t **msg_p, int *semid);
//получение списка файлов на сервере
bool upload_file(int &sock, char *read_messege, message_t **msg_p, int *semid, std::string &name);
//загрузка файла на сервер
void get_file(int &sock, char *read_messege, message_t **msg_p, int *semid, std::string &name);
//Получение файла от сервера
void create_sem(message_t **msg_p, int *semid);
//создание семафоров и разделяемой памяти
