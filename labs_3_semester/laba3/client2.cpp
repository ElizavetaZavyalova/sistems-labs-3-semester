#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>

#define SEM_ID1 2001
#define SEM_ID2 2002 /* ключ массива семафоров */
#define SHM_ID1 2003
#define SHM_ID2 2004 /* ключ разделяемой памяти */ /* ключ разделяемой памяти */
#define PERMS 0666                                 /* права доступа */

/* коды сообщений */
#define write 0
#define read 1
#define MSG_TYPE_EMPTY 0
#define MSG_TYPE_NO_EMPTY 1 /* пустое сообщение */
#define MSG_TYPE_STRING 1   /* тип сообщения о том, что передана непустая строка */
#define MSG_TYPE_FINISH 2   /* тип сообщения о том, что пора завершать обмен */
#define MAX_STRING 120

void clean(char *str, int size)
{
  for (int i = 0; i < size; i++)
  {
    str[i] = '\0';
  }
}
/* структура сообщения, помещаемого в разделяемую память */
typedef struct
{
  int type;
  char string[MAX_STRING];
} message_t;

int main()
{
  int semid[2];
  int shmid[2];
  message_t *msg_p[2];
  char s[MAX_STRING];
  semid[write] = semget(SEM_ID2, 1, 0);
  semid[read] = semget(SEM_ID1, 1, 0);
  shmid[write] = shmget(SHM_ID2, sizeof(message_t), 0);
  shmid[read] = shmget(SHM_ID1, sizeof(message_t), 0);
  msg_p[read] = (message_t *)shmat(shmid[read], 0, 0);
  msg_p[write] = (message_t *)shmat(shmid[write], 0, 0);

  while (true)
  {
    std::string str = "";
    char c = ' ';
    c = std::cin.get();
    if (c == '\n')
    {
      break;
    }
    std::cin >> str;
    str = c + str;
    c = std::cin.get();
    while (semctl(semid[write], 0, GETVAL, 0) || msg_p[write]->type == MSG_TYPE_NO_EMPTY)
      ;
    semctl(semid[write], 0, SETVAL, 1);

    strncpy(msg_p[write]->string, str.c_str(), MAX_STRING);
    msg_p[write]->type = MSG_TYPE_NO_EMPTY;
    semctl(semid[write], 0, SETVAL, 0);
    semctl(semid[read], 0, SETVAL, 1);

    while (semctl(semid[read], 0, GETVAL, 0) || msg_p[read]->type == MSG_TYPE_EMPTY)
      ;

    printf("%s\n", msg_p[read]->string);

    msg_p[read]->type == MSG_TYPE_EMPTY;
    semctl(semid[read], 0, SETVAL, 0);
  }
  shmdt(msg_p);
  exit(0);
}