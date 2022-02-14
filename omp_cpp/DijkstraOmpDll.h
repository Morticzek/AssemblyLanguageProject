#pragma once
#include <Windows.h>

struct Vertex
{
    std::string vertexName;
    std::string previousVertexName;
    int sumOfWeights;
    bool visited;
    int vertexId;
    Vertex* nextPtr;
};

struct Connection
{
    std::string sourceVertex;
    std::string destinationVertex;
    int weight;
    Connection* nextPtr;
};

char* printVertices(std::list<Vertex> vertList, double time);
void mainProgram(std::string sourceVertex, std::list<Connection> connectList, std::list<Vertex> &vertList);
char* extractCities(std::list<Connection> connectList, std::string sourceVertex);
extern "C" __declspec(dllexport) char* loadDataOmp(std::string fileName, std::string sourceVertex, int number);