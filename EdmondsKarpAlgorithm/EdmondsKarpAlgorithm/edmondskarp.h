//
//  edmondskarp.h
//  EdmondsKarpAlgorithm
//
//  Created by Сергей Миллер on 14.09.15.
//
//

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
