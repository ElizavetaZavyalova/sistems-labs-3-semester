#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#define maxsize 1024

int sendall(int s, const char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while (total < len)
    {
        n = send(s, buf + total, len - total, flags);
        if (n == -1)
        {
            break;
        }
        total += n;
    }

    return (n == -1 ? -1 : total);
}

int main()
{
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "not connect\n";
        return 0;
    }
    while (true)
    {
        char buf[maxsize];
        memset(buf, 0, sizeof(char) * (maxsize - 1));
        std::cout << "inter the string or quit to quit\n";
        std::string str = "";

        getline(std::cin, str);

        if (str == "quit")
        {
            break;
        }

        send(sock, str.c_str(), str.size(), 0);
        recv(sock, buf, maxsize, 0);
        std::string s;
        s = buf;
        std::cout << "count" << s << "\n";
    }
    close(sock);

    return 0;
}