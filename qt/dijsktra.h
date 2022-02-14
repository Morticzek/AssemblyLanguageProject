#include <iostream>
#include <string>
extern "C"
{
    char* loadDataOmp(std::string fileName, std::string sourceVertex, int number);
    char* loadData(char* fileName, char* sourceVertex);
}
