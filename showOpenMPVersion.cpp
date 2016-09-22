#include <unordered_map>
#include <cstdio>
#include <omp.h>

int main(int argc, char *argv[])
{
  std::unordered_map<unsigned,std::string> map{
    {200505,"2.5"},{200805,"3.0"},{201107,"3.1"},{201307,"4.0"},{201511,"4.5"}};
  printf("We have OpenMP %s.\n", map.at(_OPENMP).c_str());
  return 0;
}
//201307 2013/07版本為4.0
//Ref:http://stackoverflow.com/questions/1304363/how-to-check-the-version-of-openmp-on-linux
