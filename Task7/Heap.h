#pragma once

class Heap
{
public:
	Heap() = default;
	Heap(Request _neutral);
	~Heap();

	bool empty();
	Request get_min();
	Request pop_min();
	void insert(Request new_key);
	void show_heap();

private:
	enum NODE_TYPE {
		HEAD = 0,
		LEFT = 1,
		RIGHT = 2,
		NONE = 3
	};
	struct node {
		node* parent = NULL;
		node* left_child = NULL;
		node* right_child = NULL;
		Request key;
		NODE_TYPE type;

		node(NODE_TYPE _type, Request _key);
	};
	
	node* head = NULL;
	int tail;
	Request neutral;

	void delete_heap(node* ver);
	node* find_ver(int copy_tail);
	void heap_insert(Request new_key);
	Request get_heap_min();
	void show_heap(node* ver, int level);
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
	return get_heap_min();
}

void Heap::insert(Request new_key) 
{
	heap_insert(new_key);
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

	while (copy_tail > 1)
	{
		if ((copy_tail) % 2 == 0)
			path.push_back(LEFT);
		else
			path.push_back(RIGHT);
		copy_tail /= 2;
	}

	node* ver = head;
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

void Heap::heap_insert(Request new_key)
{
	node* ver = head;
	ver = find_ver(tail);

	tail++;
	ver->key = new_key;
	ver->left_child = new node(LEFT, neutral);
	ver->right_child = new node(RIGHT, neutral);
	ver->left_child->parent = ver;
	ver->right_child->parent = ver;

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

Request Heap::get_heap_min()
{
	Request res = head->key;

	if (res == neutral)
		return res;

	node* ver = head;
	ver = find_ver(tail);

	tail--;
	ver = find_ver(tail);
	head->key = ver->key;
	ver->key = neutral;
	ver->left_child = NULL;
	ver->right_child = NULL;
	delete ver->left_child;
	delete ver->right_child;

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