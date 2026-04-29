#include <iostream>
#include "RBNode.h"
using namespace std;

int main() {
    RBTree tree;
    int choice, val;

    while (true) {
        cout << "\n1-Insert  2-Delete  3-Search  4-Min  5-Max  6-Display  0-Exit\nChoice: ";
        cin >> choice;
        if (choice == 0) break;

        switch (choice) {
        case 1:
            cout << "Value: "; cin >> val;
            tree.RBInsert(tree.createNode(val));
            cout << "Inserted " << val << endl;
            break;
        case 2:
            cout << "Value: "; cin >> val;
            {
                RBNode* z = tree.search(tree.root, val);
                if (tree.isNil(z)) { cout << "Not found." << endl; break; }
                RBNode* removed = tree.del(z);
                removed->left = removed->right = RBNode::Nil;
                delete removed;
                cout << "Deleted " << val << endl;
            }
            break;
        case 3:
            cout << "Value: "; cin >> val;
            {
                RBNode* r = tree.search(tree.root, val);
                if (tree.isNil(r)) cout << "Not found." << endl;
                else cout << "Found: " << r->toString() << endl;
            }
            break;
        case 4:
            if (tree.isNil(tree.root)) cout << "Tree is empty." << endl;
            else cout << "Min: " << tree.minimum(tree.root)->toString() << endl;
            break;
        case 5:
            if (tree.isNil(tree.root)) cout << "Tree is empty." << endl;
            else cout << "Max: " << tree.maximum(tree.root)->toString() << endl;
            break;
        case 6:
            tree.indentedDisplay();
            cout << "Inorder: "; tree.inorder(); cout << endl;
            cout << "Depth: " << tree.depth() << "  BH: " << tree.bh() << endl;
            cout << "MaxBlackKey: " << tree.maxBlackKey() << "  MaxRedKey: " << tree.maxRedKey() << endl;
            break;
        default:
            cout << "Unknown option." << endl;
        }
    }
    return 0;
}
