#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
using namespace std;
void rebild(const std::string &str, std::string *s)
{
   int i = 0;
   int t = 0;
   while (i != str.length() && t != 4)
   {
      while (isspace(str[i]) && i != str.length())
      {
         i++;
      }
      if (t == 1 && i != str.length())
      {
         s[t] += str[i];
         i++;
         t++;
         continue;
      }
      if (t != 1 && i != str.length())
      {
         if (str[i] == '-')
         {
            s[t] += str[i];
            i++;
         }
         while (isdigit(str[i]) && i != str.length())
         {
            s[t] = s[t] + str[i];
            i++;
         }
      }
      t++;
   }
}

int main(int argc, char **argv)
{
   std::ifstream in(argv[1]);
   if (argc != 3)
   {
      std::cout << "no orgument";
      return 0;
   }
   std::string str;
   char *buf[100];
   pid_t pid;
   while (!in.eof())
   {
      getline(in, str);
      std::string s[3]{"", "", ""};
      rebild(str, s);
      pid = fork();
      if (pid == 0)
      {
         execl("a", "a", s[0].c_str(), s[1].c_str(), s[2].c_str(), argv[2], NULL);
      }
      else
      {
         waitpid(pid, 0, 0);
      }
   }
   in.close();
   return 0;
}