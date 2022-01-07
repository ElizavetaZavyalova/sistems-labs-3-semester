#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <time.h>
#include <unistd.h>

#include <map>
using namespace std;

int main(int argc, char **argv)
{
	int t = 0;
	srand(time(NULL));
	while (true)
	{
		pid_t pid;
		srand(clock());
		int i = rand() % 100;
		int p = 0;
		if (i < 20)
		{
			pid = fork();
			if (pid == 0)
			{

				t += 1;
			}
			else
			{

				waitpid(pid, 0, 0);
			}
		}
		else if (i < 30)
		{
			for (int r = 0; r < 2; r++)
			{
				pid = fork();
				if (pid == 0)
				{
					t++;
					break;
				}
				else
				{

					waitpid(pid, 0, 0);
				}
			}
		}
		else if (i < 40)
		{
			for (int r = 0; r < 3; r++)
			{
				pid = fork();
				if (pid == 0)
				{
					t++;
					break;
				}
				else
				{

					waitpid(pid, 0, 0);
				}
			}
		}
		else
		{
			if (t == 0)
			{
				break;
			}
			ofstream file;
			file.open("tmp.txt", ios_base::app);
			file << t << " ";
			file.close();
			exit(0);
		}
	}
	ifstream read;
	read.open("tmp.txt");
	std::string number = "";
	read >> number;
	std::map<std::string, int> proces;
	proces["0"] = 1;
	while (!read.eof())
	{
		if (proces.find(number) != proces.end())
		{
			proces[number]++;
		}
		else
		{
			proces[number] = 1;
		}
		number = "";
		read >> number;
	}
	for (auto i = proces.begin(); i != proces.end(); i++)
	{
		std::cout << (*i).first << ":";
		for (int j = 0; j < i->second; j++)
		{
			std::cout << "|_|";
		}
		std::cout << "\n";
	}

	std::remove("tmp.txt");
	return 0;
}
