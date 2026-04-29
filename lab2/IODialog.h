/*
 * IODialog.h
 */

#ifndef IODIALOG_H_
#define IODIALOG_H_

#include <iostream>
#include <sstream>
#include <list>

using namespace std;

namespace IODialog {


void showTreeOperation() {
	cout << " 1. Add nodes" << endl
		 << " 2. Delete node" << endl
		 << " 3. Get minimum node" << endl
		 << " 4. Get maximum node" << endl
		 << " 5. Get successor of a node" << endl
		 << " 6. Get predecessor of a node" << endl
		 << " 7. Show tree" << endl
		 << " 8. Show inorder traversal" << endl
		 << " 9. Show black-height of the tree" << endl
		 << "10. Show the maximum key of a black node of the tree" << endl
		 << "11. Show the maximum key of a red node of the tree" << endl
		 << "12. Show tree depth" << endl
		 << "13. Exit" << endl
		 << "Enter your choice (1-13): ";
}

int getTreeOperation() {
	int option = 0;
	while (true) {
		showTreeOperation();
		if (!(cin >> option)) {
			cout << "Unknown option. Try again." << endl;
			cin.clear();
			cin.ignore(10000, '\n');
		} else if (option < 1 || option > 13) {
			cout << "Unknown option. Try again." << endl;
		} else {
			cout << "Proceed with selection " << option << " .." << endl;
			cin.ignore(10000, '\n');
			break;
		}
	}
	return option;
}

void getNodeKeys(list<int>& nodeKeys) {
	cout << "Type in the keys of the nodes: " << flush;
	int key;
	string line;
	if (getline(cin,line)) {
		istringstream iss(line);
		while (iss >> key) {
			nodeKeys.push_back(key);
		}
	}
}

int getNodeKey() {
	cout << "Type in the key of the node: ";
	int k;
	cin >> k;
	return k;
}
}

#endif /* IODIALOG_H_ */
