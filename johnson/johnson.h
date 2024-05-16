#include "graph.h"

#include <vector>
#include <list>
#include <assert.h>
#include <map>
#include <iostream>

class johnson {
    public:
        johnson(graph G);

        bool BellmanFord(vertice s, std::map<vertice, int>& distances);
        bool Johnson();
        void print_all_paths_vertice(vertice source);

        //simply Belman-Ford for all verticies in G
        bool check_negative_cycles();
        using iter_t = std::vector<vertice>::iterator;

    private:
        bool update_distances(std::map<vertice, int>& dist);
        void change_weights(std::map<vertice, int> h_v);
        std::vector<std::vector<size_t>> Dejkstra(vertice s);

        graph G;
        std::map<vertice, std::vector<std::vector<size_t>>> all_shortest_paths;
};

