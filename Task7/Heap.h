#pragma once

class Heap
{
public:
	Heap() = default;
	Heap(Request _neutral);
	~Heap();

	bool empty();					//returns true if heap is empty, otherwise false
	Request get_min();				//returns min element from heap (O(1))
	Request pop_min();				//returns and extract min element from heap (O(logn))
	void insert(Request new_key);	//insert new element in heap (O(logn))
	void show_heap();				//print heap structure in console

private:
	enum NODE_TYPE {
		HEAD = 0,
		LEFT = 1,
		RIGHT = 2,
		NONE = 3
	};
	struct node {
		node* parent = NULL;		//pointer to ancestor
		node* left_child = NULL;	//pointer to left subtree
		node* right_child = NULL;	//pointer to right subtree
		Request key;				//key for comparision
		NODE_TYPE type;				//structure type of this vertice

		node(NODE_TYPE _type, Request _key);
	};
	
	node* head = NULL;	//pointer to peak of the heap
	int tail;			//index of the first unfilled vertice of heap
	Request neutral;	//element that is bigger than any other in the heap

	void delete_heap(node* ver);			//frees memory
	node* find_ver(int copy_tail);			//finds vertice by index due to the structure of binary tree
	void show_heap(node* ver, int level);	//recursively shows vertices
};

Heap::Heap(Request _neutral)
{
	neutral = _neutral;
	head = new node(HEAD, neutral);
	tail = 1;
}

Heap::~Heap()
{
	delete_heap(head);
}

bool Heap::empty()
{
	return head->key == neutral;
}

Request Heap::get_min()
{
	return head->key;
}

Request Heap::pop_min()
{
	Request res = head->key;

	//if heap is empty
	if (res == neutral)
		return res;

	//otherwise we need to delete min value
	node* ver = head;
	tail--;
	//find last filled element in heap
	ver = find_ver(tail);

	//swap it with the value we need to delete, and delete it
	head->key = ver->key;
	ver->key = neutral;
	ver->left_child = NULL;
	ver->right_child = NULL;
	delete ver->left_child;
	delete ver->right_child;

	//fix the structure of the heap due to
	//ver->key > max(ver->left_child->key, ver->right_child->key)
	//on the way from head to leafs
	ver = head;
	while (ver->left_child && ver->right_child)
	{
		if (ver->left_child->key < ver->right_child->key) {
			if (ver->key > ver->left_child->key) {
				swap(ver->key, ver->left_child->key);
				ver = ver->left_child;
				continue;
			}
		}
		else {
			if (ver->key > ver->right_child->key) {
				swap(ver->key, ver->right_child->key);
				ver = ver->right_child;
				continue;
			}
		}
		break;
	}

	return res;
}

void Heap::insert(Request new_key) 
{
	//find the first unfilled vertice
	node* ver = head;
	ver = find_ver(tail);

	tail++;
	//write information
	ver->key = new_key;
	ver->left_child = new node(LEFT, neutral);
	ver->right_child = new node(RIGHT, neutral);
	ver->left_child->parent = ver;
	ver->right_child->parent = ver;

	//fix the structure of the heap due to
	//ver->key > max(ver->left_child->key, ver->right_child->key)
	//on the way from leaf to head
	while (ver->parent)
	{
		if (ver->key < ver->parent->key) {
			swap(ver->parent->key, ver->key);
			ver = ver->parent;
		}
		else
			break;
	}
}

void Heap::show_heap()
{
	show_heap(head, 0);
}

Heap::node::node(NODE_TYPE _type, Request _key) 
{
	type = _type;
	key = _key;
}

void Heap::delete_heap(node* ver)
{
	//deallocate binary tree
	if (ver->left_child)
		delete_heap(ver->left_child);
	if (ver->right_child)
		delete_heap(ver->right_child);
	delete ver;
}

Heap::node* Heap::find_ver(int copy_tail)
{
	vector<NODE_TYPE> path;

	copy_tail = tail;

	// go up, collecting directions in path
	while (copy_tail > 1)
	{
		if ((copy_tail) % 2 == 0)
			path.push_back(LEFT);
		else
			path.push_back(RIGHT);
		copy_tail /= 2;
	}

	node* ver = head;
	//go down due to the path
	while (!path.empty())
	{
		if (path.back() == LEFT) {
			ver = ver->left_child;
		}
		if (path.back() == RIGHT) {
			ver = ver->right_child;
		}
		path.pop_back();
	}
	return ver;
}

void Heap::show_heap(node* ver, int level)
{
	if (ver->left_child)
		show_heap(ver->left_child, level + 1);
	for (int i = 0; i < level; ++i)
		cout << "   ";
	cout << ver->key.get_start_time() << '\n';
	if (ver->right_child)
		show_heap(ver->right_child, level + 1);
}