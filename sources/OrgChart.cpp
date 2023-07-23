#include "OrgChart.hpp"
using namespace ariel;

namespace ariel
{
    // Help function to find a node in orgChart.
    OrgChart::Node *OrgChart::find_node(string const &name)
    {
        queue<Node *> temp_queue;
        temp_queue.push(root);

        while (!temp_queue.empty())
        {
            if (temp_queue.front()->get_data() == name)
            {
                return temp_queue.front();
            }

            for (Node *node : temp_queue.front()->get_children())
            {
                temp_queue.push(node);
            }

            temp_queue.pop();
        }

        throw invalid_argument("Cannot find node");
    }

    vector<OrgChart::Node *> OrgChart::find_nodes(string const &name)
    {
        vector<Node *> nodes;
        queue<Node *> temp_queue;
        temp_queue.push(root);

        while (!temp_queue.empty())
        {
            if (temp_queue.front()->get_data() == name)
            {
                nodes.push_back(temp_queue.front());
            }

            for (Node *node : temp_queue.front()->get_children())
            {
                temp_queue.push(node);
            }

            temp_queue.pop();
        }

        if (nodes.empty())
        {
            throw invalid_argument("Cannot find Father node");
        }

        return nodes;
    };

    // OrgChart Destructor.
    OrgChart::~OrgChart()
    {
        if (!(root == nullptr))
        {
            queue<Node *> temp;
            queue<Node *> _node_queue;

            // cout << _node->get_data() << endl;
            temp.push(root);
            while (!temp.empty())
            {
                _node_queue.push(temp.front());
                for (Node *son : temp.front()->get_children())
                {
                    temp.push(son);
                }
                temp.pop();
            }

            while (!_node_queue.empty())
            {
                Node *curr = _node_queue.front();
                _node_queue.pop();
                delete curr;
            }
        }
    };

    // OrgChart manage functions add root and add sub.
    // add_root(name) : insert a new root node to the orgchart or replace data in current one with name.
    // add_sub(name,name2) : finds name in orgchart nodes and adds name2 node as his son.
    OrgChart &OrgChart::add_root(string const &name)
    {
        if (root == nullptr)
        {
            root = new Node(name);
        }
        else
        {
            root->set_data(name);
        }
        return *this;
    };
    OrgChart &OrgChart::add_sub(string const &name, string const &name2)
    {
        if (root == nullptr)
        {
            throw invalid_argument("OrgChart didnt init yet.");
        }
        if (is_test_mode)
        {
            int x = 0;
            vector<Node *> nodes = find_nodes(name);
            size_t i = 0;

            if (user_test)
            {
                x = (rand()) % (int)nodes.size();
            }
            else
            {
                if (nodes.size() > 1)
                {
                    for (; i < nodes.size(); i++)
                    {
                        // cout << i << " : " << endl
                            //  << nodes[i]->get_data() << endl;
                        nodes[i]->printSubtree(cout, "");
                        // cout << endl;
                    }
                    cout << "Choose one of the above by number\n Your Choice : ";
                    cin >> x;
                }
            }
            Node *father = nodes[(size_t)x];
            father->add_children(new Node(name2));
            return *this;
        }

        Node *father = find_node(name);
        father->add_children(new Node(name2));

        return *this;
    };

    // Ostream function orgchart.
    ostream &operator<<(ostream &os, const OrgChart &org)
    {
        if (org.root != nullptr)
        {
            os << endl;
            os << org.root->get_data() << endl;
            org.root->printSubtree(os, "");
            os << endl;
            return os;
        }

        os << "OrgChart is empty." << endl;
        return os;
    };

    // Orgchart Iteration Functions.
    OrgChart::Iterator OrgChart::begin_level_order()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator(root, Iter_Type::LEVEL);
    };
    OrgChart::Iterator OrgChart::end_level_order()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator();
    };
    OrgChart::Iterator OrgChart::begin_reverse_order()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator(root, Iter_Type::REVERSE);
    };
    OrgChart::Iterator OrgChart::reverse_order()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator();
    };
    OrgChart::Iterator OrgChart::begin_preorder()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator(root, Iter_Type::PREORDER);
    };
    OrgChart::Iterator OrgChart::end_preorder()
    {
        if (root == nullptr)
        {
            throw logic_error("Cannot init iterator from empty chart");
        }
        return Iterator();
    };

    // --------------- Iterator Functions ----- //

    // Help Functions //
    void OrgChart::Iterator::preorder_queue(Node *root)
    {
        _node_queue.push(root->get_data());

        for (Node *n : root->get_children())
        {
            preorder_queue(n);
        }
    };
    void OrgChart::Iterator::init_queue(Iter_Type type)
    {
        queue<Node *> temp;
        if (type == Iter_Type::LEVEL)
        {
            // cout << _node->get_data() << endl;
            temp.push(_node);
            while (!temp.empty())
            {
                _node_queue.push(temp.front()->get_data());
                for (Node *son : temp.front()->get_children())
                {
                    temp.push(son);
                }
                temp.pop();
            }
        }

        else if (type == Iter_Type::REVERSE)
        {
            stack<string> names;
            stack<Node *> revrese_queue;
            temp.push(_node);
            // int i = 0;

            while (!temp.empty())
            {
                // cout << i++ << endl;
                names.push(temp.front()->get_data());
                for (Node *nod : temp.front()->get_children())
                {
                    revrese_queue.push(nod);
                }

                while (!revrese_queue.empty())
                {
                    temp.push(revrese_queue.top());
                    revrese_queue.pop();
                }

                temp.pop();
            }

            while (!names.empty())
            {
                _node_queue.push(names.top());
                names.pop();
            }
        }
        else if (type == Iter_Type::PREORDER)
        {
            preorder_queue(_node);
        }
    }

    // Iterator Constructor, build orgchart iterator by Iter type.
    OrgChart::Iterator::Iterator(Node *root, Iter_Type type)
    {
        _node = root;
        init_queue(type);
    };

    // Iterator operators.
    bool OrgChart::Iterator::operator!=(const OrgChart::Iterator &b)
    {
        return (!this->_node_queue.empty());
    };
    OrgChart::Iterator &OrgChart::Iterator::operator++()
    {
        if (!_node_queue.empty())
        {
            _node_queue.pop();
        }
        return *this;
    };
    OrgChart::Iterator OrgChart::Iterator::operator++(int)
    {
        Iterator temp = *this;
        this->operator++();
        return temp;
    };
    string *OrgChart::Iterator::operator->() { return &(_node_queue.front()); };
    string &OrgChart::Iterator::operator*() { return _node_queue.front(); };

    // ------------- Node Functions ----------- //

    // Node Constructor
    OrgChart::Node::Node(string str) : name(move(str)){};

    // Node Childrens functions get and add.
    vector<OrgChart::Node *> &OrgChart::Node::get_children() { return childrens; };
    void OrgChart::Node::add_children(Node *node) { this->childrens.push_back(node); };

    // set && get data functions return string stored in node.
    void OrgChart::Node::set_data(string str) { this->name = move(str); };
    string &OrgChart::Node::get_data()
    {
        return this->name;
    };

    // Help Function to print Subtree of Node;
    void OrgChart::Node::printSubtree(ostream &os, const string &prefix)
    {
        using std::cout;
        using std::endl;
        if (this->childrens.empty())
        {
            return;
        }
        os << prefix;
        size_t n_children = childrens.size();

        if (n_children > 1)
        {
            os << "├── ";
        }

        for (size_t i = 0; i < n_children; ++i)
        {
            Node *c = childrens[i];
            if (i < n_children - 1)
            {
                if (i > 0)
                {
                    os << prefix << "├── ";
                }
                bool printStrand = n_children > 1 && !c->childrens.empty();
                std::string newPrefix = prefix + (printStrand ? "│" + string(c->name.size() + 1, ' ') : string(c->name.size() + 2, ' '));
                os << c->name << "\n";
                c->printSubtree(os, newPrefix);
            }
            else
            {
                cout << (n_children > 1 ? prefix : "") << "└── ";
                std::cout << c->name << "\n";
                c->printSubtree(os, prefix + string(c->name.size() + 2, ' '));
            }
        }
    }

};
