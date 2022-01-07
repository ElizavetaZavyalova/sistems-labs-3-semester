#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#define maxsize 1024

int count(char *buf)
{
    int symbol = 0;
    int word = 0;
    while (buf[symbol] != 0)
    {
        if (!isspace(buf[symbol]) && buf[symbol] != 0)
        {

            word++;
            while (!isspace(buf[symbol]) && buf[symbol] != 0)
            {
                symbol++;
            }
        }
        else
        {
            symbol++;
        }
    }

    return word;
}

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
                char buf[maxsize];
                memset(buf, 0, sizeof(char) * (maxsize - 1));
                bytes_read = recv(sock, buf, 1024, 0);
                if (bytes_read <= 0)
                {
                    close(sock);
                    return 0;
                }
                std::string str = std::to_string(count(buf));
                send(sock, str.c_str(), str.size(), 0);
            }
        }
        close(sock);
    }

    return 0;
}