#pragma once

#include "pch.h"
#include <string>
#include <fstream>
#include <iostream>
#include <omp.h>
#include "DijkstraOmpDll.h"

Vertex* addVertex(std::string vName, Vertex* headVertex)
{
    struct Vertex* v = new Vertex;
    (*v).vertexName = vName;
    (*v).previousVertexName = " ";
    (*v).sumOfWeights = 999999;
    (*v).visited = false;
    (*v).vertexId = 0;
    if (headVertex == nullptr)
    {
        headVertex = v;
        headVertex->nextPtr = nullptr;
        return headVertex;
    }
    else
    {
        (*v).nextPtr = headVertex;
        headVertex = v;
        return headVertex;
    }
}

Connection* addConnection(Connection connection, Connection* headConnection)
{
    struct Connection* c = new Connection;
    (*c).sourceVertex = connection.sourceVertex;
    (*c).destinationVertex = connection.destinationVertex;
    (*c).weight = connection.weight;
    if (headConnection == nullptr)
    {
        headConnection = c;
        headConnection->nextPtr = nullptr;
        return headConnection;
    }
    else
    {
        (*c).nextPtr = headConnection;
        headConnection = c;
        return headConnection;
    }
}

void cleaner(Connection* headConnection, Vertex* headVertex)
{
    struct Vertex* v = headVertex, * tv;
    while (v)
    {
        tv = v->nextPtr;
        delete(v);
        v = tv;
    }
    headVertex = nullptr;
    delete(headVertex);
    struct Connection* c = headConnection, * tc;
    while (c)
    {
        tc = c->nextPtr;
        delete(c);
        c = tc;
    }
    headConnection = nullptr;
    delete(headConnection);
}

void printVertices(Connection* headConnection, Vertex* headVertex)
{
    Vertex* v = headVertex;
    while (v)
    {
        std::cout << v->vertexName << " - " << v->previousVertexName << " - " << v->sumOfWeights << std::endl;
        v = v->nextPtr;
    }
    cleaner(headConnection, headVertex);
}

void mainProgram(std::string sourceVertex, Connection* headConnection, Vertex* headVertex)
{
    int myFirstVertex;
    int myLastVertex;
    int threadId;
    int minimumDistance;
    int threadCount;
    int minimumThreadDistance;
    int vertexCounter = 0;
    std::string minimumVertex;
    std::string minimumThreadVertex;
    std::string inspectedVertex;
    struct Connection* cHelper = headConnection;
    struct Vertex* vHelper = headVertex;
    while (vHelper)
    {
        vHelper->vertexId = vertexCounter;
        vertexCounter++;
        if (vHelper->vertexName == sourceVertex)
        {
            vHelper->previousVertexName = "[SOURCE]";
            vHelper->sumOfWeights = 0;
        }
        vHelper = vHelper->nextPtr;
    }
#pragma omp parallel private ( myFirstVertex, myLastVertex, threadId, minimumThreadDistance, minimumThreadVertex, cHelper) \
    shared (vertexCounter, threadCount, headConnection, headVertex, minimumDistance, minimumVertex, sourceVertex, inspectedVertex )
    {
        threadId = omp_get_thread_num();
        threadCount = omp_get_num_threads();
        myFirstVertex = (threadId * vertexCounter / threadCount);
        myLastVertex = ((threadId + 1) * vertexCounter / threadCount);

        if (threadId == threadCount - 1)
            myLastVertex = ((threadId + 1) * vertexCounter / threadCount) - 1;

        for (int i = 0; i < vertexCounter; i++) {
#pragma omp single
            {
                minimumDistance = 999999;
                minimumVertex = "";
            }

            minimumThreadDistance = 999999;
            minimumThreadVertex = "";

            struct Vertex* vH = headVertex;
            struct Vertex* start = headVertex;
            struct Vertex* end = headVertex;
            while (vH) {
                if (vH->vertexId == myFirstVertex) {
                    start = vH;
                }
                if (vH->vertexId == myLastVertex) {
                    end = vH;
                }
                vH = vH->nextPtr;
            }

            while (start != end) {
                if (start->visited == false && start->sumOfWeights < minimumThreadDistance) {
                    minimumThreadDistance = start->sumOfWeights;
                    minimumThreadVertex = start->vertexName;
                }
                start = start->nextPtr;
            }
#pragma omp critical
            {
                if (minimumThreadDistance < minimumDistance) {
                    minimumDistance = minimumThreadDistance;
                    minimumVertex = minimumThreadVertex;
                }
            }
#pragma omp barrier
#pragma omp single
            {
                if (minimumVertex != "") {
                    struct Vertex* vA = headVertex;
                    while (vA) {
                        if (vA->vertexName == minimumVertex) {
                            vA->visited = true;
                            break;
                        }
                        vA = vA->nextPtr;
                    }
                }
            }
#pragma omp barrier
            struct Connection* cA = headConnection;
            while (cA) {
                if (cA->sourceVertex == minimumVertex || cA->destinationVertex == minimumVertex) {
                    if (cA->sourceVertex == minimumVertex) {
                        inspectedVertex = cA->destinationVertex;
                    }
                    if (cA->destinationVertex == minimumVertex) {
                        inspectedVertex = cA->sourceVertex;
                    }
                    Vertex* vB = headVertex;
                    Vertex* startB = headVertex;
                    Vertex* endB = headVertex;
                    while (vB) {
                        if (vB->vertexId == myFirstVertex) {
                            startB = vB;
                        }
                        if (vB->vertexId == myLastVertex) {
                            endB = vB;
                        }
                        vB = vB->nextPtr;
                    }
                    while (startB != endB->nextPtr) {
                        if (startB->vertexName == inspectedVertex &&
                            startB->sumOfWeights > minimumDistance + cA->weight) {
                            startB->previousVertexName = minimumVertex;
                            startB->sumOfWeights = minimumDistance + cA->weight;
                        }
                        startB = startB->nextPtr;
                    }
                }
                cA = cA->nextPtr;
            }
#pragma omp barrier
        }
    }
    printVertices(headConnection, headVertex);
}

void uniqueList(Connection* headConnection, Vertex* headVertex, std::string sourceVertex)
{
    Vertex* v = headVertex;
    Vertex* v2, * vD;
    while (v && v->nextPtr)
    {
        v2 = v;
        while (v2->nextPtr)
        {
            if (v->vertexName == v2->nextPtr->vertexName)
            {
                vD = v2->nextPtr;
                v2->nextPtr = v2->nextPtr->nextPtr;
                free(vD);
                vD = nullptr;
            }
            else v2 = v2->nextPtr;
        }
        v = v->nextPtr;
    }
    mainProgram(sourceVertex, headConnection, headVertex);
}

void extractCities(Connection* headConnection, std::string sourceVertex)
{
    Vertex* headVertex = nullptr;
    struct Connection* c = headConnection;
    while (c)
    {
        headVertex = addVertex(c->sourceVertex, headVertex);
        headVertex = addVertex(c->destinationVertex, headVertex);
        c = c->nextPtr;
    }
    uniqueList(headConnection, headVertex, sourceVertex);
}

void loadDataOmp(std::string fileName, std::string sourceVertex, int number)
{
    omp_set_num_threads(number);
    std::ifstream StreamOpen(fileName);
    Connection c;
    Connection* headConnection = nullptr;
    while (StreamOpen >> c.sourceVertex >> c.destinationVertex >> c.weight && StreamOpen)
    {
        headConnection = addConnection(c, headConnection);
    }
    StreamOpen.close();
    extractCities(headConnection, sourceVertex);
}

int main(int argc, char** argv)
{
    loadDataOmp(argv[1], argv[2], atoi(argv[3]));
    return 0;
}