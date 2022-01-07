#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
   bool file = 1;
   std::ofstream out;
   if (argc == 4)
   {
      file = 0;
   }
   else if (argc == 5)
   {
      out.open(argv[4], std::ios::app);
   }
   else
   {
      std::cout << "noorgume\n";
      return 0;
   }
   int num[3]{0, 0, 0};
   num[0] = atoi(argv[1]);
   num[1] = atoi(argv[3]);
   std::string p(argv[2]);
   if (p == "\\")
   {
      if (num[1] == 0)
         out << "we cant make devision by zero\n";
      else
         num[2] = num[0] / num[1];
   }
   else if (p == "*")
   {
      num[2] = num[0] * num[1];
   }
   else if (p == "+")
   {
      num[2] = num[0] + num[1];
   }
   else if (p == "-")
   {
      num[2] = num[0] - num[1];
   }
   else
   {
      out << "no ection";
      return 0;
   }
   if (file)
   {
      out << "=" << num[2] << "\n";
      out.close();
   }
   else
      std::cout << "=" << num[2] << "\n";
   return 0;
}