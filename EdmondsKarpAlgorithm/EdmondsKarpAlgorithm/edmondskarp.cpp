//
//  edmondskarp.cpp
//  EdmondsKarpAlgorithm
//
//  Created by Сергей Миллер on 14.09.15.
//
//

#include "edmondskarp.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdio>

using std::vector;
using std::pair;
using std::queue;

Graph::Graph(size_t verticies, vector < pair < pair <size_t,size_t>, int> > &edges)
{
    sizeEdge = edges.size();
    sizeVert = verticies;
    DirectEdge curEdge;
    edgeList = new vector <DirectEdge>;
    outgoingList = new vector <vector <size_t> >(sizeVert);
    incomingList = new vector <vector <size_t> >(sizeVert);
    for(size_t i = 0;i < edges.size(); ++i)
    {
        curEdge.start = edges[i].first.first;
        curEdge.finish = edges[i].first.second;
        curEdge.capacity = edges[i].second;
        (*edgeList).push_back(curEdge);
        (*outgoingList)[curEdge.start].push_back(i);
        (*incomingList)[curEdge.finish].push_back(i);
    }
}

Graph::~Graph()
{
    delete edgeList;
    delete incomingList;
    delete outgoingList;
}

Network::Network(Graph *graph, size_t source, size_t sink)
{
    flow = new vector <int>(graph->sizeEdge, 0);
    path = new vector <size_t>;
    this->graph = graph;
    this->source = source;
    this->sink = sink;
}

Network::~Network()
{
    delete flow;
    delete path;
}

long long Network::getMaxFlow()
{
    size_t vert;
    totalFlow = 0;
    DirectEdge curEdge;
    
    while(getNewPath())
    {
        vert = sink;
        for(size_t i = 0;i < path->size();++i)
        {
            curEdge =  (*(graph->edgeList))[(*path)[i]];
            if(vert == curEdge.finish)
            {
                (*flow)[(*path)[i]] += increaseFlow;
                vert = curEdge.start;
            }
            else
            {
                (*flow)[(*path)[i]] -= increaseFlow;
                vert = curEdge.finish;
            }
        }
        totalFlow += increaseFlow;
    }
    
    return totalFlow;
}

//it's BFS algorithm
bool Network::getNewPath()
{
    vector <size_t> prev(graph->sizeVert, -1);
    queue < pair<size_t, int> > bfsQueue;
    increaseFlow = 0;
    size_t vert;
    DirectEdge curEdge;
    size_t numEdge;
    int curFlow;
    
    prev[source] = 0;
    bfsQueue.push(std::make_pair(source, INT32_MAX));
    
    while(!bfsQueue.empty())
    {
        vert = bfsQueue.front().first;
        curFlow = bfsQueue.front().second;
        bfsQueue.pop();
        
        if(vert == sink)
        {
            increaseFlow = curFlow;
            break;
        }
        
        for(size_t i = 0;i < (*(graph->outgoingList))[vert].size();++i)
        {
            numEdge = (*(graph->outgoingList))[vert][i];
            curEdge = (*(graph->edgeList))[numEdge];
            if(prev[curEdge.finish] == -1 && (*flow)[numEdge] < curEdge.capacity)
            {
                prev[curEdge.finish] = numEdge;
                curFlow = std::min(curFlow, curEdge.capacity - (*flow)[numEdge]);
                bfsQueue.push(std::make_pair(curEdge.finish, curFlow));
            }
        }
        
        for(size_t i = 0;i < (*(graph->incomingList))[vert].size();++i)
        {
            numEdge = (*(graph->incomingList))[vert][i];
            curEdge = (*(graph->edgeList))[numEdge];
            if(prev[curEdge.start] == -1 && (*flow)[numEdge] > 0)
            {
                prev[curEdge.start] = numEdge;
                curFlow = std::min(curFlow, (*flow)[numEdge]);
                bfsQueue.push(std::make_pair(curEdge.start, curFlow));
            }
        }
    }
    
    path->clear();
    
    if(prev[sink] == -1)
        return false;
    
    vert = sink;
    
    while(vert != source)
    {
        path->push_back(prev[vert]);
        curEdge = (*(graph->edgeList))[prev[vert]];
        if(curEdge.start == vert)
            vert = curEdge.finish;
        else
            vert = curEdge.start;
    }
    
    return true;
}
