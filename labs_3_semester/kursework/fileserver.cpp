
#include "file.h"

void rename(std::string &name)
{
  struct stat buf;
  if (stat(name.c_str(), &buf) != 0)
  {
    return;
  }
  std::string new_name = name;
  std::string rase = name;
  if (rase.rfind(".") != -1)
  {
    rase.erase(0, rase.rfind(".") + 1);
    new_name.erase(new_name.rfind("."), new_name.size() - 1);
  }
  else
  {

    rase = "";
  }
  int number = 1;
  new_name += ".";
  rase = "." + rase;
  while (stat((new_name + std::to_string(number) + rase).c_str(), &buf) == 0)
  {
    number++;
  }

  name = new_name + std::to_string(number) + rase;
  return;
}

int main()
{
  fileserver();
}

void name_files_in_server(std::string &name)
{
  dirent *dp;
  DIR *dir_f = opendir("FILES");
  int i = 1;
  while (dp = readdir(dir_f))
  {
    if (dp->d_type != DT_DIR)
    {
      name += dp->d_name;
      name += "\n";
    }
  }
  rewinddir(dir_f);
}

void open_semafory(message_t **msg_p, int *semid)
{

  int shmid[2];
  char s[DATA_SIZE];
  semid[WRITE] = semget(SEM_ID1, 1, PERMS | IPC_CREAT);
  semid[READ] = semget(SEM_ID2, 1, PERMS | IPC_CREAT);

  shmid[WRITE] = shmget(SHM_ID1, sizeof(message_t), PERMS | IPC_CREAT);
  shmid[READ] = shmget(SHM_ID2, sizeof(message_t), PERMS | IPC_CREAT);
  msg_p[READ] = (message_t *)shmat(shmid[READ], 0, 0);
  msg_p[WRITE] = (message_t *)shmat(shmid[WRITE], 0, 0);
  semctl(semid[READ], 0, SETVAL, 0);
  semctl(semid[WRITE], 0, SETVAL, 0);
  msg_p[READ]->type = MSG_TYPE_EMPTY;
  msg_p[WRITE]->type = MSG_TYPE_EMPTY;
}
void get_list_file(message_t **msg_p, int *semid)
{
  std::string s;
  name_files_in_server(s);
  strncpy(msg_p[WRITE]->string, s.c_str(), DATA_SIZE);
  msg_p[WRITE]->type = MSG_TYPE_NO_EMPTY;
  msg_p[READ]->type = MSG_TYPE_EMPTY;
  semctl(semid[WRITE], 0, SETVAL, 0);
  semctl(semid[READ], 0, SETVAL, 0);
}
void writing_file(message_t **msg_p, int *semid, std::string &name)
{
  std::string information = msg_p[READ]->string;
  std::ofstream writing(name);
  writing << information;
  writing.close();
}
void upload_file_to_server(message_t **msg_p, int *semid)
{
  std::string name = "FILES/";
  name += msg_p[READ]->name_f;
  rename(name);
  writing_file(msg_p, semid, name);
  msg_p[WRITE]->type = MSG_TYPE_EMPTY;
  msg_p[READ]->type = MSG_TYPE_EMPTY;
  semctl(semid[WRITE], 0, SETVAL, 0);
  semctl(semid[READ], 0, SETVAL, 0);
}
void reding_file(message_t **msg_p, int *semid, std::string &name)
{
  std::ifstream reading(name);
  char c = ' ';
  std::string information = "";
  c = reading.get();
  while (!reading.eof())
  {
    information += c;
    c = reading.get();
  }
  reading.close();
  strncpy(msg_p[WRITE]->string, information.c_str(), DATA_SIZE);
}

void get_file_from_server(message_t **msg_p, int *semid)
{
  std::string name = "FILES/";
  name += msg_p[READ]->name_f;
  reding_file(msg_p, semid, name);

  msg_p[WRITE]->type = MSG_TYPE_NO_EMPTY;
  msg_p[READ]->type = MSG_TYPE_EMPTY;

  semctl(semid[WRITE], 0, SETVAL, 0);
  semctl(semid[READ], 0, SETVAL, 0);
}

void fileserver()
{
  message_t *msg_p[2];
  int semid[2];
  open_semafory(msg_p, semid);
  while (true)
  {
    while (semctl(semid[READ], 0, GETVAL, 0) || msg_p[READ]->type == MSG_TYPE_EMPTY)
      ;

    semctl(semid[READ], 0, SETVAL, 1);
    if (msg_p[READ]->type == GET_LIST)
    {
      get_list_file(msg_p, semid);
    }
    else if (msg_p[READ]->type == UPLOAD_FILE)
    {
      upload_file_to_server(msg_p, semid);
    }
    else if (msg_p[READ]->type == GET_FILE)
    {
      get_file_from_server(msg_p, semid);
    }
  }
  exit(0);
}
