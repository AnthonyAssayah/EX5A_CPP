#include "OrgChart.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

namespace ariel {

        enum Order{preoder, level_order, reverse_order};

        OrgChart::Node::Node(string _data, vector<Node*> _children) : data (_data), children(_children){};
        OrgChart::Node::Node(string _data) : data(_data){};
        OrgChart::Node::~Node(){};


        OrgChart::Iterator OrgChart::Iterator::level_order(Node *root) {
            
            Iterator it;

            if (root==NULL) {
                return it;
            }
        
            queue<Node *> Q;  
            Q.push(root); 
            while (!Q.empty())
            {
                int size = Q.size();
        
                while (size > 0)
                {
                    
                    Node * element = Q.front();
                    Q.pop();
                    cout << element->data << " ";
        
                    for (size_t i=0; i<element->children.size(); i++) {
                        Q.push(element->children[i]);
                    }
                    size--;
                }
            }
            return it;
        }

        void OrgChart::Iterator::reverse_order(Node *root) {

            if (root==NULL) {
                return;
            }

            stack <Node *> S;
            queue <Node *> Q;
            Q.push(root);
        
            while (!Q.empty())
            {
                /* Dequeue node and make it root */
                root = Q.front();
                Q.pop();
                S.push(root);
                
                for (size_t i=0; i<root->children.size(); i++) {
                    if (root->children[root->children.size()-i-1] != nullptr) {
                        Q.push(root->children[root->children.size()-i-1]);
                    }
                }
            }
        
            // Now pop all items from stack one by one and print them
            while (!S.empty())
            {
                root = S.top();
                
                cout << root->data << " ";
                S.pop();
            }
            
        }


        void OrgChart::Iterator::preorder(Node *root) {
            

            stack<Node*> nodes;
        
            nodes.push(root);
        
            while (!nodes.empty()) {
        
                // store the current node and pop it from the stack
                Node* curr = nodes.top();
                nodes.pop();
        
                // current node has been travarsed
            if(curr != NULL)
            {
                cout << curr->data << " ";
        
                // store all the childrent of current node from
                vector<Node*>::iterator it = curr->children.end();
        
                while (it != curr->children.begin()) {
                    it--;
                    nodes.push(*it);
                }
            }
            }
            
        }


        OrgChart::Iterator::Iterator() {current_index=0;}

        OrgChart::Iterator::Iterator(Node *root, int order = 0) : current_index(0) {


            if (order == 0) {
                    level_order(root);
                    tree_list.push_back(nullptr);
            } else if (order == 1) {
                    preorder(root);
                    tree_list.push_back(nullptr);
            } else {
                    reverse_order(root);
                    tree_list.push_back(nullptr);
            }
        }
                //  Iterator(Node *root, Order order) : current_index(0) {


                //     if (order == Order::level_order) {
                //             level_order(root);
                //             tree_list.push_back(nullptr);
                //     } else if (order == Order::preoder) {
                //             preorder(root);
                //             tree_list.push_back(nullptr);
                //     } else {
                //             reverse_order(root);
                //             tree_list.push_back(nullptr);
                //     }
                // }


        OrgChart::Iterator& OrgChart::Iterator::operator++(){
            if(current_index<tree_list.size()){
                current_index++;
                return *this;
            }
            else
            {
                throw invalid_argument("Error, you increment to an index out of range!");
            }
        }


        OrgChart::Iterator OrgChart::Iterator::operator++(int) {
            Iterator itr = *this; 
            current_index++; 
            return itr;
        }


        string& OrgChart::Iterator::operator*() const {
            return tree_list[current_index]->data; 
        }


        string* OrgChart::Iterator::operator->() const {
            return &(tree_list[current_index]->data); 
        }


        bool OrgChart::Iterator::operator==(Iterator const &other) const {
            return tree_list[current_index] == other.tree_list[other.current_index]; 
        }


        bool OrgChart::Iterator::operator!=(Iterator const &other) const { 
            return !(*this==other); 
        }

        void OrgChart::Iterator::addNode(Node * node) {
            tree_list.push_back(node);
        }

        OrgChart::OrgChart() : root(nullptr){};

        OrgChart::~OrgChart() { 
            free_all_nodes(root);
        }


        void OrgChart::free_all_nodes(Node* root){

            for(size_t i = 0; i <root->children.size(); i++ ){
                free_all_nodes(root->children.at(i));
            }
            delete root;
        }
        

        OrgChart::Node* OrgChart::find_node(Node *n ,const string &val) {
            if (n == nullptr || n->data == val) {
                return n;
            }
            if (n == root || root->data == val) {
                return root;
            }
            for (size_t i = 0; i < this->root->children.size(); i++){
                if(this->root->children[i] == n || this->root->children[i]->data == val) {
                    return this->root->children[i];
                }
            }
            throw invalid_argument("Error, there isn't a node with this value in this tree!");           
        }


        OrgChart& OrgChart::add_root( const string& chart_root) { 

            Node *new_root = nullptr;
            new_root = new Node(chart_root);
            root = new_root;
            return *this;
            
        }
        
        
        OrgChart& OrgChart::add_sub(const string& dad_data, const string& son_data) {


            if(!root){
                throw invalid_argument("Error, there isn't any nodes in the tree!");
            }

            Node *n = find_node(root, dad_data);

            if (n == nullptr) {
                throw invalid_argument("Error, this value not exists in the tree!");
            }
            
            if (root->data == dad_data) {
                Node *child = new Node(son_data);
                root->children.push_back(child);
                return *this;
            }

            queue<Node *> Q;  
            Q.push(root); 
            while (!Q.empty())
            {
                int size = Q.size();
        
                // If this node has children
                while (size > 0)
                {
                    // Dequeue an item from queue and print it
                    Node * element = Q.front();
                    Q.pop();
                    if (element->data == dad_data) {
                        Node *child = new Node(son_data);
                        element->children.push_back(child);
                        return *this;
                    }
        
                    // Enqueue all children of the dequeued item
                    for (size_t i=0; i<element->children.size(); i++) {
                        Q.push(element->children[i]);
                    }
                    size--;
                }
            }
            
            throw  invalid_argument("Father not found");
            
        }


        OrgChart::Iterator OrgChart::begin() {
            return Iterator{root};
        }

        OrgChart::Iterator OrgChart::end() {
            return Iterator{nullptr};
        }

        OrgChart::Iterator OrgChart::begin_preorder() { 
            return Iterator(root,1); 
        }

        OrgChart::Iterator OrgChart::end_preorder(){
            return Iterator(nullptr,1);
        };

        OrgChart::Iterator OrgChart::begin_level_order() {
            return Iterator(root, 0);
        }

        OrgChart::Iterator OrgChart::end_level_order() { 
            return Iterator(nullptr,  0);
        }

        OrgChart::Iterator OrgChart::begin_reverse_order(){ 
            return Iterator(root, 2); 
        }

        OrgChart::Iterator OrgChart::end_reverse_order(){ 
            return Iterator(nullptr, 2);
        }


        ostream& operator<<( ostream& out, OrgChart &my_chart){
            return out;
        }   


        
    };


