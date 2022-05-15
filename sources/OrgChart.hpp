#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

namespace ariel {


    class OrgChart {

    /******************************************** Node class *******************************************/
    
        public:

        struct Node {


            vector<Node*> children;
            string data;
            

            Node (){}

            Node(string _data, vector<Node*> _children) 
                    : data(_data), children(_children){};
            
            Node(string _data) : data(_data){};

            string getData() {
                return this->data;
            } 

            ~Node() {}
        

        /****************************************** end Node class *******************************************/

        };

        /****************************************** class Iterator ******************************************/
        enum Order{preoder, level_order, reverse_order};

        class Iterator {

            size_t current_index;
            vector<Node*> tree_list;
           

            void level_order(Node *root) {
                
                if (root==NULL) {
                    return;
                }
            
                queue<Node *> q;  // Create a queue
                q.push(root); // Enqueue root
                while (!q.empty())
                {
                    int n = q.size();
            
                    // If this node has children
                    while (n > 0)
                    {
                        // Dequeue an item from queue and print it
                        Node * p = q.front();
                        q.pop();
                      //cout << p->data << " ";
            
                        // Enqueue all children of the dequeued item
                        for (size_t i=0; i<p->children.size(); i++) {
                            q.push(p->children[i]);
                        }
                        n--;
                    }
                }
            }

            void reverse_order(Node *root) {

                if (root==NULL) {
                    return;
                }

                stack <Node *> S;
                queue <Node *> Q;
                Q.push(root);
            
                // Do something like normal level order traversal order. Following are the
                // differences with normal level order traversal
                // 1) Instead of printing a node, we push the node to stack
                // 2) Right subtree is visited before left subtree
                while (!Q.empty())
                {
                    /* Dequeue node and make it root */
                    root = Q.front();
                    Q.pop();
                    S.push(root);
                    
                    for (size_t i=0; i<root->children.size(); i++) {
                        if (root->children[i] != nullptr) {
                            Q.push(root->children[i]);
                        }
                               // cout << "ROOT->CHILDREN[" << i << "] = " << root->children[i]->data << endl;
                    }
                
                }
            
                // Now pop all items from stack one by one and print them
                while (!S.empty())
                {
                    root = S.top();
                    
                  // cout << root->data << " ";
                    S.pop();
                }
                
            }


            void preorder(Node *root) {
                

                // Stack to store the nodes
                stack<Node*> nodes;
            
                // push the current node onto the stack
                nodes.push(root);
            
                // loop while the stack is not empty
                while (!nodes.empty()) {
            
                    // store the current node and pop it from the stack
                    Node* curr = nodes.top();
                    nodes.pop();
            
                    // current node has been travarsed
                if(curr != NULL)
                {
                //    cout << curr->data << " ";
            
                    // store all the childrent of current node from
                    // right to left.
                    vector<Node*>::iterator it = curr->children.end();
            
                    while (it != curr->children.begin()) {
                        it--;
                        nodes.push(*it);
                    }
                }
                }
                
            }

        public:

            Iterator() {current_index=0;}

            Iterator(Node *root, int order = 0) : current_index(0) {


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

            Iterator& operator++() {
                  if(current_index<tree_list.size()){
                    current_index++;
                    return *this;
                }
                else
                {
                    throw runtime_error("Index out of range");
                }
            }

            Iterator operator++(int) {
                Iterator itr = *this; 
                current_index++; 
                return itr;
            }

            string& operator*() const {
                return tree_list[current_index]->data; 
            }

            string* operator->() const {
                return &(tree_list[current_index]->data); 
            }

            bool operator==(Iterator const &other) const {
                return tree_list[current_index] == other.tree_list[other.current_index]; 
            }

            bool operator!=(Iterator const &other) const { 
                return !(*this==other); 
            }
            void addNode(Node * node) {
                tree_list.push_back(node);
            }

            bool hasNext() {
            if(current_index>=tree_list.size()-1){
                return false;
            }
            return true;
    }
       
    };

        /********************* end of class Iterator ********************/
        
 
        public:
            Node *root;

        public: 
       
            // constructor
            OrgChart() : root(nullptr){};

            // desconstructor
            ~OrgChart() { 
                free_all_nodes(root);
            }

            void free_all_nodes(Node* root){

                for(size_t i = 0; i <root->children.size(); i++ ){
                    free_all_nodes(root->children.at(i));
                }
                delete root;
            }
            
            Node* find_node(Node *n ,const string &val) {
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
                throw invalid_argument("Error");           
            }

            OrgChart& operator=(OrgChart&& other) noexcept {
                root = other.root;
                other.root = nullptr;
                return *this;
            }

            OrgChart& operator=(const OrgChart& other) {
                if (this == &other) {
                    return *this;
                }
                delete(this->root);
                root = new Node(other.root->data);
                return *this;
            }

            OrgChart& add_root( const string& chart_root) { 

                Node *new_root = nullptr;
                new_root = new Node(chart_root);
                root = new_root;
                return *this;
                
            }
          
            OrgChart& add_sub(const string& dad_data, const string& son_data) {


                if(!root){
                    throw std::invalid_argument("Error, there isn't any nodes in the tree!");
                }

                Node *n = find_node(root, dad_data);

                if (n == nullptr) {
                    throw invalid_argument("Error: existing_value is no exist in the tree.\n");
                }
                
                if (root->data == dad_data) {
                    Node *child = new Node(son_data);
                    root->children.push_back(child);
                    return *this;
                }

                queue<Node *> q;  // Create a queue
                q.push(root); // Enqueue root
                while (!q.empty())
                {
                    int n = q.size();
            
                    // If this node has children
                    while (n > 0)
                    {
                        // Dequeue an item from queue and print it
                        Node * p = q.front();
                        q.pop();
                        if (p->data == dad_data) {
                            Node *child = new Node(son_data);
                            p->children.push_back(child);
                            return *this;
                        }
            
                        // Enqueue all children of the dequeued item
                        for (size_t i=0; i<p->children.size(); i++) {
                            q.push(p->children[i]);
                        }
                        n--;
                    }
                }
                
                throw  std::runtime_error("Father not found");
                
            }


            Iterator begin() {
                return Iterator{root};
            }

            Iterator end() {
                return Iterator{nullptr};
            }

            Iterator begin_preorder() { 
                return Iterator{root,1}; 
            }

            Iterator end_preorder() { 
                return Iterator{nullptr,1}; 
            }

            Iterator begin_level_order() {
                return Iterator(root, 0);
            }

            Iterator end_level_order() { 
                return Iterator(nullptr,  0);
            }

            Iterator begin_reverse_order(){ 
                return Iterator(root, 2); 
            }

            Iterator end_reverse_order(){ 
                return Iterator(nullptr, 2);
            }


        friend ostream& operator<<( ostream& out, OrgChart &my_chart){
        // print(my_chart.root, out, "", true);
        
            return out;
        }   

        static void print(Node *root , ostream& out){
            if(!root){
                return;
            }
            out<<root->data<<"#";

            for(Node* n: root->children){
                print(n, out);
            }
        }

    

        static void print(Node *root, ostream& out, string before, bool isLast){

            out << before << "+- " << root->data <<endl;
            //before += (isLast) ? "   " : "|  ";
            if (isLast) {
                before += "   ";
            }else {
            before += "|  ";}
            

            for( size_t i = 0; i < root->children.size(); i++){
                print(root->children.at(i), out, before, i + 1 == root->children.size());
            } 

        }
        
    };

}








  // Iterator begin_level_order() {
            //     Iterator iterator;


            //     if(root== nullptr){
            //         return iterator;
            //     }
            //     queue < Node * > q;  // Create a queue
            //     q.push(root);


            //     while (!q.empty()){
            //         int n = q.size();
            //         // If this node has children
            //         while (n > 0) {
            //             // Dequeue an item from queue and print it
            //             Node *p = q.front();
            //             q.pop();
            //             iterator.addNode(new Node(p->data));

            //             // Enqueue all children of the dequeued item
            //             for (size_t i = 0; i < p->children.size(); i++)
            //                 q.push(p->children[i]);
            //             n--;
            //         }

            //     }


            //   //  iterator.addNode(new Node("END"));
            //     return iterator;
            // }

    
            // Iterator end_level_order() {
                
            //     Iterator iterator;


            //     if(root== nullptr){
            //         return iterator;
            //     }
            //     queue < Node * > q;  // Create a queue
            //     q.push(root);


            //     while (!q.empty()){
            //         int n = q.size();
            //         // If this node has children
            //         while (n > 0) {
            //             // Dequeue an item from queue and print it
            //             Node *p = q.front();
            //             q.pop();
            //             iterator.addNode(new Node(p->data));

            //             // Enqueue all children of the dequeued item
            //             for (size_t i = 0; i < p->children.size(); i++)
            //                 q.push(p->children[i]);
            //             n--;
            //         }

            //     }
            //     while (iterator.hasNext()){
            //         ++iterator;
            //     }
            //     return iterator;
            // }

            // Iterator begin_preorder() {
                
            //     Iterator iterator;
            //     // Stack to store the nodes
            //     stack<Node*> nodes;
            
            //     // push the current node onto the stack
            //     nodes.push(root);
            
            //     // loop while the stack is not empty
            //     while (!nodes.empty()) {
            
            //         // store the current node and pop it from the stack
            //         Node* curr = nodes.top();
            //         nodes.pop();

            
            //         // current node has been travarsed
            //     if(curr != NULL)
            //     {
            //       //  cout << curr->data << " ";
            //         iterator.addNode(new Node(curr->data));
            //         // store all the childrent of current node from
            //         // right to left.
            //         vector<Node*>::iterator it = curr->children.end();
            
            //         while (it != curr->children.begin()) {
            //             it--;
            //             nodes.push(*it);
            //         }
            //     }
            //     }
                
            //     return iterator;
            // }

            // Iterator end_preorder() {
                
            //     Iterator iterator;
            //     // Stack to store the nodes
            //     stack<Node*> nodes;
            
            //     // push the current node onto the stack
            //     nodes.push(root);
            
            //     // loop while the stack is not empty
            //     while (!nodes.empty()) {
            
            //         // store the current node and pop it from the stack
            //         Node* curr = nodes.top();
            //         nodes.pop();

            
            //         // current node has been travarsed
            //     if(curr != NULL)
            //     {
            //       //  cout << curr->data << " ";
            //         iterator.addNode(new Node(curr->data));
            //         // store all the childrent of current node from
            //         // right to left.
            //         vector<Node*>::iterator it = curr->children.end();
            
            //         while (it != curr->children.begin()) {
            //             it--;
            //             nodes.push(*it);
            //         }
            //     }
            //     }

            //      while (iterator.hasNext()){
            //         ++iterator;
            //     }
            //     return iterator;

                
            //     cout << endl;
            // }

         
    //     friend ostream &operator<<(std::ostream &o, OrgChart const &chart) {
    //     if(chart.root== nullptr){
    //         return o;
    //     }
    //     queue < Node * > q;  // Create a queue
    //     q.push(chart.root);


    //     while (!q.empty()){
    //         int n = q.size();
    //         // If this node has children
    //         while (n > 0) {
    //             // Dequeue an item from queue and print it
    //             Node *p = q.front();
    //             q.pop();
    //             o << p->data << " ";

    //             // Enqueue all children of the dequeued item
    //             for (size_t i = 0; i < p->children.size(); i++)
    //                 q.push(p->children[i]);
    //             n--;
    //         }
    //       //  o << endl;
    //     }
        
    //     return o;
    // }