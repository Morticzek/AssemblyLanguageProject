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

Vertex * addVertex(std::string vName, Vertex * headVertex);
Connection * addConnection(Connection connection, Connection * headConnection);
void cleaner(Connection * headConnection, Vertex * headVertex);
void printVertices(Connection * headConnection, Vertex * headVertex);
void mainProgram(std::string sourceVertex, Connection * headConnection, Vertex * headVertex);
void uniqueList(Connection * headConnection, Vertex * headVertex, std::string sourceVertex);
void extractCities(Connection * headConnection, std::string sourceVertex);
extern "C" __declspec(dllexport) void loadDataOmp(std::string fileName, std::string sourceVertex, int number);