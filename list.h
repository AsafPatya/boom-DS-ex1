#ifndef LIST_H_
#define LIST_H_
#include <cstddef>

template <class T>
class LinkedList {
public:
class Node{
    private:
        //Inserts a node left or right to a node
        void insert(Node *node,bool left){
        if(left){
            node->left = this->left;
            node->right = this;
            if(this->left)
                this->left->right = node;   
            this->left=node;  
        }
        else{
            node->right = this->right;
            node->left = this;
            if(this->right)
                this->right->left = node;   
            this->right=node; 
        }
    }

    public:
        T info;
        Node *left,*right;
        
        Node(T info):info(info),left(0),right(0){}

        Node():info(),left(0),right(0){}

        ~Node(){ }

        void insertLeft(Node *node){
            this->insert(node,true);
        }

        void insertRight(Node *node){
            this->insert(node,false);
        }
        
};
	Node *first,*last;

    //Builds list
	LinkedList() :first(NULL),last(NULL){}

    //Builds list with a node
    LinkedList(Node * firstNode){
        first=firstNode;
        last=firstNode;
    }

    //Builds list with a node containing info
    LinkedList(T info){
        Node* node = new Node (info);
        first=node;
        last=node;
    }

    //Copy constructor for list
    LinkedList(const LinkedList<T>& list){
        first = new Node(*list.first);
        Node* myIt = first;
        for(Node* it=list.first;it->right!=NULL;it=it->right){
            myIt->right =  new Node (*(it->right));
            myIt->right->left = myIt;
            myIt=myIt->right;
        }
        last = myIt;
    }

    //Operator = for list
    LinkedList& operator=(const LinkedList& list) {
        if(*this == list)
            return *this;
        Node* node=this->first;
        first = last = NULL;
		while(node!=0)
        {
            Node* next=node->right;
            delete node;
            node=next;
        }
        first = new Node(*list.first);
        Node* myIt = first;
        for(Node* it=list.first;it->right!=NULL;it=it->right){
            myIt->right =  new Node (*(it->right));
            myIt->right->left = myIt;
            myIt=myIt->right;
        }
        last = myIt;
        return *this;
    }

    //Deletes list
	~LinkedList()
    {
        Node* node=this->first;
        first = last = NULL;
		while(node!=0)
        {
            Node* next=node->right;
            delete node;
            node=next;
        }
	}

    //Inserts a node to the end
    Node* insertNext(T info){
        Node *node = new Node(info);
        if(this->last==0){
            this->last=node;
            this->first=this->last;
        }
        else{
            this->last->right = node;
            node->left = this->last;
            this->last = this->last->right;
        }
        return node;
    }

    //Inserts a node to the start
    Node* insertFirst(T info){
        Node *node = new Node(info);
        if(this->last==0){
            this->last=node;
            this->first=this->last;
        }
        else{
            this->first->left = node;
            node->right = this->first;
            this->first = this->first->left;
        }
        return node;
    }

    //Inserts a new node to the list given his left neighbor
	Node* insertNodeRightTo(T info,Node *leftNeighbor)
	{
        Node *node = new Node(info);
        if(this->first==NULL){
            this->first=node;
            this->last=this->first;
        }
        else if (leftNeighbor==NULL){
            node->right=this->first;
            this->first->left=node;
            this->first=node;
        }
        else if(this->last==leftNeighbor){
            node->left=this->last;
            this->last->right=node;
            this->last=node;
        }
        else{
            node->right = leftNeighbor->right;
            node->left = leftNeighbor;
            leftNeighbor->right->left = node;
            leftNeighbor->right = node;
        }
        return node;
	}

    //Deletes an existing node
    void deleteNode(Node* node){
        if (node==this->first&&node==this->last){
            this->first=NULL;
            this->last=NULL;
        }
        else if(node==this->first){
            this->first=this->first->right;
            this->first->left=NULL;
        }
        else if(node==this->last){
            this->last=this->last->left;
            this->last->right=NULL;
        }
        else {
            node->left->right=node->right;
            node->right->left=node->left;
        }
        delete node;
    }


};
#endif 