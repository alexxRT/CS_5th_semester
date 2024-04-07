#ifndef SPLAY_TREE_DEBUG
#define SPLAY_TREE_DEBUG

#include "splay_tree.h"

using namespace sptr;

void call_display(const char* file_png, const char* file_gv);

template <typename T>
void dump_tree(Node<T>* node, size_t* n_id, FILE* df) {
    size_t parent_id    = *n_id;
    size_t left_node_id = *n_id + 1;

    if (node->left_child) {
        fprintf(df, "Node%lu[label = \"%d\";];\n", left_node_id, node->left_child->get_key());
        fprintf(df, "Node%lu -> Node%lu\n", parent_id, left_node_id);

        (*n_id) += 1;
        dump_tree(node->left_child, n_id, df);
    }

    if (node->right_child) {
        size_t right_node_id = *n_id + 1;

        fprintf(df, "Node%lu[label = \"%d\";];\n", right_node_id, node->right_child->get_key());
        fprintf(df, "Node%lu -> Node%lu\n", parent_id, right_node_id);

        (*n_id) += 1;
        dump_tree(node->right_child, n_id, df);
    }

    return;
}


template <typename T>
void display(Node<T>* head, const char* file_path) {
    char* file_gv = strdup(file_path);
    char* dot_pos = strchr(file_gv, (int)'.');

    strncpy(dot_pos, ".gv", strlen(".gv") + 1);

    FILE* dump_file = fopen(file_gv, "w");
    fprintf(dump_file, "digraph {\n");
    fprintf(dump_file, "node [ shape=circle ];\n");

    if (!head) {
        fprintf(dump_file, "}\n");
        return;
    }

    size_t node_id = 0;
    fprintf(dump_file, "Node%lu[label=\"%d\";];\n", node_id, head->get_key());

    dump_tree(head, &node_id, dump_file);

    fprintf(dump_file, "}\n");

    fclose(dump_file);

    call_display(file_path, file_gv);
    free(file_gv);
}

#endif