//
//  edkarpalg.cpp
//  EdmondsKarpAlgorithm
//
//  Created by Сергей Миллер on 14.09.15.
//
//

//
//  main.cpp
//  EdmondsKarpAlgorithm
//
//  Created by Сергей Миллер on 14.09.15.
//
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>


using std::make_pair;

#ifndef EdmondsKarpAlgorithm_edmonscarp_h
#define EdmondsKarpAlgorithm_edmonscarp_h

#include <vector>
#include <cstdlib>
#include <algorithm>

using std::vector;
using std::pair;

class Graph;
class Network;

class DirectEdge
{
public:
    //points of directed edge
    size_t start;
    size_t finish;
    //value of max flow for edge
    int capacity;
};

class Graph
{
    friend Network;
private:
    //lists of numbers of edges that outgoing and incoming in vertex
    vector <vector <size_t> > *outgoingList = nullptr;
    vector <vector <size_t> > *incomingList = nullptr;
    //full info about edge
    vector <DirectEdge> *edgeList = nullptr;
    //total quantity of verticies and edges
    size_t sizeVert;
    size_t sizeEdge;
public:
    //get graph from list pairs of vertex
    Graph(size_t verticies, vector < pair < pair <size_t,size_t>, int > > &edges);
    
    ~Graph();
};

class Network
{
private:
    Graph *graph;
    //current flow in each edge
    vector <int> *flow;
    //list of vertecies in current path
    vector <size_t> *path;
    //value of summary flow
    long long totalFlow;
    int increaseFlow;
    //sourece and sink in Edmonds-Karp Algorithm
    size_t source;
    size_t sink;
    //get new path in BFS algorithm from source to sink
    bool getNewPath();
public:
    Network(Graph *graph, size_t source, size_t sink);
    ~Network();
    //workfunction
    long long getMaxFlow();
};


#endif

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


int main() {
        std::ifstream cin("input.txt");
    size_t n, m, x, y, capacity;
    std::cin >> n >> m;
    std::vector <pair<pair<size_t,size_t> ,int> > edges;
    for(int i = 0;i < m;++i)
    {
        std::cin >> x >> y >> capacity;
        --x;
        --y;
        edges.push_back(make_pair(make_pair(x,y),capacity));
    }
    
    Graph graph(n, edges);
    
    Network net(&graph, 0, n - 1);
    
    std::cout << net.getMaxFlow() << std::endl;
    
    return 0;
}
