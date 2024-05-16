#include "graph.h"

vertice::vertice(size_t key) : key(key) {};

int graph::add_vertice(size_t key) {
    iter_t it = find_vertice(key);
    if (it != graph.end() || key == S_VERTICE)
        return -1;
    
    vertice v(key);
    graph.push_back(v);

    return 0;
};

int graph::add_edge(size_t key_first, size_t key_second, int dist) {
    iter_t v_first  = find_vertice(key_first);
    iter_t v_second = find_vertice(key_second);

    // no such vertices exist
    if (v_first  == graph.end() || v_second == graph.end())
        return -1;

    std::pair<iter_t, size_t> p1(v_second, dist);
    v_first->adj.push_back(p1);

    std::pair<iter_t, size_t> p2(v_first, dist);
    v_second->adj.push_back(p2);

    // edge added successfuly
    return 0;
};

void graph::dump_graph(const char* file_path) {
    assert(file_path);
    FILE* file = fopen(file_path, "w");
    if (!file)
        return;
    
    fprintf(file, "strict graph weighted {\n");
    fprintf(file, "node [ shape=circle ];\n");

    for (vertice v : graph)
        fprintf(file, "Node%lu[label=\"%lu\";];\n", v.key, v.key);
    
    //paint edges
    for (vertice v : graph) {
        for (auto& adj : v.adj) {
            iter_t v_adj  = std::get<0>(adj);
            int    weight = std::get<1>(adj);
            fprintf(file, "Node%lu -- Node%lu [label=\"%d\"; len=2];\n", v.key, (*v_adj).key, weight);
        }
    }
    fprintf(file, "}\n");
    fclose(file);
};

graph::iter_t graph::find_vertice(size_t key) {
    graph::iter_t it;
    for (it = graph.begin(); it < graph.end(); it ++) {
        if ((*it).key == key)
            return it;
    }

    return it;
};

