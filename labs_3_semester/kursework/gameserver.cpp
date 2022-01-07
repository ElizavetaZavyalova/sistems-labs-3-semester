#include "game.h"

void make_coloda(std::map<int, std::pair<std::string, int>> &desk)
{
    for (int i = 6; i <= 10; i++)
    {
        desk[i - 6] = std::make_pair("PIKY " + std::to_string(i), i);
        desk[i - 6 + 9] = std::make_pair("CHERVY " + std::to_string(i), i);
        desk[i - 6 + 18] = std::make_pair("KRESTY " + std::to_string(i), i);
        desk[i - 6 + 27] = std::make_pair("BUBY " + std::to_string(i), i);
    }
    int j = 5;
    desk[j] = std::make_pair("PIKY TUZ", 11);
    desk[j + 1] = std::make_pair("PIKY VALET", 2);
    desk[j + 2] = std::make_pair("PIKY DAMA", 3);
    desk[j + 3] = std::make_pair("PIKY KYNG", 4);
    j += 9;
    desk[j] = std::make_pair("CHERVY TUZ", 11);
    desk[j + 1] = std::make_pair("CHERVY VALET", 2);
    desk[j + 2] = std::make_pair("CHERVY DAMA", 3);
    desk[j + 3] = std::make_pair("CHERVY KYNG", 4);
    j += 9;
    desk[j] = std::make_pair("KRESTY TUZ", 11);
    desk[j + 1] = std::make_pair("KRESTY VALET", 2);
    desk[j + 2] = std::make_pair("KRESTY DAMA", 3);
    desk[j + 3] = std::make_pair("KRESTY KYNG", 4);
    j += 9;
    desk[j] = std::make_pair("BUBY TUZ", 11);
    desk[j + 1] = std::make_pair("BUBY VALET", 2);
    desk[j + 2] = std::make_pair("BUBY DAMA", 3);
    desk[j + 3] = std::make_pair("BUBY KYNG", 4);
}

void start_game(std::map<int, std::pair<std::string, int>> &desk, std::map<long, std::pair<int, bool>> &players)
{
    srand(getpid());
    key_t key[2];
    int msgid[2];
    key[1] = ftok("read.txt", 's');
    key[0] = ftok("write.txt", 's');
    msgid[0] = msgget(key[0], 0666 | IPC_CREAT);
    msgid[1] = msgget(key[1], 0666 | IPC_CREAT);
    Message massage, answer;
    int end_game = 0;
    while (end_game != 2)
    {
        end_game = 0;
        for (auto i = players.begin(); i != players.end(); i++)
        {
            if (i->second.second == 1)
            {
                end_game++;
                continue;
            }
            std::string str = "", inf;
            msgrcv(msgid[0], (struct msgbuf *)(&answer), 256, i->first, 0);
            inf = answer.data;
            if (inf == "STOP")
            {
                i->second.second = 1;
                continue;
            }
            int number = rand() % desk.size();
            std::pair<std::string, int> card = desk[number];
            desk[number] = desk[desk.size() - 1];
            desk.erase(desk.size() - 1);
            i->second.first += card.second;
            str += card.first;
            str += "\nGIVE YOU ";
            str += std::to_string(card.second);
            str += "\nYOUR POINTS NOW ";
            str += std::to_string(i->second.first);
            str += "\n";
            str += '\0';
            if (i->second.first > 21)
            {
                i->second.second = 1;
            }
            memset(massage.data, '\0', 255);
            strcpy(massage.data, str.c_str());
            massage.type = i->first;
            msgsnd(msgid[1], (struct msgbuf *)(&massage), DATA_SIZE, 0);
            if (i->second.first == 21)
            {
                end_game = 2;
            }
        }
    }
    std::string resaults = "";
    int winner = 0;
    for (auto i = players.begin(); i != players.end(); i++)
    {
        if (i->second.first <= 21)
        {
            if (winner < i->second.first)
            {
                winner = i->second.first;
            }
        }
        resaults += std::to_string(i->second.first);
        resaults += "\n";
    }
    for (auto i = players.begin(); i != players.end(); i++)
    {
        std::string send = resaults;
        if (winner == i->second.first)
        {
            send += "YOU ARE WIN\n\0";
        }
        else
        {
            send += "YOU ARE GAME OVER\n\0";
        }
        memset(massage.data, '\0', 255);
        strcpy(massage.data, send.c_str());
        massage.type = i->first;
        msgsnd(msgid[1], (struct msgbuf *)(&massage), strlen(send.c_str()) + 1, 0);
    }
    return;
}

int main(int argc, char **argv)
{
    std::ifstream connect("connect.txt"), read("read.txt"), write("write.txt");
    connect.close();
    read.close();
    write.close();
    key_t key;
    int msgid;
    key = ftok("connect.txt", 's');
    msgid = msgget(key, 0666 | IPC_CREAT);
    Message massage, answer;
    while (true)
    {
        std::map<int, std::pair<std::string, int>> desk;
        std::map<long, std::pair<int, bool>> players;
        make_coloda(desk);
        while (players.size() != 2)
        {
            std::string str;
            msgrcv(msgid, (struct msgbuf *)(&answer), 256, 0, 0);
            players[answer.type] = std::make_pair(0, 0);
        }
        int pid = fork();
        if (pid == 0)
        {
            start_game(desk, players);
            break;
        }
    }
}