#include "johnson.h"


johnson::johnson(graph G) : G(G) {};

bool johnson::update_distances(std::map<vertice, int>& dist) {
        size_t V = G.graph.size();

        for (size_t i = 0; i < V - 1; i ++) {
            for (vertice& v: G.graph) {
                for (auto& adj : v.adj) {
                    vertice v_adj = *(std::get<0>(adj));
                    int weight    = std::get<1>(adj);

                    if (dist.at(v) != INF_DIST && dist.at(v) + weight < dist.at(v_adj))
                        dist.at(v_adj) = dist.at(v) + weight;
                }
            }
        }

        for (vertice& v: G.graph) {
            for (auto& adj : v.adj) {
                vertice v_adj = *(std::get<0>(adj));
                int weight    = std::get<1>(adj);

                if (dist.at(v) != INF_DIST && dist.at(v) + weight < dist.at(v_adj)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool johnson::check_negative_cycles() {
        std::map<vertice, bool> visited;
        for (vertice v : G.graph)
            visited.insert({v, false});

        std::map<vertice, int> distances;
        for (vertice v : G.graph) {
            if (visited.at(v) == false) {
                if (BellmanFord(v, distances))
                    return true;

                //mark all visited vertices
                for (vertice vistd : G.graph) {
                    if (distances.at(vistd) != INF_DIST)
                        visited.at(vistd) = true;
                }
                distances.clear();
            }
        }
        return false;
    };

    bool johnson::BellmanFord(vertice s, std::map<vertice, int>& distances) {
        for (vertice& v : G.graph)
            distances.insert({v, INF_DIST});
        distances.at(s) = 0;

        return update_distances(distances);
    }

   //find all shortest paths from s to all other vertices
    std::vector<std::vector<size_t>> johnson::Dejkstra(vertice s) {
        std::map<vertice, size_t> dist;
        for (vertice& v : G.graph)
            dist.insert({v, INF_DIST});
        dist.at(s) = 0;

        //for each node contain previous one to build shortest path
        std::map<vertice, vertice>   prev;
        std::vector<vertice> v_deq = G.graph;

        while (!v_deq.empty()) {
            size_t  d_min = INF_DIST;
            vertice v_min(0);

            for (vertice& v : v_deq) {
                if (dist.at(v) <= d_min) {
                    d_min = dist.at(v);
                    v_min = v;
                }
            }

            iter_t min_it = std::find(v_deq.begin(), v_deq.end(), v_min);
            v_deq.erase(min_it);

            for (auto& adj : v_min.adj) {
                vertice v_adj  = *(std::get<0>(adj));
                size_t  weight = static_cast<size_t>(std::get<1>(adj));

                iter_t  adj_it = std::find(v_deq.begin(), v_deq.end(), v_adj);
                if (adj_it != v_deq.end()) {
                    size_t path_weigt = dist.at(v_min) + weight;
                    if (path_weigt < dist.at(v_adj)) {
                        dist.at(v_adj) = path_weigt;

                        if (prev.find(v_adj) != prev.end())
                            prev.at(v_adj) =  v_min;
                        else 
                            prev.insert({v_adj, v_min});
                    }
                }
            }
        }

        // init all paths and return
        std::vector<std::vector<size_t>> paths;

        for (vertice& v : G.graph) {
            std::vector<size_t> path;
            vertice v_prev = v;
            path.push_back(v_prev.key);

            while (v_prev.key != s.key) {
                v_prev = prev.at(v_prev);
                path.push_back(v_prev.key);
            }

            paths.push_back(path);
        }
        return paths;
    }

    void johnson::change_weights(std::map<vertice, int> h_v){
        for (vertice& v : G.graph) {
            for (auto& adj : v.adj) {
                vertice v_adj    = *(std::get<0>(adj)); 

                int weight_add = h_v.at(v) -  h_v.at(v_adj);
                adj.second += weight_add;
            }
        }
        return;
    };

    bool johnson::Johnson() {
        if (check_negative_cycles())
            return false;

        //insert S vertice
        vertice S(S_VERTICE);
        G.graph.push_back(S);

        for (vertice& v : G.graph)
            G.add_edge(v.key, S_VERTICE, 0);

        //G.dump_graph("insert.dot");
        
        std::map<vertice, int> distances;
        if (BellmanFord(*G.find_vertice(S_VERTICE), distances))
            return false;
        change_weights(distances);

        //delete S vertice
        G.graph.pop_back();
        for (vertice& v : G.graph)
            v.adj.pop_back();

        //G.dump_graph("delete.dot");

        for (vertice source : G.graph) {
            if (source.key != S_VERTICE) {
                all_shortest_paths.insert({source, Dejkstra(source)});
            }
        }
        return true;
    }

    void johnson::print_all_paths_vertice(vertice source) {
        if (all_shortest_paths.find(source) == all_shortest_paths.end()) {
            std::cout << "No paths count with source " << source.key << "\n";
            return;
        }
        std::cout << "----------  ORIGIN = " << source.key << " ----------\n";
        std::vector<std::vector<size_t>> paths = all_shortest_paths.at(source);
        for (auto& path : paths) {
            for (std::vector<size_t>::iterator key = path.end() - 1; key >= path.begin(); key --) {
                //does not display paths which contain S vertice
                if (*key == S_VERTICE)
                    continue;

                std::cout << *key << " ";
                if (*key != path.front())
                    std::cout << "--> ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        return;
    };