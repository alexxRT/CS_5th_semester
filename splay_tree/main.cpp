#include "splay_tree.h"
#include "splay_tree_debug.h"


int main () {
    sptr::SplayTree<int> sp_tree;
    
    sp_tree.insert_binary(sp_tree.get_head(), 10);
    sp_tree.insert_binary(sp_tree.get_head(), 20);
    sp_tree.insert_binary(sp_tree.get_head(), 25);
    sp_tree.insert_binary(sp_tree.get_head(), 15);
    sp_tree.insert_binary(sp_tree.get_head(), 4);
    sp_tree.insert_binary(sp_tree.get_head(), 7);
    sp_tree.insert_binary(sp_tree.get_head(), 8);
    sp_tree.insert_binary(sp_tree.get_head(), 1);
    display(sp_tree.get_head(), "../init.png");

    sp_tree.insert(20);
    display(sp_tree.get_head(), "../after_insert.png");

    Node<int>* found = sp_tree.find(sp_tree.get_head(), 7);
    display(sp_tree.get_head(), "../after_find.png");

    return 0;
}