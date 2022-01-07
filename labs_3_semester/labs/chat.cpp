#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#define BUFFER_SIZE 256
void parent(const std::string &my_port, const std::string &name, const std::string &ip);
void chield(const std::string &name, const std::string &ip, const std::string &my_port);

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cout << "inter 4 parameters -a xxxx.xxxx.xxxx.xxxx:xxxx -u name\n";
        return 1;
    }
    std::string ip = argv[1];
    std::string name = argv[3];
    if (ip == "-a" && name == "-u")
    {
        ip = argv[2];
        name = argv[4];
    }
    else if (ip == "-u" && name == "-a")
    {
        ip = argv[4];
        name = argv[2];
    }
    else
    {
        std::cout << "inter 4 parameters -a xxxx.xxxx.xxxx.xxxx:xxxx -u name\n";
        return 2;
    }

    std::string my_port = ip;
    my_port.erase(0, ip.rfind(":") + 1);
    ip.erase(ip.rfind(":"), ip.size() - 1);

    int pid = fork();
    if (pid == 0)
    {
        chield(name, ip, my_port); //client
    }
    else
    {
        parent(my_port, name, ip); //server
    }
}

void chield(const std::string &name, const std::string &ip, const std::string &my_port)
{

    int sockCli = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockCli == -1)
    {
        perror("socket");
    }
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(atoi(my_port.c_str()));
    addrSer.sin_addr.s_addr = inet_addr(ip.c_str());
    socklen_t addrlen = sizeof(struct sockaddr);
    while (1)
    {

        std::string send = name + ": ";
        std::string write = "";

        getline(std::cin, write);
        if (write == "quit!")
        {
            addrSer.sin_port = htons(atoi(my_port.c_str()));
            sendto(sockCli, write.c_str(), write.size(), 0, (struct sockaddr *)&addrSer, addrlen);
            break;
        }
        int size = write.size() - 1;
        std::string port = write;
        write.erase(write.size() - 4, 4);
        port.erase(0, port.size() - 4);
        addrSer.sin_port = htons(atoi(port.c_str()));
        send += write;

        sendto(sockCli, send.c_str(), send.size(), 0, (struct sockaddr *)&addrSer, addrlen);
    }

    return;
}

void parent(const std::string &my_port, const std::string &name, const std::string &ip)
{

    int sockSer = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockSer == -1)
        perror("socket");

    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(atoi(my_port.c_str()));
    addrSer.sin_addr.s_addr = inet_addr(ip.c_str());

    socklen_t addrlen = sizeof(struct sockaddr);
    int res = bind(sockSer, (struct sockaddr *)&addrSer, addrlen);
    if (res == -1)
        perror("bind");

    char read_massege[256];
    struct sockaddr_in addrCli;
    while (1)
    {
        memset(read_massege, 0, BUFFER_SIZE - 1);
        recvfrom(sockSer, read_massege, BUFFER_SIZE, 0, (struct sockaddr *)&addrCli, &addrlen);
        std::string write = read_massege;
        if (read_massege[0] == 'q' && read_massege[1] == 'u' && read_massege[2] == 'i' &&
            read_massege[3] == 't' && read_massege[4] == '!' && read_massege[5] == 0)
        {
            break;
        }
        std::cout << write << "\n";
    }
    return;
}
