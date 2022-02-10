#pragma once
#include <Windows.h>

extern "C"
{
    void main_program(struct Connection* headConnection, struct Vertex* headVertex, char* sourceVertex);
}

struct Vertex
{
    char* vertexName;
    char* previousVertexName;
    int sumOfWeights;
    bool visited;
    struct Vertex* nextPtr;
};

struct Connection
{
    char* sourceVertex;
    char* destinationVertex;
    int weight;
    struct Connection* nextPtr;
};

struct Vertex* addVertex(char* vName, struct Vertex* headVertex);
struct Connection* addConnection(struct Connection* connection, struct Connection* headConnection);
void cleaner(struct Connection* headConnection, struct Vertex* headVertex);
void printVertices(struct Connection* headConnection, struct Vertex* headVertex, double execTime);
//void mainProgram(struct Connection* headConnection, struct Vertex* headVertex, char* sourceVertex);
void uniqueList(struct Connection* headConnection, struct Vertex* headVertex, char* sourceVertex);
void extractData(struct Connection* headConnection, char* sourceVertex);
extern "C" __declspec(dllexport) void loadData(char* fileName, char* sourceVertex);