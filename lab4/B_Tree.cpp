#include <iostream>
#include <vector>
#include <cassert>

template <typename Key>
struct BNode {
    int              n;
    std::vector<Key> key;
    std::vector<BNode*> c;
    bool             leaf;

    explicit BNode(int t, bool isLeaf)
        : n(0), key(2*t - 1), c(2*t, nullptr), leaf(isLeaf) {}
};

template <typename Key>
void DiskRead(BNode<Key>* /*x*/)  { /* no-op in RAM */ }

template <typename Key>
void DiskWrite(BNode<Key>* /*x*/) { /* no-op in RAM */ }

template <typename Key>
BNode<Key>* AllocateNode(int t, bool isLeaf) {
    return new BNode<Key>(t, isLeaf);
}

template <typename Key>
class BTree {
public:
    using Node = BNode<Key>;

    explicit BTree(int minDegree) : t(minDegree) {
        assert(t >= 2);
        root = AllocateNode<Key>(t, true);
        root->leaf = true;
        root->n    = 0;
        DiskWrite(root);
    }

    ~BTree() { destroy(root); }

    std::pair<Node*, int> search(const Key& k) {
        return BTreeSearch(root, k);
    }

    void insert(const Key& k) {
        Node* r = root;
        if (r->n == 2*t - 1) {
            Node* s = AllocateNode<Key>(t, false);
            root    = s;
            s->leaf = false;
            s->n    = 0;
            s->c[0] = r;
            BTreeSplitChild(s, 0, r);
            BTreeInsertNonfull(s, k);
        } else {
            BTreeInsertNonfull(r, k);
        }
    }

    void remove(const Key& k) {
        if (!root) return;
        BTreeDelete(root, k);
        // if root became empty, shrink the tree
        if (root->n == 0 && !root->leaf) {
            Node* oldRoot = root;
            root = root->c[0];
            delete oldRoot;
        }
    }

    void print() const {
        std::cout << "B-Tree (t=" << t << "):\n";
        printNode(root, 0);
        std::cout << '\n';
    }

private:
    int   t;
    Node* root;

    // B-Tree-Search(x, k)
    std::pair<Node*, int> BTreeSearch(Node* x, const Key& k) {
        int i = 0;
        while (i < x->n && k > x->key[i])
            ++i;
        if (i < x->n && k == x->key[i])
            return {x, i};
        else if (x->leaf)
            return {nullptr, -1};
        else {
            DiskRead(x->c[i]);
            return BTreeSearch(x->c[i], k);
        }
    }

    // B-Tree-Split-Child(x, i, y)
    void BTreeSplitChild(Node* x, int i, Node* y) {
        Node* z = AllocateNode<Key>(t, y->leaf);
        z->n    = t - 1;

        for (int j = 0; j <= t-2; ++j)
            z->key[j] = y->key[j + t];

        if (!y->leaf)
            for (int j = 0; j <= t-1; ++j)
                z->c[j] = y->c[j + t];

        y->n = t - 1;

        for (int j = x->n; j >= i+1; --j)
            x->c[j+1] = x->c[j];
        for (int j = x->n - 1; j >= i; --j)
            x->key[j+1] = x->key[j];

        x->c[i+1] = z;
        x->key[i] = y->key[t-1];
        x->n     += 1;

        DiskWrite(y);
        DiskWrite(z);
        DiskWrite(x);
    }

    // B-Tree-Insert-Nonfull(x, k)
    void BTreeInsertNonfull(Node* x, const Key& k) {
        int i = x->n - 1;
        if (x->leaf) {
            while (i >= 0 && k < x->key[i]) {
                x->key[i+1] = x->key[i];
                --i;
            }
            x->key[i+1] = k;
            x->n       += 1;
            DiskWrite(x);
        } else {
            while (i >= 0 && k < x->key[i])
                --i;
            ++i;
            DiskRead(x->c[i]);
            if (x->c[i]->n == 2*t - 1) {
                BTreeSplitChild(x, i, x->c[i]);
                if (k > x->key[i])
                    ++i;
            }
            BTreeInsertNonfull(x->c[i], k);
        }
    }

    // B-Tree-Delete(x, k)
    void BTreeDelete(Node* x, const Key& k) {
        int i = findKeyIndex(x, k);

        if (i < x->n && x->key[i] == k) {
            if (x->leaf) {
                // Case 1: key in leaf, just remove
                removeFromLeaf(x, i);
            } else {
                Node* y = x->c[i];
                Node* z = x->c[i+1];

                if (y->n >= t) {
                    // Case 2a: replace with predecessor
                    Key kp = getPredecessor(y);
                    x->key[i] = kp;
                    BTreeDelete(y, kp);
                } else if (z->n >= t) {
                    // Case 2b: replace with successor
                    Key ks = getSuccessor(z);
                    x->key[i] = ks;
                    BTreeDelete(z, ks);
                } else {
                    // Case 2c: merge k + z into y
                    merge(x, i, y, z);
                    BTreeDelete(y, k);
                }
            }
        } else {
            if (x->leaf) return;

            // Case 3: ensure c[i] has at least t keys before descending
            if (x->c[i]->n == t - 1) {
                if (i > 0 && x->c[i-1]->n >= t) {
                    // Case 3a: borrow from left sibling
                    rotateRight(x, i);
                } else if (i < x->n && x->c[i+1]->n >= t) {
                    // Case 3a: borrow from right sibling
                    rotateLeft(x, i);
                } else {
                    // Case 3b: merge
                    if (i < x->n) {
                        merge(x, i, x->c[i], x->c[i+1]);
                    } else {
                        merge(x, i-1, x->c[i-1], x->c[i]);
                        i--;
                    }
                }
            }

            DiskRead(x->c[i]);
            BTreeDelete(x->c[i], k);
        }
    }

    int findKeyIndex(Node* x, const Key& k) {
        int i = 0;
        while (i < x->n && x->key[i] < k)
            ++i;
        return i;
    }

    void removeFromLeaf(Node* x, int i) {
        for (int j = i+1; j < x->n; ++j)
            x->key[j-1] = x->key[j];
        x->n--;
        DiskWrite(x);
    }

    Key getPredecessor(Node* y) {
        while (!y->leaf)
            y = y->c[y->n];
        return y->key[y->n - 1];
    }

    Key getSuccessor(Node* z) {
        while (!z->leaf)
            z = z->c[0];
        return z->key[0];
    }

    void merge(Node* x, int i, Node* y, Node* z) {
        y->key[y->n] = x->key[i];
        y->n++;

        for (int j = 0; j < z->n; ++j)
            y->key[y->n + j] = z->key[j];

        if (!y->leaf)
            for (int j = 0; j <= z->n; ++j)
                y->c[y->n + j] = z->c[j];

        y->n += z->n;

        for (int j = i+1; j < x->n; ++j)
            x->key[j-1] = x->key[j];
        for (int j = i+2; j <= x->n; ++j)
            x->c[j-1] = x->c[j];
        x->n--;

        DiskWrite(x);
        DiskWrite(y);
        delete z;
    }

    void rotateRight(Node* x, int i) {
        Node* child   = x->c[i];
        Node* sibling = x->c[i-1];

        for (int j = child->n - 1; j >= 0; --j)
            child->key[j+1] = child->key[j];
        if (!child->leaf)
            for (int j = child->n; j >= 0; --j)
                child->c[j+1] = child->c[j];

        child->key[0] = x->key[i-1];
        if (!child->leaf)
            child->c[0] = sibling->c[sibling->n];

        x->key[i-1] = sibling->key[sibling->n - 1];
        child->n++;
        sibling->n--;

        DiskWrite(x); DiskWrite(child); DiskWrite(sibling);
    }

    void rotateLeft(Node* x, int i) {
        Node* child   = x->c[i];
        Node* sibling = x->c[i+1];

        child->key[child->n] = x->key[i];
        if (!child->leaf)
            child->c[child->n + 1] = sibling->c[0];
        child->n++;

        x->key[i] = sibling->key[0];

        for (int j = 1; j < sibling->n; ++j)
            sibling->key[j-1] = sibling->key[j];
        if (!sibling->leaf)
            for (int j = 1; j <= sibling->n; ++j)
                sibling->c[j-1] = sibling->c[j];
        sibling->n--;

        DiskWrite(x); DiskWrite(child); DiskWrite(sibling);
    }

    void destroy(Node* x) {
        if (!x) return;
        if (!x->leaf)
            for (int i = 0; i <= x->n; ++i)
                destroy(x->c[i]);
        delete x;
    }

    void printNode(Node* x, int depth) const {
        if (!x) return;
        std::string indent(depth * 4, ' ');
        std::cout << indent << "[";
        for (int i = 0; i < x->n; ++i) {
            std::cout << x->key[i];
            if (i < x->n - 1) std::cout << " ";
        }
        std::cout << "]\n";
        if (!x->leaf)
            for (int i = 0; i <= x->n; ++i)
                printNode(x->c[i], depth + 1);
    }
};

int main() {
    std::cout << "=== B-Tree Demo (t=3, max 5 keys/node) ===\n\n";

    BTree<int> bt(3);

    for (int k : {7,13,16,23,1,3,4,5,10,11,14,15,18,19,20,21,22,24,26})
        bt.insert(k);

    std::cout << "Initial tree:\n";
    bt.print();

    bt.insert(2);
    std::cout << "After inserting 2:\n";
    bt.print();

    bt.insert(17);
    std::cout << "After inserting 17:\n";
    bt.print();

    bt.insert(12);
    std::cout << "After inserting 12:\n";
    bt.print();

    bt.insert(6);
    std::cout << "After inserting 6:\n";
    bt.print();

    std::cout << "\n=== Deletions ===\n\n";

    bt.remove(6);
    std::cout << "After deleting 6 (Case 1 - leaf):\n";
    bt.print();

    bt.remove(13);
    std::cout << "After deleting 13 (Case 2a - predecessor):\n";
    bt.print();

    bt.remove(7);
    std::cout << "After deleting 7 (Case 2c - merge):\n";
    bt.print();

    bt.remove(4);
    std::cout << "After deleting 4 (Case 3b - merge):\n";
    bt.print();

    bt.remove(2);
    std::cout << "After deleting 2 (Case 3a - rotation):\n";
    bt.print();

    std::cout << "=== Search ===\n";
    for (int k : {5, 16, 99}) {
        auto result = bt.search(k);
        if (result.first)
            std::cout << "search(" << k << ") -> found at index " << result.second << "\n";
        else
            std::cout << "search(" << k << ") -> NOT FOUND\n";
    }

    return 0;
}
