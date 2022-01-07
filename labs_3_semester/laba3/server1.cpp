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

struct Message
{
    long mtype;
    char Data[256];
};

double croot(double x)
{
    if (x < 0)
        return -pow(-x, 1.0 / 3.0);
    return pow(x, 1.0 / 3.0);
}

std::string cardan(int A, int B, int C, int D)
{

    if (A == 0)
    {

        return "A=0";
    }
    double p = (3.0 * A * C - B * B) / (3.0 * A * A);
    double q = (2.0 * B * B * B - 9.0 * A * B * C + 27.0 * A * A * D) / (27.0 * A * A * A);
    double S = (q * q / 4.0) + (p * p * p / 27.0);
    double F;

    if (q == 0)
        F = M_PI / 2.0;
    else if (q < 0)
        F = atan(-2.0 * sqrt(-S) / q);
    else if (q > 0)
        F = atan(-2.0 * sqrt(-S) / q) + M_PI;
    std::string resault = "";

    if (S < 0)
    {

        resault += std::to_string((2.0 * sqrt(-p / 3.0) * cos(F / 3.0) - B / (3.0 * A)));
        resault += " ";
        resault += std::to_string((2.0 * sqrt(-p / 3.0) * cos((F / 3.0) + 2.0 * M_PI / 3.0) - B / (3.0 * A)));
        resault += " ";
        resault += std::to_string((2.0 * sqrt(-p / 3.0) * cos((F / 3.0) + 4.0 * M_PI / 3.0) - B / (3.0 * A)));
    }
    else if (S == 0)
    {

        resault += std::to_string((2.0 * croot(-q / 2.0) - B / (3.0 * A)));
        resault += " ";
        resault += std::to_string((-croot(-q / 2.0) - B / (3.0 * A)));
        resault += " ";
        resault += std::to_string((-croot(-q / 2.0) - B / (3.0 * A)));
    }
    else if (S > 0)
    {

        double temp1 = croot((-q / 2.0) + sqrt(S)) + croot((-q / 2.0) - sqrt(S));
        double temp2 = croot((-q / 2.0) + sqrt(S)) - croot((-q / 2.0) - sqrt(S));
        resault += std::to_string((temp1 - B / (3.0 * A)));
        resault += " ";
        resault += std::to_string((-temp1 / 2.0 - B / (3.0 * A)));
        resault += "+i*";
        resault += std::to_string((sqrt(3) * temp2 / 2.0));
        resault += " ";
        resault += std::to_string((-temp1 / 2.0 - B / (3.0 * A)));
        resault += "+i*";
        resault += std::to_string((-sqrt(3) * temp2 / 2.0));
    }
    return resault;
}
std::string resault(std::string ret)
{
    int coficents[4];
    int i = 0;
    int j = -1;
    std::string num = "";
    do
    {

        if (ret[i] == ' ')
        {

            i++;
            j++;
            coficents[j] = atoi(num.c_str());
            num = "";
        }
        else
        {
            num += ret[i];
            i++;
        }

    } while (ret[i] != '\0');

    return cardan(coficents[0], coficents[1], coficents[2], coficents[3]);
}

int main(int argc, char **argv)
{
    std::remove("tmp.txt");
    std::ofstream file("tmp.txt");
    file.close();
    key_t key;
    int msgid;
    key = ftok("tmp.txt", 's');
    msgid = msgget(key, 0666 | IPC_CREAT);
    int client = 1;
    Message massage, answer;
    while (true)
    {
        std::string str;
        msgrcv(msgid, (struct msgbuf *)(&answer), 256, 0, 0);
        str = answer.Data;

        str = resault(str);

        strcpy(massage.Data, str.c_str());
        massage.mtype = answer.mtype;
        msgsnd(msgid, (struct msgbuf *)(&massage), strlen(str.c_str()) + 1, 0);
    }
}
