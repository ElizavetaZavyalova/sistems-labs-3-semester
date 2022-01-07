#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

void laba5()
{
    dirent *dp; // структура для записи файлов в деректории.
    std::string s = "";
    std::cin >> s;
    DIR *dir_f = opendir(s.c_str()); // ./ текущая деректория.
    std::vector<char *> files;
    while (dp = readdir(dir_f))
    {
        if (dp->d_type != DT_DIR)
        {
            files.push_back((dp->d_name));
        }
    }
    rewinddir(dir_f);
    sort(files.begin(), files.end(), [](char *a, char *b)
         {
             struct stat aa, bb;
             stat(a, &aa);
             stat(b, &bb);
             return aa.st_mtim.tv_sec < bb.st_mtim.tv_sec;
         });

    for (auto i = files.begin(); i != files.end(); i++)
    {
        struct stat aa;
        stat(*i, &aa);

        std::cout << *i << ":\n";
        std::cout << aa.st_nlink << "link\n";
        std::cout << aa.st_size << "size\n";
        std::cout << aa.st_mtim.tv_sec << "time\n";
        std::cout << aa.st_mode << "rools\n";
    }
}

void laba1()
{
    dirent *dp;  

    std::string s = "";
    std::cout << "write name of the dir";
    std::cin >> s;

    DIR *dir_f = opendir(s.c_str());  
    int i = 1;
    while (dp = readdir(dir_f))
    {

        std::cout << i << ":" << dp->d_name << "\n";
        i++;
    }
    rewinddir(dir_f);
}

void laba2()
{

    dirent *dp;  

    std::string s = "";
    std::cout << "write name of the dir";
    std::cin >> s;

    DIR *dir_f = opendir(s.c_str());  
    int i = 1;
    while (dp = readdir(dir_f))
    {
        if (dp->d_type != DT_DIR)
        {
            std::cout << i << ":" << dp->d_name << "\n";
            i++;
        }
    }
    rewinddir(dir_f);
}

void laba3()
{
    std::string name = "";
    std::cout << "write name of the file";
    std::cin >> name;

    struct stat buf;
    if (stat(name.c_str(), &buf) != 0)
    {
        fclose(fopen(name.c_str(), "w"));
    }
    else
    {
        std::string new_name = name;
        std::string rase = name;

        rase.erase(0, rase.rfind(".") + 1);
        new_name.erase(new_name.rfind("."), new_name.size() - 1);
        int number = 0;

        while (stat((new_name + "." + std::to_string(number) + "." + rase).c_str(), &buf) == 0)
        {
            number++;
        }

        link(name.c_str(), (new_name + "." + std::to_string(number) + "." + rase).c_str());
    }
}

void laba4()
{
    std::string name = "";
    std::cout << "write name of the file";
    std::cin >> name;
    struct stat buf;
    if (stat(name.c_str(), &buf) != 0)
    {
        fclose(fopen(name.c_str(), "w"));
    }
    else
    {
        std::string new_name = name;
        new_name += buf.st_nlink + 1 + '0';
        new_name += ".ext";
        symlink(name.c_str(), new_name.c_str());
        name = new_name;
    }
    struct stat aa;
    lstat(name.c_str(), &aa);

    std::cout << name << ":\n";
    std::cout << aa.st_nlink << "link\n";
    std::cout << aa.st_size << "size\n";
    std::cout << aa.st_mtim.tv_sec << "time\n";
    std::cout << aa.st_mode << "rools\n";
}

int main()
{

    while (true)
    {
        std::cout << "chose the ection:\n";
        std::cout << "inter the number ";
        int laba = 0;
        std::cin >> laba;
        if (laba == 1)
        {
            laba1();
        }
        else if (laba == 2)
        {
            laba2();
        }
        else if (laba == 3)
        {
            laba3();
        }
        else if (laba == 4)
        {
            laba4();
        }
        else if (laba == 5)
        {
            laba5();
        }
        else
        {
            break;
        }
    }
    return 1;
}