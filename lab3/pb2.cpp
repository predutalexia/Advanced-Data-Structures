#include <iostream>
#include <queue>
#include <unordered_map>

/**
 * Problem 2: Dynamic Median with Removals
 */
class MedianFinder {
private:
    std::priority_queue<int> left; // Max-heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> right; // Min-heap
    std::unordered_map<int, int> delayed_removals;
    int size = 0;

    template<typename T>
    void clean(T& heap) {
        while (!heap.empty() && delayed_removals[heap.top()] > 0) {
            delayed_removals[heap.top()]--;
            heap.pop();
        }
    }

    void balance() {
        if (left.size() > right.size() + 1) {
            right.push(left.top());
            left.pop();
            clean(left);
        } else if (right.size() > left.size()) {
            left.push(right.top());
            right.pop();
            clean(right);
        }
    }

public:
    void add(int x) {
        if (left.empty() || x <= left.top()) {
            left.push(x);
        } else {
            right.push(x);
        }
        size++;
        clean(left);
        clean(right);
        balance();
    }

    void remove(int x) {
        if (size == 0) return;
        delayed_removals[x]++;
        if (x <= left.top()) {
        } else {
        }
        size--;
        clean(left);
        clean(right);
        balance();
    }

    int getMedian() {
        clean(left);
        return left.top();
    }
};

int main() {
    MedianFinder mf;
    mf.add(5);
    mf.add(2);
    mf.add(10);
    std::cout << mf.getMedian() << std::endl; 
    mf.add(7);
    std::cout << mf.getMedian() << std::endl; 
    mf.remove(5);
    std::cout << mf.getMedian() << std::endl;
}