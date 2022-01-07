#include "net.h"

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
    ;
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
  int sock;
  struct sockaddr_in addr;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    std::cout << "NOT_CONNECT\n";
    return 0;
  }
  while (true)
  {
    char read_massege[DATA_SIZE];
    memset(read_massege, 0, sizeof(char) * (DATA_SIZE - 1));
    std::cout << "Enter the server name 'FILE'\\'GAME'.\n";
    std::cout << "'DISCONNECT' to exit the application.\n";
    std::string send_me = "";
    std::cin >> send_me;
    send(sock, send_me.c_str(), send_me.size(), 0);
    if (send_me == "FILE")
    {
      clientfile(sock, read_massege);
    }
    else if (send_me == "GAME")
    {
      clientgame(sock, read_massege);
    }
    else if (send_me == "DISCONNECT")
    {
      break;
    }
  }
  close(sock);
  return 0;
}

/* структура сообщения, помещаемого в разделяемую память */

int clientfile(int &sock, char *read_massege)
{
  while (true)
  {
    std::cout << "Enter the comand GET <FILENAME> to get file from server\n";
    std::cout << "UPLOAD <FILENAME> to upload file to server\n";
    std::cout << "LIST to show files in server\n QUIT! to quit\n";
    std::string str = "";
    std::cin >> str;
    std::string name = "";
    if (str == "QUIT!")
    {
      send(sock, str.c_str(), str.size(), 0);
      break;
    }
    else if (str == "GET" || str == "UPLOAD")
    {
      std::cin >> name;
    }
    std::string ection = str + "\b" + name;
    send(sock, ection.c_str(), ection.size(), 0);
    if (str == "GET")
    {
      memset(read_massege, 0, DATA_SIZE - 1);
      recv(sock, read_massege, DATA_SIZE, 0);
      rename(name);
      std::string information = read_massege;
      std::ofstream writing(name);
      writing << information;
      writing.close();
    }
    else if (str == "UPLOAD")
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
      send(sock, information.c_str(), information.size(), 0);
    }
    else if (str == "LIST")
    {
      recv(sock, read_massege, DATA_SIZE, 0);
      std::string information = read_massege;
      std::cout << information;
    }
  }
  return 0;
}

int clientgame(int &sock, char *read_massege)
{
  while (true)
  {
    std::string ection = "";
    memset(read_massege, 0, DATA_SIZE - 1);
    std::cout << "enrer what you whant\nSTOP||CONTINU\n";
    while (ection != "STOP" && ection != "CONTINU")
    {
      std::cin >> ection;
    }
    std::cout << "--------WAITING-------\n";
    send(sock, ection.c_str(), ection.size(), 0);
    recv(sock, read_massege, DATA_SIZE, 0);
    std::string str = read_massege;
    std::cout << str;
    if (isdigit(str[0]))
    {
      break;
    }
  }
  return 1;
}
