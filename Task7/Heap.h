#pragma once

class Heap
{
public:
	Heap(int _neutral) {
		neutral = _neutral;

		head = new node(HEAD, neutral);
		tail = 1;
	}

	int get_min()
	{
		return get_heap_min();
	}

	void insert(int new_key) {
		heap_insert(new_key);
	}

	void show_heap() {
		show_heap(head, 0);
	}

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
		int key;
		NODE_TYPE type;

		node(NODE_TYPE _type, int _key) {
			type = _type;
			key = _key;
		}
	};
	
	node* head = NULL;
	int tail;
	int neutral;
	int type;

	node* find_ver(int copy_tail)
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

	void heap_insert(int new_key)
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
			if (ver->parent->key > ver->key) {
				swap(ver->parent->key, ver->key);
				ver = ver->parent;
			}
			else
				break;
		}
	}

	int get_heap_min() 
	{
		int res = head->key;

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

	void show_heap(node* ver, int level)
	{
		if (ver->left_child)
			show_heap(ver->left_child, level + 1);
		for (int i = 0; i < level; ++i)
			cout << "   ";
		cout << ver->key << '\n';
		if (ver->right_child)
			show_heap(ver->right_child, level + 1);
	}
};