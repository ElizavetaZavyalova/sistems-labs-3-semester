#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

struct Message
{

    long mtype;

    char Data[256];
};

int main(int argc, char **argv)
{
    int id = getpid();
    Message massage, answer;
    key_t key;
    int msgid;
    key = ftok("tmp.txt", 's');
    msgid = msgget(key, 0666 | IPC_CREAT);
    while (true)
    {
        int cofisents[4];
        std::cout << "\nA:";
        std::cin >> cofisents[0];
        std::cout << "\nB:";
        std::cin >> cofisents[1];
        std::cout << "\nC:";
        std::cin >> cofisents[2];
        std::cout << "\nD:";
        std::cin >> cofisents[3];
        std::string ret = "";
        for (int i = 0; i < 4; i++)
        {
            ret += std::to_string(cofisents[i]);
            ret += " ";
        }
        massage.mtype = id;
        strcpy(massage.Data, ret.c_str());
        msgsnd(msgid, (struct msgbuf *)(&massage), strlen(massage.Data) + 1, 0);
        answer.mtype = id;
        msgrcv(msgid, (struct msgbuf *)(&answer), 256, id, 0);
        std::string str(answer.Data);
        std::cout << str;
        std::cout << "\n";
    }
    return 0;
}