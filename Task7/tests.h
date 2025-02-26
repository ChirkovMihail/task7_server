#pragma once

void do_tests()
{
	int n, i, j,x;

	cin >> n;
	Treap<int> *tree = new Treap<int>;

	for (i = 0; i < n; ++i) {
		cin >> x;
		tree->insert(x);
		tree->show_treap();
		cout << "---------------------------------------\n";
	}
}