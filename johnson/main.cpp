#include "graph.h"
#include "johnson.h"


int main () {
    graph g;

    g.add_vertice(1);
    g.add_vertice(2);
    g.add_vertice(3);
    g.add_vertice(4);
    g.add_vertice(5);
    g.add_vertice(6);
    g.add_vertice(7);
    g.add_vertice(8);
    g.add_vertice(9);

    g.add_edge(1, 2, 4);
    g.add_edge(1, 3, 8);
    g.add_edge(2, 4, 8);
    g.add_edge(2, 3, 11);
    g.add_edge(3, 5, 1);
    g.add_edge(4, 6, 2);
    g.add_edge(5, 6, 6);
    g.add_edge(4, 7, 7);
    g.add_edge(4, 8, 4);
    g.add_edge(5, 8, 2);
    g.add_edge(7, 8, 14);
    g.add_edge(4, 7, 2);
    g.add_edge(7, 9, 9);
    g.add_edge(8, 9, 10);

    // g.add_vertice(1);
    // g.add_vertice(2);
    // g.add_vertice(3);
    // g.add_vertice(4);
    // g.add_vertice(5);
    // g.add_vertice(6);

    // g.add_edge(1, 2, 4);
    // g.add_edge(1, 3, 8);
    // g.add_edge(2, 4, 8);
    // g.add_edge(2, 3, 11);
    // g.add_edge(3, 5, 1);
    // g.add_edge(4, 6, 2);
    // g.add_edge(5, 6, 6);

    g.dump_graph("input.dot");

    johnson p(g);
    p.Johnson();

    p.print_all_paths_vertice(*g.find_vertice(9));
    p.print_all_paths_vertice(*g.find_vertice(1));
    p.print_all_paths_vertice(*g.find_vertice(4));

    return 0;
}