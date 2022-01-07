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
/* ключ массива семафоров */
#define SEM_ID1 2001
#define SEM_ID2 2002
/* ключ разделяемой памяти */
#define SHM_ID1 2003
#define SHM_ID2 2004
/* права доступа */
#define PERMS 0666

/*индыксы масива семафоров*/
#define write 1
#define read 0
/* коды сообщений */
#define MSG_TYPE_EMPTY 0    /* пустое сообщение */
#define MSG_TYPE_NO_EMPTY 1 /*не пустое сообщение */

#define MAX_STRING 120
/* структура сообщения, помещаемого в разделяемую память */
typedef struct
{
  int type;
  char string[MAX_STRING];
} message_t;
void clean(char *str, int size)
{
  for (int i = 0; i < size; i++)
  {
    str[i] = '\0';
  }
}
std::string resault(std::string str)
{
  std::string str2 = "";
  int a[2], i = 0, j = 0;
  char op = ' ';

  while (i != str.length() && str[i] != '\0')
  {
    if (isdigit(str[i]))
    {
      while (isdigit(str[i]) && i != str.length())
      {
        str2 += str[i];
        i++;
      }
      a[j] = atoi(str2.c_str());
      j++;
      str2 = "";
    }
    else if (isspace(str[i]))
    {
      while (isspace(str[i]) && i != str.length())
      {

        i++;
      }
    }
    else if (str[i] == '-' || str[i] == '+' || str[i] == '*' || str[i] == '/')
    {
      op = str[i];
      i++;
    }
  }
  if (op == '-')
    return std::to_string(a[0] - a[1]);
  if (op == '*')
    return std::to_string(a[0] * a[1]);
  if (op == '+')
    return std::to_string(a[0] + a[1]);
  if (op == '/')
  {
    if (a[1] == 0)
      return "devision by zero";
    return std::to_string(a[0] / a[1]);
  }
  return "not correct opiration";
}

int main()
{
  int semid[2];
  int shmid[2];
  message_t *msg_p[2];
  char s[MAX_STRING];
  semid[write] = semget(SEM_ID1, 1, PERMS | IPC_CREAT);
  semid[read] = semget(SEM_ID2, 1, PERMS | IPC_CREAT);
  shmid[write] = shmget(SHM_ID1, sizeof(message_t), PERMS | IPC_CREAT);
  shmid[read] = shmget(SHM_ID2, sizeof(message_t), PERMS | IPC_CREAT);
  msg_p[read] = (message_t *)shmat(shmid[read], 0, 0);
  msg_p[write] = (message_t *)shmat(shmid[write], 0, 0);
  semctl(semid[read], 0, SETVAL, 0);
  semctl(semid[write], 0, SETVAL, 0);
  msg_p[read]->type = MSG_TYPE_EMPTY;
  msg_p[write]->type = MSG_TYPE_EMPTY;
  while (1)
  {
    while (semctl(semid[read], 0, GETVAL, 0) || msg_p[read]->type == MSG_TYPE_EMPTY);
    semctl(semid[read], 0, SETVAL, 1);
    std::string str = msg_p[read]->string;
    printf("%s\n", msg_p[read]->string);
    str = resault(str);
    strcpy(s, str.c_str());
    strncpy(msg_p[write]->string, s, MAX_STRING);
    msg_p[read]->type = MSG_TYPE_EMPTY;
    msg_p[write]->type = MSG_TYPE_NO_EMPTY;
    semctl(semid[write], 0, SETVAL, 0);
    semctl(semid[read], 0, SETVAL, 0);
  }
  exit(0);
}