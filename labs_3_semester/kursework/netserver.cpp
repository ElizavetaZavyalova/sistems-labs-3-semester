
#include "file.h"
#include "game.h"
#include "net.h"

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    int bytes_read;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "server is used\n";
    }
    listen(listener, 1);
    while (1)
    {
        sock = accept(listener, NULL, NULL);
        int pid = fork();
        if (pid == 0)
        {
            while (1)
            {
                char read_messege[DATA_SIZE];
                memset(read_messege, 0, sizeof(char) * (DATA_SIZE - 1));
                bytes_read = recv(sock, read_messege, 1024, 0);
                if (bytes_read <= 0)
                {
                    close(sock);
                    return 0;
                }
                std::string write = read_messege;
                if (write == "FILE")
                {
                    clientfile(sock, read_messege);
                }
                else if (write == "GAME")
                {
                    clientgame(sock, read_messege);
                }
            }
        }
        close(sock);
    }
    return 0;
}

void get_list(int &sock, char *read_messege, message_t **msg_p, int *semid)
{
    msg_p[WRITE]->type = GET_LIST;
    semctl(semid[WRITE], 0, SETVAL, 0);
    semctl(semid[READ], 0, SETVAL, 1);
    while (semctl(semid[READ], 0, GETVAL, 0) || msg_p[READ]->type == MSG_TYPE_EMPTY)
        ;
    std::string information = msg_p[READ]->string;
    send(sock, information.c_str(), information.size(), 0);
    msg_p[READ]->type == MSG_TYPE_EMPTY;
    semctl(semid[READ], 0, SETVAL, 0);
}

bool upload_file(int &sock, char *read_messege, message_t **msg_p, int *semid, std::string &name)
{
    msg_p[WRITE]->type = UPLOAD_FILE;
    int bytes_read = recv(sock, read_messege, 1024, 0);
    if (bytes_read <= 0)
    {
        close(sock);
        return 1;
    }
    std::string information = read_messege;
    strncpy(msg_p[WRITE]->string, information.c_str(), DATA_SIZE);
    strncpy(msg_p[WRITE]->name_f, name.c_str(), NAME_SIZE);
    semctl(semid[WRITE], 0, SETVAL, 0);
    msg_p[READ]->type == MSG_TYPE_EMPTY;
    semctl(semid[READ], 0, SETVAL, 0);
    return 0;
}

void get_file(int &sock, char *read_messege, message_t **msg_p, int *semid, std::string &name)
{
    msg_p[WRITE]->type = GET_FILE;
    strncpy(msg_p[WRITE]->name_f, name.c_str(), NAME_SIZE);
    semctl(semid[WRITE], 0, SETVAL, 0);
    msg_p[READ]->type == MSG_TYPE_EMPTY;
    semctl(semid[READ], 0, SETVAL, 1);
    while (semctl(semid[READ], 0, GETVAL, 0) || msg_p[READ]->type == MSG_TYPE_EMPTY)
        ;
    rename(name);
    std::string information = msg_p[READ]->string;
    send(sock, information.c_str(), information.size(), 0);
    msg_p[READ]->type == MSG_TYPE_EMPTY;
    semctl(semid[READ], 0, SETVAL, 0);
}

void create_sem(message_t **msg_p, int *semid)
{
    int shmid[2];
    semid[WRITE] = semget(SEM_ID2, 1, 0);
    semid[READ] = semget(SEM_ID1, 1, 0);
    shmid[WRITE] = shmget(SHM_ID2, sizeof(message_t), 0);
    shmid[READ] = shmget(SHM_ID1, sizeof(message_t), 0);
    msg_p[READ] = (message_t *)shmat(shmid[READ], 0, 0);
    msg_p[WRITE] = (message_t *)shmat(shmid[WRITE], 0, 0);
}

int clientfile(int &sock, char *read_messege)
{
    message_t *msg_p[2];
    int semid[2];
    create_sem(msg_p, semid);
    while (true)
    {
        memset(read_messege, 0, DATA_SIZE - 1);
        int bytes_read = recv(sock, read_messege, 1024, 0);
        if (bytes_read <= 0)
        {
            close(sock);
            return 0;
        }
        std::string write = read_messege;
        std::string str = write;
        std::string name = "";
        if (str == "QUIT!")
        {
            shmdt(msg_p[0]);
            shmdt(msg_p[1]);
            return 0;
        }
        if (str != "LIST")
        {
            name = write;
            str.erase(str.find('\b'), str.size() - 1);
            name.erase(0, name.find('\b') + 1);
        }
        while (semctl(semid[WRITE], 0, GETVAL, 0) || msg_p[WRITE]->type != MSG_TYPE_EMPTY)
            ;
        semctl(semid[WRITE], 0, SETVAL, 1);
        if (str == "GET")
        {
            get_file(sock, read_messege, msg_p, semid, name);
        }
        else if (str == "UPLOAD")
        {
            if (upload_file(sock, read_messege, msg_p, semid, name))
            {
                return 0;
            }
        }
        else if (str == "LIST")
        {
            get_list(sock, read_messege, msg_p, semid);
        }
    }
    return 0;
}

void game(int &sock, char *read_messege)
{
    int id = getpid();
    Message massage, answer;
    key_t key[2];
    int msgid[2];
    key[0] = ftok("write.txt", 's');
    key[1] = ftok("read.txt", 's');
    msgid[0] = msgget(key[0], 0666 | IPC_CREAT);
    msgid[1] = msgget(key[1], 0666 | IPC_CREAT);
    std::string ection = "";
    while (true)
    {
        memset(read_messege, 0, DATA_SIZE - 1);
        int bytes_read = recv(sock, read_messege, 1024, 0);
        if (bytes_read <= 0)
        {
            close(sock);
            return;
        }
        std::string ection = read_messege;
        massage.type = id;
        strcpy(massage.data, ection.c_str());
        msgsnd(msgid[0], (struct msgbuf *)(&massage), strlen(massage.data) + 1, 0);
        answer.type = id;
        msgrcv(msgid[1], (struct msgbuf *)(&answer), 256, id, 0);
        std::string str = answer.data;
        send(sock, str.c_str(), str.size(), 0);
        if (isdigit(str[0]))
        {
            break;
        }
    }
}

int clientgame(int &sock, char *read_messege)
{
    int id = getpid();
    Message massage, answer;
    memset(answer.data, '\0', 255);
    memset(massage.data, '\0', 255);
    key_t key;
    int msgid;
    key = ftok("connect.txt", 's');
    msgid = msgget(key, 0666 | IPC_CREAT);
    massage.type = id;
    msgsnd(msgid, (struct msgbuf *)(&massage), strlen(massage.data) + 1, 0);
    game(sock, read_messege);
    return 0;
}