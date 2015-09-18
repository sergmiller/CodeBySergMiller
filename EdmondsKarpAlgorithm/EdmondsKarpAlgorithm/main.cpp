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
#include "edmondskarp.h"


using std::make_pair;

int main() {
    //std::ifstream cin("input.txt");
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
