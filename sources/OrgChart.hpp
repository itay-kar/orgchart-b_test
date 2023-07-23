#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <queue>
#include <stack>
#include <memory>

using namespace std;

namespace ariel
{
    enum Iter_Type
    {
        LEVEL,
        REVERSE,
        PREORDER
    };

    class OrgChart
    {
        // Inner class node;
        class Node
        {
            string name;
            vector<Node *> childrens;

        public:
            Node(){};
            Node(string str);
            void add_father(Node *node);
            void add_children(Node *node);
            string &get_data();
            void set_data(string str);
            vector<Node *> &get_children();
            void printSubtree(ostream &os, const string &prefix);
            friend ostream &operator<<(ostream &os, Node &node)
            {
                os << node.name;
                node.printSubtree(os, " ");
                return os;
            };
        };
        bool user_test=false;
        bool is_test_mode;
        Node *root;
        Node *find_node(string const &name);
        vector<Node *> find_nodes(string const &name);

    public:
        class Iterator;
        // Constructors defualt , move , copy and Destructor functions.
        OrgChart() : is_test_mode(false) { root = nullptr; };
        OrgChart(const string &str)
        {
            if (str == "test")
            {
                is_test_mode = true;
            }

               if (str == "user")
            {
                is_test_mode = true;
                user_test = true;
            }
            root = nullptr;
        };
        ~OrgChart();
        OrgChart(OrgChart &&other) noexcept = default;
        OrgChart(const OrgChart &other)
        {
            root = other.root;
            is_test_mode = false;
        };
        OrgChart &operator=(const OrgChart &other) = default;
        OrgChart &operator=(OrgChart &&other) noexcept = default;

        OrgChart &add_root(string const &name);
        OrgChart &add_sub(string const &name, string const &name2);
        friend ostream &operator<<(ostream &os, const OrgChart &org);

        // OrgChart Iteration functions return class Iterator;
        Iterator begin() { return begin_level_order(); };
        Iterator end() { return end_level_order(); };
        Iterator begin_level_order();
        Iterator end_level_order();
        Iterator begin_reverse_order();
        Iterator reverse_order();
        Iterator begin_preorder();
        Iterator end_preorder();

        /// Inner Class Iterator //
        class Iterator
        {
            queue<string> _node_queue;
            stack<Node *> _node_stack;
            Node *_node;

            // Help Functions to init iterator
            void init_queue(Iter_Type type);
            void preorder_queue(Node *root);

        public:
            Iterator() : _node(nullptr){};
            Iterator(Node *root, Iter_Type type);
            string &operator*();
            bool operator!=(const Iterator &b);
            Iterator &operator++();
            Iterator operator++(int);
            string *operator->();
        };
    };
};