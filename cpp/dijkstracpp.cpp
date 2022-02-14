#include "pch.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <malloc.h>
#include <time.h>
#include "dijkstracpp.h"

struct Vertex* addVertex(char* vName, struct Vertex* headVertex)
{
    const char* space = " ";
    struct Vertex* v = (Vertex*)(malloc(sizeof(struct Vertex)));
    (*v).vertexName = vName;
    (*v).previousVertexName = (char*)space;
    (*v).sumOfWeights = 999999;
    (*v).visited = false;
    if (headVertex == NULL)
    {
        headVertex = v;
        headVertex->nextPtr = NULL;
        return headVertex;
    }
    else
    {
        (*v).nextPtr = headVertex;
        headVertex = v;
        return headVertex;
    }
}

struct Connection* addConnection(struct Connection* connection, struct Connection* headConnection)
{
    struct Connection* c = (Connection*)(malloc(sizeof(struct Connection)));
    (*c).sourceVertex = connection->sourceVertex;
    (*c).destinationVertex = connection->destinationVertex;
    (*c).weight = connection->weight;
    if (headConnection == NULL)
    {
        headConnection = c;
        headConnection->nextPtr = NULL;
        return headConnection;
    }
    else
    {
        (*c).nextPtr = headConnection;
        headConnection = c;
        return headConnection;
    }
}

void cleaner(struct Connection* headConnection, struct Vertex* headVertex)
{
    struct Vertex* v = headVertex, * tv = headVertex;
    while (v)
    {
        tv = v->nextPtr;
        free(v);
        v = tv;
    }
    headVertex = NULL;
    free(headVertex);
    struct Connection* c = headConnection, * tc = headConnection;
    while (c)
    {
        tc = c->nextPtr;
        free(c);
        c = tc;
    }
    headConnection = NULL;
    free(headConnection);
}

char* printVertices(struct Connection* headConnection, struct Vertex* headVertex, double execTime)
{
    struct Vertex* vert = headVertex;
    char* buffer = (char*)malloc(sizeof(char) * 16384);
    int offset = sprintf(buffer, "Execution Time: %f s\n", execTime);
    while (vert)
    {
        offset += sprintf(&buffer[offset], "%s - %s - %i\n", vert->vertexName, vert->previousVertexName, vert->sumOfWeights);
        vert = vert->nextPtr;
    }
    cleaner(headConnection, headVertex);
    return buffer;
}

//void mainProgram(struct Connection* headConnection, struct Vertex* headVertex, char* sourceVertex)
//{
//    const char* space = " ";
//    int minimumDistance;
//    char* minimumVertex;
//    char* inspectedVertex;
//    int x = 0;
//    for(int i = 0; i < 10000; i++)
//    {
//        for(int j = 0; j < 10000; j++)
//        {
//            x+=i;
//        }
//    }
//
//    struct Vertex* v = headVertex;
//    while(v)
//    {
//        if(strcmp(v->vertexName, sourceVertex) == 0)
//        {
//            v->sumOfWeights = 0;
//            break;
//        }
//        else v = v->nextPtr;
//    }
//    struct Vertex* v1 = headVertex;
//    while(v1)
//    {
//        minimumVertex = (char*)space;
//        minimumDistance = 999999;
//        inspectedVertex = (char*)space;
//        struct Vertex* v2 = headVertex;
//        while(v2)
//        {
//            if(v2->visited == false && v2->sumOfWeights < minimumDistance)
//            {
//                minimumDistance = v2->sumOfWeights;
//                minimumVertex = v2->vertexName;
//            }
//            v2 = v2->nextPtr;
//        }
//        if(minimumVertex != (char*)space)
//        {
//            struct Vertex* v3 = headVertex;
//            while(v3)
//            {
//                if(strcmp(v3->vertexName, minimumVertex) == 0)
//                {
//                    v3->visited = true;
//                    break;
//                }
//                v3 = v3->nextPtr;
//            }
//        }
//        struct Connection* c = headConnection;
//        while(c)
//        {
//            if(strcmp(c->sourceVertex, minimumVertex) == 0 || strcmp(c->destinationVertex, minimumVertex) == 0)
//            {
//                if(strcmp(c->sourceVertex, minimumVertex) == 0)
//                {
//                    inspectedVertex = c->destinationVertex;
//                }
//                if(strcmp(c->destinationVertex, minimumVertex) == 0)
//                {
//                    inspectedVertex = c->sourceVertex;
//                }
//                struct Vertex* v4 = headVertex;
//                while(v4)
//                {
//                    if(strcmp(v4->vertexName, inspectedVertex) == 0 && v4->sumOfWeights > minimumDistance + c->weight)
//                    {
//                        v4->previousVertexName = minimumVertex;
//                        v4->sumOfWeights = minimumDistance + c->weight;
//                        break;
//                    }
//                    v4 = v4->nextPtr;
//                }
//            }
//            c = c->nextPtr;
//        }
//        v1 = v1->nextPtr;
//    }
//}

char* uniqueList(struct Connection* headConnection, struct Vertex* headVertex, char* sourceVertex)
{
    struct Vertex* v1, * v2, * vD;
    v1 = headVertex;
    while (v1 && v1->nextPtr)
    {
        v2 = v1;
        while (v2->nextPtr)
        {
            if (strcmp(v1->vertexName, v2->nextPtr->vertexName) == 0)
            {
                vD = v2->nextPtr;
                v2->nextPtr = v2->nextPtr->nextPtr;
                free(vD);
            }
            else v2 = v2->nextPtr;
        }
        v1 = v1->nextPtr;
    }


    double exec_time = 0.0;
    clock_t begin = clock();
    main_program(headConnection, headVertex, sourceVertex);
    clock_t end = clock();
    exec_time += (double)(end - begin) / CLOCKS_PER_SEC;
    return printVertices(headConnection, headVertex, exec_time);
}

char* extractData(struct Connection* headConnection, char* sourceVertex)
{
    struct Connection* c = headConnection;
    struct Vertex* headVertex = NULL;
    while (c)
    {
        headVertex = addVertex(c->sourceVertex, headVertex);
        headVertex = addVertex(c->destinationVertex, headVertex);
        c = c->nextPtr;
    }
    return uniqueList(headConnection, headVertex, sourceVertex);
}


char* loadData(char* fileName, char* sourceVertex)
{
    struct Connection* headConnection = NULL;
    FILE* fileStream = fopen(fileName, "r");
    char line[256];
    char* token;
    char* pEnd;
    while (fgets(line, sizeof(line), fileStream))
    {
        token = strtok(line, " ");
        char* sourceVertex = (char*)(malloc(sizeof(char) * strlen(token) + sizeof(char)));
        strcpy(sourceVertex, token);
        token = strtok(NULL, " ");
        char* destinationVertex = (char*)(malloc(sizeof(char) * strlen(token) + sizeof(char)));
        strcpy(destinationVertex, token);
        token = strtok(NULL, "\n");
        char* distance = (char*)(malloc(sizeof(char) * strlen(token) + sizeof(char)));
        strcpy(distance, token);
        int weight = strtol(distance, &pEnd, 10);
        struct Connection* c = (Connection*)(malloc(sizeof(struct Connection)));
        c->sourceVertex = sourceVertex;
        c->destinationVertex = destinationVertex;
        c->weight = weight;
        headConnection = addConnection(c, headConnection);
    }
    return extractData(headConnection, sourceVertex);
}

int main(int argc, char** argv)
{
    loadData(argv[1], argv[2]);
    return 0;
}