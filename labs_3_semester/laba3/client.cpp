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

#define SEM_ID 2001 /* ключ массива семафоров */
#define SHM_ID 2002 /* ключ разделяемой памяти */
#define PERMS 0666  /* права доступа */

/* коды сообщений */

#define MSG_TYPE_EMPTY 0  /* пустое сообщение */
#define MSG_TYPE_STRING 1 /* тип сообщения о том, что передана непустая строка */
#define MSG_TYPE_FINISH 2 /* тип сообщения о том, что пора завершать обмен */
#define MAX_STRING 120

/* структура сообщения, помещаемого в разделяемую память */
typedef struct
{
  int type;
  char string[MAX_STRING];
} message_t;

void sys_err(std::string str)
{
  std::cout << str << "\n";
  exit(1);
}

int main()
{
  int semid;        /* идентификатор семафора */
  int shmid;        /* идентификатор разделяемой памяти */
  message_t *msg_p; /* адрес сообщения в разделяемой памяти */
  char s[MAX_STRING];

  /* получение доступа к массиву семафоров */
  if ((semid = semget(SEM_ID, 1, 0)) < 0)
    sys_err("client: can not get semaphore");

  /* получение доступа к сегменту разделяемой памяти */
  if ((shmid = shmget(SHM_ID, sizeof(message_t), 0)) < 0)
    sys_err("client: can not get shared memory segment");

  /* получение адреса сегмента */
  if ((msg_p = (message_t *)shmat(shmid, 0, 0)) == NULL)
    sys_err("client: shared memory attach error");

  while (1)
  {
    scanf("%s", s);
    while (semctl(semid, 0, GETVAL, 0) || msg_p->type != MSG_TYPE_EMPTY)
      /*
         *   если сообщение не обработано или сегмент блокирован - ждать
         *                                                             */
      ;
    semctl(semid, 0, SETVAL, 1); /* блокировать */
    if (strlen(s) != 1)
    {
      /* записать сообщение "печать строки" */
      msg_p->type = MSG_TYPE_STRING;
      strncpy(msg_p->string, s, MAX_STRING);
    }
    else
    {
      /* записать сообщение "завершение работы" */
      msg_p->type = MSG_TYPE_FINISH;
    };
    semctl(semid, 0, SETVAL, 0); /* отменить блокировку */
    if (strlen(s) == 1)
      break;
  }
  shmdt(msg_p); /* отсоединить сегмент разделяемой памяти */
  exit(0);
}