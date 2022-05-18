#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <list>

using namespace std;

namespace ariel {

    class OrgChart {

        struct Node {

            public:
                /********* variables ***********/
                vector<Node*> children;
                string data;
                /********* Constructors **********/
                Node();
                Node(string _data, vector<Node*> _children);         
                Node(string _data);

                ~Node();

        };
      //  Node *tree;
        

    public:

        class Iterator {

        private:
		    Node* pointer_to_current_node;

            /********* variables ***********/
            size_t current_index;
            vector<Node*> tree_list;
            stack<Node*> nodes;
            queue <Node *> Q;



            public:

                /********* Constructors **********/
                Iterator();
                Iterator(Node *root, int order);

                /********* main functions **********/
                void level_order(Node *root);
                void preorder(Node *root);
                void reverse_order(Node *root);

                /******** Operators functions *******/
                Iterator& operator++();
                Iterator operator++(int);
                string& operator*() const;
                string* operator->() const;
                bool operator==(Iterator const &other) const;
                bool operator!=(Iterator const &other) const;

                /******** Other functions **********/
                void addNode(Node * node);
                // bool hasNext();
       
        };

        public:

            /********* variables **************/
            Node *root;

            /********* Constructors ***********/
            OrgChart();
            ~OrgChart();


            /********* main functions **********/
            void free_all_nodes(Node* root);
            Node* find_node(Node *n ,const string &val);
            OrgChart& add_root( const string& chart_root);
            OrgChart& add_sub(const string& dad_data, const string& son_data);

            /********** Iterator functions ******/
            Iterator begin();
            Iterator end();
            Iterator begin_preorder();
            Iterator end_preorder();
            Iterator begin_level_order();
            Iterator end_level_order();
            Iterator begin_reverse_order();
            Iterator end_reverse_order();

            friend ostream& operator<<( ostream& out, OrgChart &my_chart);

        
    };
}
