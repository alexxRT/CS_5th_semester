#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <assert.h>
#include <map>
#include <iostream>

static const int INF_DIST  = 10000000;
static size_t    S_VERTICE = 88;

class vertice {
    public:
        using iter_t = std::vector<vertice>::iterator;
        vertice(size_t key);

        bool operator<(const vertice& v2) const {
            return key < v2.key ? true : false;
        };

        bool operator==(const vertice& v2) const {
            return key == v2.key ? true : false;
        };

        size_t key;
        //vector of adjestement verticies
        std::vector<std::pair<iter_t, int>> adj;
};

class graph {
    public:
        using iter_t = std::vector<vertice>::iterator;
        int add_vertice(size_t key);
        int add_edge(size_t key_first, size_t key_second, int dist);
        void dump_graph(const char* file_path);
        iter_t find_vertice(size_t key);

        std::vector<vertice> graph;
};

#endif //GRAPH_H