#pragma once

void treap_tests()
{
	int n, i, j, x;

	cin >> n;
	Treap<int>* tree = new Treap<int>;

	for (i = 0; i < n; ++i) {
		cin >> x;
		tree->insert(x);
		tree->show_treap();
		cout << "---------------------------------------\n";
	}
}


void queue_tests()
{
	int n, i, j, x, op;

	Heap* H = new Heap(Request());

	cin >> n;
	for (i = 0; i < n; ++i) {
		cin >> op;
		if (op == 0) {
			H->insert(Request(i + 1, i + 1, READ_START, i + 1, i + 1));
		}
		else
		{
			x = H->get_min().get_start_time();
			cout << '\t' << x << '\n';
		}
		H->show_heap();
		cout << "---------------------------------------\n";
	}
}

void do_tests()
{
	//treap_tests();
	//queue_tests();

}