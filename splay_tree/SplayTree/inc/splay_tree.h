#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEAD_ROTATE(node) !node->parent->parent

#define LEFT_SUBTREE_ROTATE(node) node->parent->parent->left_child == node->parent

#define RIGHT_SUBTREE_ROTATE(node) node->parent->parent->right_child == node->parent

#define ZIG_ZIG(node) (node->parent->left_child == node && node->parent->parent->left_child == node->parent) \
|| (node->parent->right_child == node && node->parent->parent->right_child == node->parent)


namespace sptr {

template <typename T>
class Node {
    public:
        Node(T key) : node_key (key) {};

        void print_key() {
            std::cout << "The key value is " << node_key << "\n";
        };

        T get_key() {return node_key; };

        Node<T>* left_child = nullptr;
        Node<T>* right_child = nullptr;
        Node<T>* parent = nullptr;

    private:
        T node_key;
};

template <typename T>
class SplayTree {
    public:
        ~SplayTree() {
            while (tree.size() > 0) {
                delete tree.back();
                tree.pop_back();
            }
        };

        void right_rotate(Node<T>* node) {
            assert(node->parent);
            Node<T>* grandpa = node->parent->parent;

            //swap subtrees
            Node<T>* lc = node->left_child;
            node->left_child = node->parent;
            node->parent->right_child = lc;

            if (lc)
                lc->parent = node->parent;
            
            if(HEAD_ROTATE(node))
                head = node;
            
            else if (LEFT_SUBTREE_ROTATE(node))
                grandpa->left_child = node;

            else if (RIGHT_SUBTREE_ROTATE(node))
                grandpa->right_child = node;

            //swap predicessors
            node->parent->parent = node;
            node->parent = grandpa;
        }

        void left_rotate(Node<T>* node) {
            assert(node->parent);
            Node<T>* grandpa = node->parent->parent;

            //swap subtrees
            Node<T>* rc = node->right_child;
            node->right_child = node->parent;
            node->parent->left_child = rc;

            if (rc)
                rc->parent = node->parent;
            
            if(HEAD_ROTATE(node))
                head = node;
            
            else if (LEFT_SUBTREE_ROTATE(node))
                grandpa->left_child = node;
                
            else if (RIGHT_SUBTREE_ROTATE(node))
                grandpa->right_child = node;

            //swap predicessors
            node->parent->parent = node;
            node->parent = grandpa;
        };

        void rotate_zig_zig(Node<T>* node) {
            // left zig-zig
            if (node == node->parent->left_child) {
                left_rotate(node->parent);
                left_rotate(node);
            }
            // right zig-zig
            else {
                right_rotate(node->parent);
                right_rotate(node);
            }
        };

        void rotate_zig_zag(Node<T>* node) {
            if (node == node->parent->right_child) {
                right_rotate(node);
                left_rotate(node);
            }
            else {
                left_rotate(node);
                right_rotate(node);
            }
        };

        void splay(Node<T>* node) {
            assert(node);

            // do actions whie node is not in head
            while (node->parent) {
                // path to root contains grandpa
                if (node->parent->parent) {
                    if (ZIG_ZIG(node))
                        rotate_zig_zig(node);
                    else
                        rotate_zig_zag(node);
                }
                // simple rotate
                else {
                    if (node == node->parent->left_child)
                        left_rotate(node);
                    else
                        right_rotate(node);
                }
            }

        };

        Node<T>* find(Node<T>* node, T key) {
            if (!node)
                return nullptr;

            if (node->right_child && key > node->get_key())
                return find(node->right_child, key);
    
            if (node->left_child && key < node->get_key())
                return find(node->left_child, key);

            splay(node);
            return head;
        };

        void insert(T key) {
            Node<T>* node = find(head, key);

            if (!node)
                return;

            if (key == node->get_key()) {
                std::cout << "Already inserted!\n";
                return;
            }

            Node<T>* new_node = new Node<T>(key);
            tree.push_back(new_node);

            if (node->get_key() > key) {
                new_node->right_child = node;
                new_node->left_child  = node->left_child;

                node->parent = new_node;
                if (node->left_child)
                    node->left_child->parent = new_node;
            } 
            else {
                new_node->left_child = node;
                new_node->right_child  = node->right_child;

                node->parent = new_node;
                if (node->right_child)
                    node->right_child->parent = new_node;
            }
        }

        // returns head of new merged tree
        // on entry two trees with parent = nullptr
        Node<T>* merge(Node<T>* right, Node<T>* left) {
            if (!right)
                return left;
            if (!left)
                return right;
            
            Node<T>* new_head = find(right, left->get_key());
            std::cout << "found node with key " << new_head->get_key() << "\n";
            assert(!new_head->left_child);

            new_head->parent = nullptr;
            new_head->left_child = left;
            left->parent = new_head;

            return new_head;
        }

        void remove(T key) {
            Node<T>* node = find(head, key);

            if (node->get_key() != key) {
                std::cout << "No node exists with key " << key << "\n";
                return;
            }

            //init two subtrees to merge
            Node<T>* rc = node->right_child;
            if (rc)
                rc->parent = nullptr;

            Node<T>* lc = node->left_child;
            if (lc)
                lc->parent = nullptr;

            head = merge(rc, lc);

            tree.erase(std::find(tree.begin(), tree.end(), node));
            delete node;
        }

        void insert_binary(Node<T>* node, T key) {
            if (is_empty()) {
                head = new Node<T>(key);
                tree.push_back(head);
                return;
            }

            if (node->right_child && key > node->get_key()) {
                insert_binary(node->right_child, key);
                return;
            }

            if (node->left_child && key <= node->get_key()) {
                insert_binary(node->left_child, key);
                return;
            }

            //found node to insert
            Node<T>* new_node = new Node<T>(key);
            tree.push_back(new_node);

            if (!node->right_child && node->left_child) {
                node->right_child = new_node;
                new_node->parent = node;
                return;
            }

            if (!node->left_child && node->right_child) {
                node->left_child = new_node;
                new_node->parent = node;
                return;
            }

            if (key > node->get_key()) {
                node->right_child = new_node;
                new_node->parent = node;
            }
            else {
                node->left_child = new_node;
                new_node->parent = node;
            }
            return;
        };

        bool is_empty() {
            return head == nullptr ? true : false;
        };

        Node<T>* get_head () { return head; };

    private:
        sptr::Node<T>* head = nullptr;
        std::vector<sptr::Node<T>*> tree;
};
}

#endif
