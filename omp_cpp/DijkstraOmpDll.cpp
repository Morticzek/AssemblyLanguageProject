#include "pch.h"
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include <omp.h>
#include <list>
#include "DijkstraOmpDll.h"

char* printVertices(std::list<Vertex> vertList, double time)
{
    char* buffer = (char*)malloc(sizeof(char) * 16384);
    int offset = sprintf(buffer, "Execution Time: %f s\n", time);
    for (auto it = vertList.begin(); it != vertList.end(); it++)
    {
        offset += sprintf(&buffer[offset], "%s - %s - %i\n", it->vertexName.c_str(), it->previousVertexName.c_str(), it->sumOfWeights);
    }
    return buffer;
}

void mainProgram(std::string sourceVertex, std::list<Connection> connectList, std::list<Vertex> &vertList)
{
    int myFirstVertex;
    int myLastVertex;
    int threadId;
    int minimumDistance;
    std::string minimumVertex;
    int threadCount;
    int minimumThreadDistance;
    std::string minimumThreadVertex;
    std::string inspectedVertex;
    int vertexCounter;
    std::list<Connection>::iterator connIter;

    for (auto it = vertList.begin(); it != vertList.end(); it++)
    {
        if (it->vertexName == sourceVertex)
        {
            it->sumOfWeights = 0;
            break;
        }
    }
#pragma omp parallel private ( myFirstVertex, myLastVertex, threadId, minimumThreadDistance, minimumThreadVertex, vertexCounter, connIter ) \
    shared ( threadCount, connectList, vertList, minimumDistance, minimumVertex, sourceVertex, inspectedVertex )
    {
        threadId = omp_get_thread_num();
        threadCount = omp_get_num_threads();
        myFirstVertex = (threadId * vertList.size() / threadCount);
        myLastVertex = ((threadId + 1) * vertList.size() / threadCount);

        for (vertexCounter = 0; vertexCounter < vertList.size(); vertexCounter++)
        {
#pragma omp single
            {
                minimumDistance = INT32_MAX;
                minimumVertex = "";
            }

            minimumThreadDistance = INT32_MAX;
            minimumThreadVertex = "";

            auto iterStart = vertList.begin();
            auto iterEnd = vertList.begin();
            std::advance(iterStart, myFirstVertex);
            std::advance(iterEnd, myLastVertex);

            for (; iterStart != iterEnd; iterStart++)
            {
                if (iterStart->visited == false && iterStart->sumOfWeights < minimumThreadDistance)
                {
                    minimumThreadDistance = iterStart->sumOfWeights;
                    minimumThreadVertex = iterStart->vertexName;
                }
            }
#pragma omp critical
            {
                if (minimumThreadDistance < minimumDistance)
                {
                    minimumDistance = minimumThreadDistance;
                    minimumVertex = minimumThreadVertex;
                }
            }
#pragma omp barrier
#pragma omp single
            {
                if (minimumVertex != "")
                {
                    for (auto vertexIterator = vertList.begin(); vertexIterator != vertList.end(); vertexIterator++)
                    {
                        if (vertexIterator->vertexName == minimumVertex)
                        {
                            vertexIterator->visited = true;
                            break;
                        }
                    }
                }
            }
#pragma omp barrier
            for (connIter = connectList.begin(); connIter != connectList.end(); connIter++)
            {
                if (connIter->sourceVertex == minimumVertex || connIter->destinationVertex == minimumVertex)
                {
                    if (connIter->sourceVertex == minimumVertex)
                    {
                        inspectedVertex = connIter->destinationVertex;
                    }
                    if (connIter->destinationVertex == minimumVertex)
                    {
                        inspectedVertex = connIter->sourceVertex;
                    }
                    auto vertIterStart = vertList.begin();
                    auto vertIterEnd = vertList.begin();
                    std::advance(vertIterStart, myFirstVertex);
                    std::advance(vertIterEnd, myLastVertex);
                    for (; vertIterStart != vertIterEnd; vertIterStart++)
                    {
                        if (vertIterStart->vertexName == inspectedVertex && vertIterStart->sumOfWeights > minimumDistance + connIter->weight)
                        {
                            vertIterStart->previousVertexName = minimumVertex;
                            vertIterStart->sumOfWeights = minimumDistance + connIter->weight;
                        }
                    }
                }
            }
#pragma omp barrier
        }
    }
}

char* extractCities(std::list<Connection> connectList, std::string sourceVertex)
{
    std::list<std::string> nameList;
    for (std::list<Connection>::iterator it = connectList.begin(); it != connectList.end(); it++)
    {
        nameList.push_back(it->sourceVertex);
        nameList.push_back(it->destinationVertex);
    }
    nameList.sort();
    nameList.unique();
    std::list<Vertex> vertList;
    for (std::list<std::string>::iterator it = nameList.begin(); it != nameList.end(); it++)
    {
        Vertex v;
        v.vertexName = *it;
        v.previousVertexName = "";
        v.sumOfWeights = 999999;
        v.visited = false;
        v.nextPtr = nullptr;
        vertList.push_back(v);
    }
    double exec_time = 0.0;
    clock_t begin = clock();
    mainProgram(sourceVertex, connectList, vertList);
    clock_t end = clock();
    exec_time += (double)(end - begin) / CLOCKS_PER_SEC;
    return printVertices(vertList, exec_time);
}

char* loadDataOmp(std::string fileName, std::string sourceVertex, int number)
{
    omp_set_num_threads(number);
    std::ifstream StreamOpen(fileName);
    std::list<Connection> connectList;
    Connection c;
    if (!StreamOpen.is_open())
    {
        std::cout << "Failed to open file" << std::endl;
    }
    else
    {
        while (StreamOpen >> c.sourceVertex >> c.destinationVertex >> c.weight && StreamOpen)
        {
            connectList.push_back(c);
        }
    }
    StreamOpen.close();
    return extractCities(connectList, sourceVertex);
}

int main(int argc, char** argv)
{
    char* x = loadDataOmp(argv[1], argv[2], atoi(argv[3]));
    return 0;
}