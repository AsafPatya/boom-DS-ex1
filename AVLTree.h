#ifndef AVLTREE_H_
#define AVLTREE_H_
#include <algorithm>

//Class that implements an AVL tree.
template <class T, class Compare>
class AVLTree
{
public: 

class Node {
private:

	//Inserts node left or right to his parent and balances the tree
    void insert(Node *node,bool left){
        node->parent=this;
        if(left)
            this->left=node;
        else
            this->right = node;
        this->updateHeight();
        balance();
    }

	//Updates the height of this node
	void updateHeight(){
        this->h= std::max(getHeight(this->right),getHeight(this->left))+1;
    }

	//Returns the balance factor of a node
	int bf(){
        return getHeight(this->left)-getHeight(this->right);
    }

	//Balances the tree
    void balance(){
        int bf = this->bf();
        if(bf>1){
            if(this->left->bf()<0)
                LRRotate();
            else
                LLRotate();
        }
        else if(bf<-1){
            if(this->right->bf()>0)
                RLRotate();
            else
                RRRotate();
        }
    }

	void LLRotate(){
		Node *Root=this;
		Node *LeftSon=left;
		Node *RightSon=right;
		Node *LeftLeftSon=LeftSon->left;
		Node *LeftRightSon=LeftSon->right;
		LeftSon->swapInfo(Root);
		LeftSon->left=LeftRightSon;
		if(LeftSon->left)
			LeftSon->left->parent=LeftSon;
		LeftSon->right=RightSon;
		if(LeftSon->right)
			LeftSon->right->parent=LeftSon;
		Root->right=LeftSon;
		Root->left=LeftLeftSon;
		if(Root->left)
			Root->left->parent=Root;
		LeftSon->updateHeight();
	}

	void RRRotate(){
		Node *Root=this;
		Node *RightSon=right;
		Node *Rootl=left;
		Node *RightLeftSon=RightSon->left;
		Node *RightRightSon=RightSon->right;
		RightSon->swapInfo(Root);
		RightSon->right=RightLeftSon;
		if(RightSon->right!=0)
			RightSon->right->parent=RightSon;
		RightSon->left=Rootl;
		if(RightSon->left!=0)
			RightSon->left->parent=RightSon;
		Root->left=RightSon;
		Root->right=RightRightSon;
		if(Root->right!=0)
			Root->right->parent=Root;
		RightSon->updateHeight();
	}

    void LRRotate(){
		this->left->RRRotate();
		this->LLRotate();
	}

	void RLRotate(){
		this->right->LLRotate();
		this->RRRotate();
	}

public:

	T info;
    Node *parent,*left,*right;
    int h;

    Node(T info):info(info),parent(0),left(0),right(0),h(0){}

    void insertLeft(Node *node){
		insert(node,true);
	}

	void insertRight(Node *node){
		insert(node,false);
	}

    void DeleteSubtree(){
		if(this->left){
			this->left->DeleteSubtree();
			delete this->left;
		}
		if(this->right){
			this->right->DeleteSubtree();
			delete this->right;
		}
	}

    Node* getMostLeft(){
		Node* node=this;
		while(node->left != NULL)
			node = node->left;
		return node;
	}

    Node* getMostRight(){
		Node* node=this;
		while(node->right != NULL)
			node = node->right;
		return node;
	}

    int getHeight(Node *node){
        if(!node)
            return 0;
        return node->h;
    }

	//Swaps the info between nodes
    void swapInfo(Node *node){
        T temp = this->info;
        this->info=node->info;
        node->info=temp;
    }

	//Returns the next smaller node in order
	Node* getNextSmaller(){
		if(left != 0)
			return left->getMostRight();
		for(Node* node = this; node->parent != 0; node = node->parent){
			if(node == node->parent->right){
				return node->parent;
			}
		}
		return 0; 
	}
	
	//Balances the tree and updates the heights
	void updateParentHeights(){
        this->balance();
        this->updateHeight();
        if(this->parent!=0)
            this->parent->updateParentHeights();
    }
};

	Compare compare;
	Node *head;
	Node *max;

    //Build tree
	AVLTree(Compare compare) : compare(compare), head(0),max(0){}

    //Delete tree
	~AVLTree()
	{
		if(this->head!=0){
			this->head->DeleteSubtree();
			delete this->head;
		}
		head = max = NULL;
	}

    //Get first node
	Node *getFirst(){
		if(head == 0){
			return 0;
		}
		return head->getMostLeft();
	}

	//Get last node
	Node *getLast()
	{
		if(head == NULL){
			return NULL;
		}
		return head->getMostRight();
	}

	//Updates the max node
	void updateMax(){
		if(head == NULL)
			max = NULL;
		else
			max=this->getLast();
	}

	//Insert new node to the tree and balance the tree
	bool insertNode(T item)
	{
		if(this->head==0){
			this->head=new Node(item);
			this->updateMax();
			return true;
		}
		Node *node;
		if(findNode<T>(this->head, item, &node)){
			return false;
		}
		int equal = compare(item,node->info);
		Node *son = new Node(item);
		if(equal<0)
			node->insertLeft(son);
		else 
			node->insertRight(son);
		this->updateMax();
		return true;
	}

	//Remove an item from the tree and balance the tree
	template <class Key>
	bool removeNode(Key key)
	{
		if(this->head==0){
			return false;
		}
		Node *node;
		if(!findNode<Key>(this->head, key, &node))
			return false;
		if(node->left!=0&&node->right!=0){
			Node *nextSmallest = node->left;
		    while(nextSmallest->right!=0){
		    	nextSmallest=nextSmallest->right;
		    }
			nextSmallest->swapInfo(node);
			node=nextSmallest;
		}
		Node* son = node->right;
		if(node->left!=0)
			son = node->left;
		if(son!=0)
			son->parent=node->parent;
		if(node->parent==0)
			this->head=son;
		else {
			if(node->parent->left==node)
				node->parent->left=son;
			else 
				node->parent->right=son;
			node->parent->updateParentHeights();
		}
		delete node;
		this->updateMax();
		return true;
	}

	//Finds Node and return the next smaller node by pointer
	template <class Key>
	bool findNode(Node *node, Key info, Node **closestNode)
	{
		(*closestNode)=node;
		int equal = compare(info, node->info);
		if(equal==0){
			return true;
		}
		else if(equal>0&&node->right!=0){
			return findNode<Key>(node->right, info, closestNode);
		}
		else if (equal<0&&node->left!=0){
			return findNode<Key>(node->left, info, closestNode);
		}
		return false;
	}
	
	//gets K largest elements, and if theres not enough returns how much left
	int getKLargest (int k,T* arr){
		int count=0;
		Node* current = max;
		while(current!=0&&count<k){
			arr[count]=current->info;
			current = current->getNextSmaller();
			count++;
		}
		return k-count;
	}

};

#endif 